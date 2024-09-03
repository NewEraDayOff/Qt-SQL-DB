#ifndef CATALOGDIALOG_H
#define CATALOGDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "productdialog.h"
#include "mysql.h"

namespace Ui {
class CatalogDialog;
}

class CatalogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CatalogDialog(QWidget *parent = nullptr);
    ~CatalogDialog();
    inline void setUser(unsigned int id) { this->user = id; } //сеттер пользователя
    inline void setSeller(unsigned int id) { this->id = id; } //сеттер продавца
    inline void setIsUser(bool isUser) { this->isUser = isUser; } //сеттер условия пользователь или продавец

public slots:
    void open() override;

private:
    unsigned int user = 0;
    unsigned int id = 0;
    bool isUser = false;
    ProductDialog *pd = nullptr;
    Ui::CatalogDialog *ui;
};

#endif // CATALOGDIALOG_H
