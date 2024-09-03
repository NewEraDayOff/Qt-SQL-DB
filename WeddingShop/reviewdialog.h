#ifndef REVIEWDIALOG_H
#define REVIEWDIALOG_H

#include <QDialog>

#include "mysql.h"

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewDialog(QWidget *parent = nullptr);
    ~ReviewDialog();

public slots:
    inline void setID(unsigned int id) { this->id = id; } //сеттер индекса
    inline void setProduct(unsigned int product) { this->product = product; } //сеттер индекса товара
    void setUser(bool me);

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::ReviewDialog *ui;
    unsigned int product = 0;
    unsigned int id = 0;
    unsigned int myReviewID = 0;
    bool me = false;
    bool myReviewExist = false;

};

#endif // REVIEWDIALOG_H
