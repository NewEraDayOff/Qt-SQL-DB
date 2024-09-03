#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>

#include "mysql.h"

namespace Ui {
class RegDialog;
}

class RegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegDialog(QWidget *parent = nullptr);
    ~RegDialog();

    void open() override;

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::RegDialog *ui;

signals:
    void sendLogin(const QString &);
};

#endif // REGDIALOG_H
