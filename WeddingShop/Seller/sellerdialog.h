#ifndef SELLERDIALOG_H
#define SELLERDIALOG_H

#include <QDialog>
#include <QInputDialog>
#include <QComboBox>

#include "mysql.h"
#include "Seller/catalogdialog.h"

namespace Ui {
class SellerDialog;
}

class SellerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SellerDialog(QWidget *parent = nullptr);
    ~SellerDialog();

private:
    unsigned int id;
    int catalog;
    QString name;
    Ui::SellerDialog *ui;
    CatalogDialog *cd = nullptr;

    void setPropertyToComboBox(MyProperty &prop, QVector<QComboBox *> cbvec);
    void setProductToComboBox(MyProduct &prod, QVector<QComboBox *> cbvec);
    void connectCheckBoxWithSlots(QCheckBox *chb, QComboBox *cob, QLineEdit *le);

public slots:
    inline void setID(unsigned int id) { this->id = id; } //сеттер индекса
    void setName(const QString &name);
    void openCatalog(unsigned int seller, bool meUser, unsigned int user = 0);

private slots:
    void clearAddTab();
    void clearUpdTab();

    void loadAddTab();
    void loadUpdTab();
    void loadProfile();

    void floadAddTab();
    void floadUpdTab();

    void on_pushButton_ok_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_comboBox_product_u_currentIndexChanged(int index);
    void on_pushButton_ok_u_clicked();
    void on_pushButton_ok_d_clicked();
    void on_pushButton_name_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_catalog_clicked();
};

#endif // SELLERDIALOG_H
