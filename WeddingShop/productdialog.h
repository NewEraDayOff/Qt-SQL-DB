#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "mysql.h"
#include "reviewdialog.h"

namespace Ui {
class ProductDialog;
}

class ProductDialog : public QDialog
{
    Q_OBJECT

public slots:
    inline void setID(unsigned int id) { this->id = id; } //сеттер индекса
    inline void setProduct(unsigned int product) { this->product = product; } //сеттер индекса продукта
    void openDialog(const bool isUser);

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

signals:
    void openCatalog(unsigned int id, bool isUser);

private slots:

    void on_pushButton_buy_clicked();

    void on_pushButton_rate_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void updateReviewList();

private:
    Ui::ProductDialog *ui;
    unsigned int seller = 0;
    unsigned int id = 0;
    unsigned int product = 0;
    bool isUser = true;

    ReviewDialog *rd = nullptr;
};

#endif // PRODUCTDIALOG_H
