#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) //конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Магазин");
    this->ui->stackedWidget->setCurrentIndex(0);
    this->pd = new ProductDialog(this);

    //при нажатии QAction"Смотреть все"
    this->connectAction(this->ui->action_catalog, ALL, ALL);

    //при нажатии QAction"От 25 000 до 60 000 руб"
    this->connectAction(this->ui->action_25to60, PRICE, F25T60);
    //при нажатии QAction"От 60 000 до 100 000 руб"
    this->connectAction(this->ui->action_60to100, PRICE, F60TO100);
    //при нажатии QAction"От 100 000 до 400 000 руб"
    this->connectAction(this->ui->action_100to400, PRICE, F100TO400);

    //при нажатии QAction"Кружевные"
    this->connectAction(this->ui->action_lace, MATERIAL, LACE);
    //при нажатии QAction"Атлас"
    this->connectAction(this->ui->action_satin, MATERIAL, SATIN);
    //при нажатии QAction"Шифон"
    this->connectAction(this->ui->action_chiffon, MATERIAL, CHIFFON);
    //при нажатии QAction"Фатин"
    this->connectAction(this->ui->action_fatin, MATERIAL, FATIN);
    //при нажатии QAction"Блестящие"
    this->connectAction(this->ui->action_sparkly, MATERIAL, SPARKLY);
    //при нажатии QAction"Прозрачные"
    this->connectAction(this->ui->action_transparent, MATERIAL, TRANSPARENT);
    //при нажатии QAction"Другой"
    this->connectAction(this->ui->action_mtrl_other, MATERIAL, MTRL_OTHER);

    //при нажатии QAction"Айвори"
    this->connectAction(this->ui->action_ivory, COLOR, IVORY);
    //при нажатии QAction"Бежевый"
    this->connectAction(this->ui->action_beige, COLOR, BEIGE);
    //при нажатии QAction"Белый"
    this->connectAction(this->ui->action_white, COLOR, WHITE);
    //при нажатии QAction"Молочный"
    this->connectAction(this->ui->action_lactic, COLOR, LACTIC);
    //при нажатии QAction"Пыльная роза"
    this->connectAction(this->ui->action_dusty_rose, COLOR, DUSTY_ROSE);
    //при нажатии QAction"Другой"
    this->connectAction(this->ui->action_col_other, COLOR, COL_OTHER);


    QObject::connect(this->ui->action_profile, &QAction::triggered, this, &MainWindow::gotoProfile);
    QObject::connect(this->ui->action_history, &QAction::triggered, this, &MainWindow::gotoCollection);
    QObject::connect(this->ui->action_cart, &QAction::triggered, this, &MainWindow::gotoCart);

    QObject::connect(this->ui->action_login, &QAction::triggered, this, &MainWindow::gotoLogin);

    QObject::connect(this->ui->lineEdit_srch, &QLineEdit::textEdited, this, &MainWindow::loadCatalog);

    QObject::connect(this->ui->comboBox_srch_pr, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::loadCatalog);
    QObject::connect(this->ui->comboBox_srch_mtr, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::loadCatalog);
    QObject::connect(this->ui->comboBox_srch_col, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::loadCatalog);

    //задаем индекс пользователя
    QObject::connect(&this->ld, QOverload<unsigned int>::of(&LoginDialog::user), this, &MainWindow::setUserID);
    //задаем название QMenu(menu_login)
    QObject::connect(&this->ld, QOverload<const QString &>::of(&LoginDialog::user), this->ui->menu_login, &QMenu::setTitle);
    //задаем название QAction(action_login)
    QObject::connect(&this->ld, QOverload<const QString &>::of(&LoginDialog::user),
                     this, [=](const QString &name){ this->ui->action_login->setText(QString("Выйти из аккаунта %1").arg(name)); });
    //открываем главное окно, если вошли как пользователь
    QObject::connect(&this->ld, QOverload<unsigned int>::of(&LoginDialog::user), this, &QWidget::show);
    QObject::connect(&this->ld, QOverload<const QString &>::of(&LoginDialog::user), this, [=](const QString &name){ this->name = name; });
    //открываем главное окно, если отменили вход
    QObject::connect(&this->ld, &QDialog::rejected, this, &QWidget::show);
    //задаем индекс продавца
    QObject::connect(&this->ld, QOverload<unsigned int>::of(&LoginDialog::seller), &this->sd, &SellerDialog::setID);
    //задаем логин продавца
    QObject::connect(&this->ld, QOverload<const QString &>::of(&LoginDialog::seller), &this->sd, &SellerDialog::setName);

    //открываем окно продавца, если вошли как продавец
    QObject::connect(&this->ld, QOverload<unsigned int>::of(&LoginDialog::seller), &this->sd, &QDialog::open);

    //закрываем программу, если вышли из окна продавца
    QObject::connect(&this->sd, &QDialog::rejected, this, &QApplication::quit);

    QObject::connect(&this->sd, &QDialog::accepted, &this->ld, &QDialog::open);

    QObject::connect(this->ui->pushButton_toc, &QPushButton::clicked, this->ui->action_catalog, &QAction::trigger);

    this->ui->tableWidget_catalog->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableWidget_cart->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableWidget_collection->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow() //деструктор
{
    delete this->ui;
}

void MainWindow::connectAction(QAction *a, int num, int index) //коннекты действий
{
    //очищаем строку поиска
    QObject::connect(a, &QAction::triggered, this->ui->lineEdit_srch, &QLineEdit::clear);
    //задаем ценовую категорию
    QObject::connect(a, &QAction::triggered, std::bind(&QComboBox::setCurrentIndex,
                                                       this->ui->comboBox_srch_pr, num == 1 ? index : 0));
    //задаем материал
    QObject::connect(a, &QAction::triggered, std::bind(&QComboBox::setCurrentIndex,
                                                       this->ui->comboBox_srch_mtr, num == 2 ? index : 0));
    //задаем цвет
    QObject::connect(a, &QAction::triggered, std::bind(&QComboBox::setCurrentIndex,
                                                       this->ui->comboBox_srch_col, num == 3 ? index : 0));
    //загружаем каталог
    QObject::connect(a, &QAction::triggered, this, &MainWindow::loadCatalog);
    //открываем каталог
    QObject::connect(a, &QAction::triggered, std::bind(&QStackedWidget::setCurrentWidget,
                                                       this->ui->stackedWidget, this->ui->page_catalog));
}

void MainWindow::gotoProfile() //перейти в профиль
{
    this->curPage = this->ui->page_profile;
    if (!this->id)
    {
        this->hide();
        this->ld.open();
        return;
    }

    this->ui->label_prf_name->setText(this->name);
    this->ui->label_prf_r->setText(QString("Количество отзывов: %1").arg(MySql::review().wuser(this->id).get().size()));
    this->ui->label_prf_amount->setText(QString("Количество приобретенного товара: %1").arg(MySql::history().wuser(this->id).get().size()));

    this->ui->stackedWidget->setCurrentWidget(this->curPage);
}

void MainWindow::gotoCollection() //перейти в историю покупок
{
    this->curPage = this->ui->page_collection;
    if (!this->id)
    {
        this->hide();
        this->ld.open();
        return;
    }

    MySql::history().wuser(this->id).get();
    const int ROWS = MySql::history().info().size();
    if (ROWS < 1)
    {
        QMessageBox::information(this, "Оповещение", "Вы еще не купили ни одного товара");
        return;
    }
    this->ui->tableWidget_collection->clearContents();
    this->ui->tableWidget_collection->setRowCount(ROWS);
    unsigned int crow = 0;
    while (MySql::history().info().next())
    {
        for (unsigned int col = 0; col < HISTORY; ++col)
        {
            if (this->ui->tableWidget_collection->item(crow, col) == nullptr)
            {
                this->ui->tableWidget_collection->setItem(crow, col, new QTableWidgetItem());
            }
        }

        const unsigned int PRODUCT = MySql::history().info().value(SQL_PRODUCT).toUInt();
        const QDate DATE = MySql::history().info().value(SQL_DATE).toDate();

        MySql::product(PRODUCT).name().price().seller().get().first();
        const QString PRODUCT_NAME = MySql::product().info().value(SQL_NAME).toString();
        const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();
        const unsigned int SELLER = MySql::product().info().value(SQL_SELLER).toUInt();

        MySql::seller(SELLER).name().get().first();
        const QString SELLER_NAME = MySql::seller().info().value(SQL_NAME).toString();

        this->ui->tableWidget_collection->item(crow, 0)->setText(PRODUCT_NAME);
        this->ui->tableWidget_collection->item(crow, 0)->setToolTip(PRODUCT_NAME);

        this->ui->tableWidget_collection->item(crow, 1)->setText(QString("%1 руб.").arg(PRICE));
        this->ui->tableWidget_collection->item(crow, 1)->setToolTip(QString("%1 руб.").arg(PRICE));

        auto pb = new QPushButton(SELLER_NAME, this->ui->tableWidget_collection);
        pb->setProperty("seller", SELLER_NAME);
        pb->setToolTip(SELLER_NAME);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            this->sd.openCatalog(SELLER, true, this->id);
        });
        this->ui->tableWidget_collection->setCellWidget(crow, 2, pb);

        this->ui->tableWidget_collection->item(crow, 3)->setText(QLocale(QLocale::Russian).toString(DATE, QLocale::ShortFormat));
        this->ui->tableWidget_collection->item(crow, 3)->setToolTip(QLocale(QLocale::Russian).toString(DATE, QLocale::LongFormat));

        pb = new QPushButton("Посмотреть", this->ui->tableWidget_collection);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            this->pd->setID(this->id);
            this->pd->setProduct(PRODUCT);
            this->pd->openDialog(true);
        });
        this->ui->tableWidget_collection->setCellWidget(crow, 4, pb);

        ++crow;
    }

    this->ui->stackedWidget->setCurrentWidget(this->curPage);
}

void MainWindow::gotoCart() //перейти в корзину
{
    this->curPage = this->ui->page_cart;
    if (!this->id)
    {
        this->hide();
        this->ld.open();
        return;
    }

    MySql::cart().wuser(this->id).get();
    const int ROWS = MySql::cart().info().size();
    if (ROWS < 1)
    {
        QMessageBox::information(this, "Оповещение", "Ваша корзина пуста");
        return;
    }
    this->ui->tableWidget_cart->clearContents();
    this->ui->tableWidget_cart->setRowCount(ROWS);
    unsigned int crow = 0;
    while (MySql::cart().info().next())
    {
        for (unsigned int col = 0; col < CART; ++col)
        {
            if (this->ui->tableWidget_cart->item(crow, col) == nullptr)
            {
                this->ui->tableWidget_cart->setItem(crow, col, new QTableWidgetItem());
            }
        }

        const unsigned int PRODUCT = MySql::cart().info().value(SQL_PRODUCT).toUInt();
        const QDate DATE = MySql::cart().info().value(SQL_DATE).toDate();
        MySql::product(PRODUCT).name().price().get().first();
        const QString PRODUCT_NAME = MySql::product().info().value(SQL_NAME).toString();
        const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();

        this->ui->tableWidget_cart->item(crow, 0)->setText(PRODUCT_NAME);
        this->ui->tableWidget_cart->item(crow, 0)->setToolTip(PRODUCT_NAME);

        this->ui->tableWidget_cart->item(crow, 1)->setText(QString("%1 руб.").arg(PRICE));
        this->ui->tableWidget_cart->item(crow, 1)->setToolTip(QString("%1 руб.").arg(PRICE));

        this->ui->tableWidget_cart->item(crow, 2)->setText(QLocale(QLocale::Russian).toString(DATE, QLocale::ShortFormat));
        this->ui->tableWidget_cart->item(crow, 2)->setToolTip(QLocale(QLocale::Russian).toString(DATE, QLocale::LongFormat));

        auto pb = new QPushButton("Посмотреть", this->ui->tableWidget_cart);
        this->ui->tableWidget_cart->setCellWidget(crow, 3, pb);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            this->pd->setID(this->id);
            this->pd->setProduct(PRODUCT);
            this->pd->openDialog(true);
        });

        pb = new QPushButton("Убрать", this->ui->tableWidget_cart);
        pb->setProperty("cart", MySql::cart().info().value(SQL_ID).toUInt());
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            QMessageBox::StandardButton sb = QMessageBox::question(this, "Предупреждение", "Вы уверены, что хотите удалить товар из корзины?");
            if (sb == QMessageBox::No) return;

            MySql::cart(qobject_cast<QPushButton *>(QObject::sender())->property("cart").toUInt())._delete();
            QMessageBox::information(this, "Оповещение", "Товар удален");
            if (MySql::cart().wuser(this->id).get().first())
            {
                this->gotoCart();
            }
            else
            {
                this->loadCatalog();
                this->ui->stackedWidget->setCurrentWidget(this->ui->page_catalog);
            }
        });
        this->ui->tableWidget_cart->setCellWidget(crow, 4, pb);

        ++crow;
    }

    this->ui->stackedWidget->setCurrentWidget(this->curPage);
}

void MainWindow::gotoLogin() //вход/выход из аккаунта
{
    this->curPage = this->ui->stackedWidget->currentWidget();
    if (!this->id)
    {
        this->hide();
        this->ld.open();
        return;
    }

    QMessageBox::StandardButton sb = QMessageBox::question(this, "Предупреждение", "Вы уверены, что хотите выйти?");
    if (sb == QMessageBox::No) return;

    this->id = 0;
    this->ui->menu_login->setTitle("Войти");
    this->ui->action_login->setText("Войти в аккаунт");
    if (this->ui->stackedWidget->currentIndex() > 1)
        this->ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::loadCatalog() //загрузить каталог
{
    const QString &SRCH_STR = this->ui->lineEdit_srch->text();
    if (!SRCH_STR.trimmed().isEmpty())
    {
        MySql::product().wcname(SRCH_STR);
    }

    if (this->ui->comboBox_srch_pr->currentIndex())
    {
        unsigned int fprice = 0;
        unsigned int tprice = 0;
        switch (this->ui->comboBox_srch_pr->currentIndex())
        {
        case 1:
        {
            fprice = 25000;
            tprice = 60000;
            break;
        }
        case 2:
        {
            fprice = 60000;
            tprice = 100000;
            break;
        }
        case 3:
        {
            fprice = 100000;
            tprice = 400000;
            break;
        }
        default: break;
        }
        MySql::product().wprice(fprice, MyQuery::EBIGGER).wprice(tprice, MyQuery::ESMALLER);
    }
    if (this->ui->comboBox_srch_mtr->currentIndex())
    {
        MySql::product().wmaterial(this->ui->comboBox_srch_mtr->currentIndex());
    }
    if (this->ui->comboBox_srch_col->currentIndex())
    {
        MySql::product().wcolor(this->ui->comboBox_srch_col->currentIndex());
    }

    const int ROWS = MySql::product().get().size();
    this->ui->tableWidget_catalog->clearContents();
    this->ui->tableWidget_catalog->setRowCount(ROWS);

    unsigned int row = 0;
    while (MySql::product().info().next())
    {
        for (int col = 0; col < CATALOG; ++col)
        {
            if (this->ui->tableWidget_catalog->item(row, col) == nullptr)
            {
                this->ui->tableWidget_catalog->setItem(row, col, new QTableWidgetItem());
            }
        }

        const QString NAME = MySql::product().info().value(SQL_NAME).toString();
        const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();

        const unsigned int PRODUCT = MySql::product().info().value(SQL_ID).toUInt();
        const unsigned int SELLER = MySql::product().info().value(SQL_SELLER).toUInt();
        const unsigned int MATERIAL = MySql::product().info().value(SQL_MATERIAL).toUInt();
        const unsigned int COLOR = MySql::product().info().value(SQL_COLOR).toUInt();

        MySql::seller(SELLER).name().get().first();
        const QString SELLER_NAME = MySql::seller().info().value(SQL_NAME).toString();

        MySql::material(MATERIAL).name().get().first();
        const QString MATERIAL_NAME = MySql::material().info().value(SQL_NAME).toString();

        MySql::color(COLOR).name().get().first();
        const QString COLOR_NAME = MySql::color().info().value(SQL_NAME).toString();

        this->ui->tableWidget_catalog->item(row, 0)->setText(NAME);
        this->ui->tableWidget_catalog->item(row, 0)->setToolTip(NAME);

        this->ui->tableWidget_catalog->item(row, 1)->setText(SELLER_NAME);
        this->ui->tableWidget_catalog->item(row, 1)->setToolTip(SELLER_NAME);

        this->ui->tableWidget_catalog->item(row, 2)->setText(QString("%1 руб.").arg(PRICE));
        this->ui->tableWidget_catalog->item(row, 2)->setToolTip(QString("%1 руб.").arg(PRICE));

        this->ui->tableWidget_catalog->item(row, 3)->setText(MATERIAL_NAME);
        this->ui->tableWidget_catalog->item(row, 3)->setToolTip(MATERIAL_NAME);

        this->ui->tableWidget_catalog->item(row, 4)->setText(COLOR_NAME);
        this->ui->tableWidget_catalog->item(row, 4)->setToolTip(COLOR_NAME);

        auto pb = new QPushButton("Посмотреть", this);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            this->pd->setID(this->id);
            this->pd->setProduct(PRODUCT);
            this->pd->openDialog(true);
        });
        this->ui->tableWidget_catalog->setCellWidget(row, 5, pb);

        pb = new QPushButton("Купить", this);
        pb->setProperty("product", PRODUCT);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
           if (!this->id)
           {
               QMessageBox::warning(this, "Ошибка", "Сначала войдите в свой аккаунт");
               return;
           }
           auto pb = qobject_cast<QPushButton *>(QObject::sender());
           const unsigned int PRODUCT = pb->property("product").toUInt();
           MySql::cart()._insert(this->id, PRODUCT);
           QMessageBox::information(this, "Оповещение", "Товар добавлен в корзину");
        });
        this->ui->tableWidget_catalog->setCellWidget(row, 6, pb);

        ++row;
    }

}

void MainWindow::on_pushButton_cart_clicked() //покупка
{
    MySql::cart().wuser(this->id).product().get();
    unsigned int sum = 0;
    while (MySql::cart().info().next())
    {
        const unsigned int PRODUCT = MySql::cart().info().value(SQL_PRODUCT).toUInt();
        MySql::product(PRODUCT).price().get().first();
        sum += MySql::product().info().value(SQL_PRICE).toUInt();
    }
    QMessageBox::StandardButton sb = QMessageBox::question(this, "Покупка", QString("Общая сумма покупки: %1 руб., продолжить?").arg(sum));
    if (sb == QMessageBox::No) return;

    MySql::cart().wuser(this->id).product().get();
    while (MySql::cart().info().next())
    {
        const unsigned int PRODUCT = MySql::cart().info().value(SQL_PRODUCT).toUInt();
        MySql::history()._insert(this->id, PRODUCT);
    }
    MySql::cart().wuser(this->id)._delete();

    QMessageBox::information(this, "Покупка", "Покупка успешно завершена");

    this->gotoCollection();
}


void MainWindow::on_pushButton_prf_name_clicked() //изменения имени
{
    bool ok;
    QString newName;
    do
    {
        newName = QInputDialog::getText(this, "Новое имя", "Введите новое имя:", QLineEdit::Normal, this->name, &ok);
        if (!ok) return;
        if (newName.trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Вы не ввели новое имя");
            continue;
        }
        if (newName.size() > 128)
        {
            QMessageBox::warning(this, "Ошибка", "Имя слишком длинное");
            continue;
        }
        if (newName == this->name)
        {
            QMessageBox::warning(this, "Ошибка", "Введите новое имя");
            continue;
        }
        const bool SELLER_EXIST = MySql::user().wname(newName).get().first();
        if (SELLER_EXIST)
        {
            QMessageBox::warning(this, "Ошибка", "Такое имя уже занято");
            continue;
        }
        break;
    }
    while (true);

    if (ok)
    {
        this->name = newName;
        this->ui->label_prf_name->setText(this->name);
        this->ui->action_login->setText(QString("Выйти из аккаунта %1").arg(this->name));
        this->ui->menu_login->setTitle(this->name);
        MySql::user(this->id).uname(this->name)._update();
    }
}
