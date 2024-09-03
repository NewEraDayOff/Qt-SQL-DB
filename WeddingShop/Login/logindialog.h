#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "mysql.h"
#include "Login/regdialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public slots:
    void open() override;

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_reg_clicked();

private:
    RegDialog rd;
    Ui::LoginDialog *ui;

signals:
    void user(unsigned int id);
    void user(const QString &name);

    void seller(unsigned int id);
    void seller(const QString &name);
};

#endif // LOGINDIALOG_H
