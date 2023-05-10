#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "password_crypt.h"
#include "api_helpers.h"
#include "httplib.h"
#include "logindialog.h"

#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QPixmap>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::try_login() {
    std::string key = "VQZBJ6TD8M9WBUWT";
    std::string iv = "joiwef08u23j341a";

    LoginDialog dlg(this);

    if (dlg.exec()) {
        this->pass = dlg.getPass().toStdString();
        this->email = dlg.getEmail().toStdString();
        std::string pass_encrypted = password_crypt(this->pass, iv, key);
        std::string url = "/api/login.action?m=1&uid="+this->email+"&pwd="+pass_encrypted;
        httplib::Client cli("https://www.storytel.com");
        auto res = cli.Get(url);
        if (res->status!=200) {
            return false;
        }
        this->loginResponse = res->body;
        return true;
    }
    return false;
}

void MainWindow::showList() {
    if (try_login()) {
        this->token = getToken(this->loginResponse);
        std::string bookshelf = getBookshelf(token);
        this->books = parseEntries(bookshelf);

        for (auto &it : this->books) {
            QListWidgetItem * newItem = new QListWidgetItem;
            newItem->setText(QString::fromStdString(it.book.title));
            ui->listWidget->addItem(newItem);
        }

        connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::displayCover);

    } else {
        QMessageBox::warning(
            this,
            tr("Login"),
            tr("Incorrect login information.") );
        this->close();
    }
}

void MainWindow::displayCover() {
    this->idx = ui->listWidget->currentRow();
    const QUrl coverUrl("https://storytel.com"+QString::fromStdString(this->books[this->idx].imgUrl));
    this->cd = new CoverDownloader(coverUrl, this);
    connect(this->cd, SIGNAL (downloaded()), this, SLOT(loadImage()));
}

void MainWindow::loadImage() {
    QPixmap img;
    img.loadFromData(this->cd->downloadedData());
    ui->coverView->setPixmap(img);
}

void MainWindow::mediaPlayerInit() {
    this->player = new QMediaPlayer;
    this->player->setVolume(80);
}

void MainWindow::on_playBtn_clicked()
{
    this->idx = ui->listWidget->currentRow();
    const QUrl streamUrl = QUrl("https://www.storytel.com/mp3streamRangeReq?startposition=0&programId="+
                                QString::number(books[this->idx].abook.id)+"&token="+QString::fromStdString(this->token));

    this->player->setMedia(streamUrl);
    this->player->play();

    connect(this->player, &QMediaPlayer::seekableChanged, this, [=]() {
        player->setPosition((this->books[this->idx].bookmark.position == -1) ? 0 : this->books[this->idx].bookmark.position/1000);
    });



}
void MainWindow::on_pauseBtn_clicked()
{
    int64_t duration = player->duration();
    setBookmark(this->token, std::to_string(this->books[this->idx].bookmark.id), duration,
                (this->books[this->idx].bookmark.position == -1) ? 0 : this->books[this->idx].bookmark.position);
    player->pause();
}


void MainWindow::on_forwardBtn_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState) player->setPosition(player->position() + 5000);

}


void MainWindow::on_backBtn_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState) player->setPosition(player->position() - 5000);

}

void MainWindow::initKeyboardShortcuts() {
    QShortcut *shortcut_Fwd = new QShortcut(QKeySequence(Qt::Key_Right), this);
    QShortcut *shortcut_Back = new QShortcut(QKeySequence(Qt::Key_Left), this);

    QObject::connect(shortcut_Fwd, &QShortcut::activated, this, &MainWindow::on_forwardBtn_clicked);
    QObject::connect(shortcut_Back, &QShortcut::activated, this, &MainWindow::on_backBtn_clicked);

}

