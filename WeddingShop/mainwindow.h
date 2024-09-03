#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "mysql.h"
#include "Login/logindialog.h"
#include "Seller/sellerdialog.h"
#include "productdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
//    bool logged = false;
    unsigned int id = 0;
    QString name;
    Ui::MainWindow *ui;
    LoginDialog ld;
    SellerDialog sd;
    ProductDialog *pd = nullptr;
    QWidget *curPage = nullptr;

    enum TableColumns { CATALOG = 7, CART = 5, HISTORY = 5 };
    enum CatalogType { ALL, PRICE, MATERIAL, COLOR };
    enum PriceType { F25T60 = 1, F60TO100, F100TO400 };
    enum MaterialType { LACE = 1, SATIN, CHIFFON, FATIN, SPARKLY, TRANSPARENT, MTRL_OTHER };
    enum ColorType { IVORY = 1, BEIGE, WHITE, LACTIC, DUSTY_ROSE, COL_OTHER };
    void connectAction(QAction *a, int num, int index);

private slots:
    void gotoProfile();
    void gotoCollection();
    void gotoCart();
    void gotoLogin();
    void loadCatalog();
    inline void setUserID(unsigned int id) { this->id = id; } //сеттер индекса пользователя
    void on_pushButton_cart_clicked();
    void on_pushButton_prf_name_clicked();
};
#endif // MAINWINDOW_H
