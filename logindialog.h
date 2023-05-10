#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <string>


namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QString getEmail();
    QString getPass();

private:
    Ui::LoginDialog *ui;
    QString email;
    QString pass;

private slots:
    void on_submitBtn_clicked();


};

#endif // LOGINDIALOG_H
