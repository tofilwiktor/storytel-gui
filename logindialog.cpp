#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_submitBtn_clicked()
{
    this->pass = ui->passField->text();
    this->email = ui->emailField->text();
    QDialog::accept();
}

QString LoginDialog::getEmail() {
    return this->email;
}

QString LoginDialog::getPass() {
    return this->pass;
}
