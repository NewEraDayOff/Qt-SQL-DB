#include "sellerdialog.h"
#include "ui_sellerdialog.h"

SellerDialog::SellerDialog(QWidget *parent) : //констурктор
    QDialog(parent),
    ui(new Ui::SellerDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Продавец");
    this->ui->tabWidget->setCurrentIndex(0);

    this->connectCheckBoxWithSlots(this->ui->checkBox_mtrl, this->ui->comboBox_mtrl, this->ui->lineEdit_mtrl);
    this->connectCheckBoxWithSlots(this->ui->checkBox_col, this->ui->comboBox_col, this->ui->lineEdit_col);

    QObject::connect(this->ui->pushButton_clear, &QPushButton::clicked, this, &SellerDialog::clearAddTab);

    this->connectCheckBoxWithSlots(this->ui->checkBox_mtrl_u, this->ui->comboBox_mtrl_u, this->ui->lineEdit_mtrl_u);
    this->connectCheckBoxWithSlots(this->ui->checkBox_col_u, this->ui->comboBox_col, this->ui->lineEdit_col_u);

    QObject::connect(this->ui->pushButton_clear_u, &QPushButton::clicked, this, &SellerDialog::clearUpdTab);


    QObject::connect(this->ui->comboBox_product_d, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     this->ui->pushButton_ok_d, &QWidget::setEnabled);
}

SellerDialog::~SellerDialog() //деструктор
{
    delete this->ui;
}

void SellerDialog::connectCheckBoxWithSlots(QCheckBox *chb, QComboBox *cob, QLineEdit *le) //подключение коннектов к чек боксам
{
    //включение QComboBox при выключении QCheckBox и наоборот
    QObject::connect(chb, &QCheckBox::stateChanged,
                     this, [cob](bool arg){ cob->setEnabled(!arg); });
    //сокрытие QLineEdit при выключении QCheckBox и наоборот
    QObject::connect(chb, &QCheckBox::stateChanged, le, &QWidget::setVisible);
    //очистка QLineEdit при нажатии QCheckBox
    QObject::connect(chb, &QCheckBox::stateChanged, le, &QLineEdit::clear);
}

void SellerDialog::setPropertyToComboBox(MyProperty &prop, QVector<QComboBox *> cbvec) //заполнение комбо-боксов значениями свойств
{
    prop.id().name().get();
    for (auto cb : cbvec)
        cb->clear();
    while (prop.info().next())
    {
        const QString NAME = prop.info().value(SQL_NAME).toString();
        const unsigned int INDEX = prop.info().value(SQL_ID).toUInt();
        for (auto cb : cbvec)
            cb->addItem(NAME, INDEX);
    }
}

void SellerDialog::setProductToComboBox(MyProduct &prod, QVector<QComboBox *> cbvec) //заполнение комбо-боксов значениями товара
{
    prod.id().name().wseller(this->id).get();
    for (auto cb : cbvec)
    {
        cb->clear();
        cb->addItem("Выберите товар", 0);
    }
    while (prod.info().next())
    {
        const QString NAME = prod.info().value(SQL_NAME).toString();
        const unsigned int INDEX = prod.info().value(SQL_ID).toUInt();
        for (auto cb : cbvec)
            cb->addItem(NAME, INDEX);
    }
}

void SellerDialog::clearAddTab() //очистка вкладки добавления товара
{
    this->ui->lineEdit_name->clear();

    this->ui->spinBox_price->setValue(1);

    this->ui->comboBox_mtrl->setCurrentIndex(0);
    this->ui->checkBox_mtrl->setChecked(false);
    this->ui->lineEdit_mtrl->setVisible(false);

    this->ui->comboBox_col->setCurrentIndex(0);
    this->ui->checkBox_col->setChecked(false);
    this->ui->lineEdit_col->setVisible(false);

    this->ui->textEdit_descr->clear();
}

void SellerDialog::loadAddTab() //загрузка вкладки добавления товара
{
    this->setPropertyToComboBox(MySql::material(), {this->ui->comboBox_mtrl});
    this->setPropertyToComboBox(MySql::color(), {this->ui->comboBox_col});
}

void SellerDialog::floadAddTab() //очистка + загрузка вкладки добавления
{ this->clearAddTab(); this->loadAddTab(); }

void SellerDialog::clearUpdTab() //очистка вкладки изменения
{
    this->ui->comboBox_product_u->setCurrentIndex(0);

    this->ui->lineEdit_name_u->clear();

    this->ui->spinBox_price_u->setValue(1);

    this->ui->comboBox_mtrl_u->setCurrentIndex(0);
    this->ui->checkBox_mtrl_u->setChecked(false);
    this->ui->lineEdit_mtrl_u->setVisible(false);

    this->ui->comboBox_col_u->setCurrentIndex(0);
    this->ui->checkBox_col_u->setChecked(false);
    this->ui->lineEdit_col_u->setVisible(false);

    this->ui->textEdit_descr_u->clear();
}

void SellerDialog::loadUpdTab() //загрузка вкладки изменения
{
    this->setProductToComboBox(MySql::product(), {this->ui->comboBox_product_u});
    this->setPropertyToComboBox(MySql::material(), {this->ui->comboBox_mtrl_u});
    this->setPropertyToComboBox(MySql::color(), {this->ui->comboBox_col_u});
}

void SellerDialog::floadUpdTab() //очистка + загрузка вкладки изменения
{ this->clearUpdTab(); this->loadUpdTab(); }

void SellerDialog::loadProfile() //загрузка вкладки профиля
{
    MySql::seller().date().wid(this->id).get().first();
    const QDate DATE = MySql::seller().info().value(SQL_DATE).toDate();
    const QString DATE_STR = QString("Дата регистрации: %1").arg(QLocale(QLocale::Russian).toString(DATE, QLocale::LongFormat));
    this->ui->label_date->setText(DATE_STR);
    MySql::product().id().wseller(this->id).get();
    this->catalog = MySql::product().info().size();
    this->ui->label_amount->setText(QString("В вашем каталоге %1 шт. товара").arg(this->catalog));
    MySql::product().wseller(this->id).get();
    unsigned int rcount = 0;
    unsigned int rsize = 0;
    while (MySql::product().info().next())
    {
        rsize += MySql::review().rating().wproduct(MySql::product().info().value(SQL_ID).toUInt()).get().size();
        while (MySql::review().info().next())
        {
            rcount += MySql::review().info().value(SQL_RATING).toUInt();
        }
    }
    if (rsize > 0)
    {
        this->ui->label_review->setText(QString("Ваша средняя оценка: %1").arg((double)rcount / (double)rsize, 0, 'f', 2));
    }
    else
    {
        this->ui->label_review->setText(QString("Ваша средняя оценка: Вам не ставили не одной оценки"));
    }
}

void SellerDialog::setName(const QString &name) //сеттер имени
{
    this->name = name;
    this->setWindowTitle(QString("Добро пожаловать, %1!").arg(this->name));
    this->ui->label_myname->setText(this->name);
    this->loadProfile();
}

void SellerDialog::on_pushButton_ok_clicked() //добавление товара
{
    const QString NAME = this->ui->lineEdit_name->text();
    if (NAME.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите имя");
        return;
    }

    const bool NAME_EXIST = MySql::product().id().wname(NAME).wseller(this->id).get().first();
    if (NAME_EXIST)
    {
        QMessageBox::warning(this, "Ошибка", "У вас уже есть такой товар");
        return;
    }

    bool materialExist = true;
    unsigned int materialID = 0;
    QString material = this->ui->lineEdit_mtrl->text();
    if (this->ui->checkBox_mtrl->isChecked())
    {
        if (material.trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Введите название материала");
            return;
        }
        materialExist = MySql::material().id().wname(material).get().first();
        if (materialExist) materialID = MySql::material().info().value(SQL_ID).toInt();
    }
    else
    {
        materialID = this->ui->comboBox_mtrl->currentData().toUInt();
        MySql::material(materialID).name().get().first();
        material = MySql::material().info().value(SQL_NAME).toString();
    }

    bool colorExist = true;
    unsigned int colorID = 0;
    QString color = this->ui->lineEdit_col->text();
    if (this->ui->checkBox_col->isChecked())
    {
        if (color.trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Введите цвет");
            return;
        }
        colorExist = MySql::color().id().wname(color).get().first();
        if (colorExist) colorID = MySql::color().info().value(SQL_ID).toInt();
    }
    else
    {
        colorID = this->ui->comboBox_col->currentData().toUInt();
        MySql::color(colorID).name().get().first();
        color = MySql::color().info().value(SQL_NAME).toString();
    }

    const unsigned int PRICE = this->ui->spinBox_price->value();
    const QString DESCRIPTION = !this->ui->textEdit_descr->toPlainText().trimmed().isEmpty()
            ? this->ui->textEdit_descr->toPlainText() : "Нет описания";

    QMessageBox::StandardButton sb = QMessageBox::question(this, "Предупреждение",
                                                           QString("Вы уверены, что хотите добавить данный товар?\n" \
                                                                   "Название: %1\n" \
                                                                   "Цена: %2\n" \
                                                                   "Материал: %3\n" \
                                                                   "Цвет: %4\n" \
                                                                   "Описание: %5\n").arg(NAME, QString::number(PRICE),
                                                                                         material, color, DESCRIPTION));
    if (sb == QMessageBox::No) return;

    if (!materialExist)
    {
        MySql::material()._insert(material).getlast().first();
        materialID = MySql::material().info().value(SQL_ID).toInt();
    }

    if (!colorExist)
    {
        MySql::color()._insert(color).getlast().first();
        colorID = MySql::color().info().value(SQL_ID).toInt();
    }

    MySql::product()._insert(this->id, materialID, colorID, NAME, PRICE, DESCRIPTION);

    if (!materialExist || !colorExist)
        this->floadAddTab();
    else
        this->clearAddTab();
}


void SellerDialog::on_tabWidget_currentChanged(int index) //переключение вкладок
{
    switch (index)
    {
    case 0: this->loadProfile(); break;
    case 1: this->floadAddTab(); break;
    case 2: this->floadUpdTab(); break;
    case 3: this->setProductToComboBox(MySql::product(), {this->ui->comboBox_product_d}); break;
    }
}

void SellerDialog::on_comboBox_product_u_currentIndexChanged(int index) //выбор товара на вкладке изменения
{
    this->ui->lineEdit_name_u->setEnabled(index);
    this->ui->spinBox_price_u->setEnabled(index);
    this->ui->comboBox_mtrl_u->setEnabled(index);
    this->ui->checkBox_mtrl_u->setEnabled(index);
    this->ui->lineEdit_mtrl_u->setEnabled(index);
    this->ui->comboBox_col_u->setEnabled(index);
    this->ui->checkBox_col_u->setEnabled(index);
    this->ui->lineEdit_col_u->setEnabled(index);
    this->ui->textEdit_descr_u->setEnabled(index);
    this->ui->pushButton_ok_u->setEnabled(index);
    this->ui->pushButton_clear_u->setEnabled(index);

    if (index < 1) return;

    const unsigned int PRODUCT = this->ui->comboBox_product_u->currentData().toUInt();
    MySql::product(PRODUCT).get().first();
    const QString NAME = MySql::product().info().value(SQL_NAME).toString();
    const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();
    const unsigned int MATERIAL = MySql::product().info().value(SQL_MATERIAL).toUInt();
    const unsigned int COLOR = MySql::product().info().value(SQL_COLOR).toUInt();
    const QString DESCRIPTION = MySql::product().info().value(SQL_DESCRIPTION).toString();

    this->ui->lineEdit_name_u->setText(NAME);
    this->ui->spinBox_price_u->setValue(PRICE);
    for (int i = 0; i < this->ui->comboBox_mtrl_u->count(); i++)
    {
        if (this->ui->comboBox_mtrl_u->itemData(i).toUInt() == MATERIAL)
        {
            this->ui->comboBox_mtrl_u->setCurrentIndex(i);
            break;
        }
    }
    for (int i = 0; i < this->ui->comboBox_col_u->count(); i++)
    {
        if (this->ui->comboBox_col_u->itemData(i).toUInt() == COLOR)
        {
            this->ui->comboBox_col_u->setCurrentIndex(i);
            break;
        }
    }
    this->ui->textEdit_descr_u->clear();
    this->ui->textEdit_descr_u->append(DESCRIPTION);
}


void SellerDialog::on_pushButton_ok_u_clicked() //изменение товара
{
    const QString NEW_NAME = this->ui->lineEdit_name_u->text();
    if (NEW_NAME.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите новое имя");
        return;
    }

    const unsigned int PRODUCT = this->ui->comboBox_product_u->currentData().toUInt();
    const bool NAME_EXIST = MySql::product().wid(PRODUCT, MyQuery::NEQUAL).wseller(this->id).wname(NEW_NAME).get().first();
    if (NAME_EXIST)
    {
        QMessageBox::warning(this, "Ошибка", "У вас уже есть товар с таким названием");
        return;
    }

    bool materialExist = true;
    unsigned int materialID = 0;
    QString material = this->ui->lineEdit_mtrl_u->text();
    if (this->ui->checkBox_mtrl_u->isChecked())
    {
        if (material.trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Введите название материала");
            return;
        }
        materialExist = MySql::material().id().wname(material).get().first();
        if (materialExist) materialID = MySql::material().info().value(SQL_ID).toInt();
    }
    else
    {
        materialID = this->ui->comboBox_mtrl_u->currentData().toUInt();
        MySql::material(materialID).name().get().first();
        material = MySql::material().info().value(SQL_NAME).toString();
    }

    bool colorExist = true;
    unsigned int colorID = 0;
    QString color = this->ui->lineEdit_col_u->text();
    if (this->ui->checkBox_col_u->isChecked())
    {
        if (color.trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Введите цвет");
            return;
        }
        colorExist = MySql::color().id().wname(color).get().first();
        if (colorExist) colorID = MySql::color().info().value(SQL_ID).toInt();
    }
    else
    {
        colorID = this->ui->comboBox_col_u->currentData().toUInt();
        MySql::color(colorID).name().get().first();
        color = MySql::color().info().value(SQL_NAME).toString();
    }

    MySql::product(PRODUCT).get().first();

    const QString OLD_NAME = MySql::product().info().value(SQL_NAME).toString();
    if (OLD_NAME != NEW_NAME)  MySql::product().uname(NEW_NAME);

    const unsigned int NEW_PRICE = this->ui->spinBox_price_u->value();
    const unsigned int OLD_PRICE = MySql::product().info().value(SQL_PRICE).toUInt();
    if (OLD_PRICE != NEW_PRICE) MySql::product().uprice(NEW_PRICE);

    const unsigned int OLD_MATERIAL = MySql::product().info().value(SQL_MATERIAL).toUInt();
    if (materialExist && OLD_MATERIAL != materialID) MySql::product().umaterial(materialID);
    else if (!materialExist)
    {
        MySql::material()._insert(material).getlast().first();
        materialID = MySql::material().info().value(SQL_ID).toInt();
        MySql::product().umaterial(materialID);
    }

    const unsigned int OLD_COLOR = MySql::product().info().value(SQL_COLOR).toUInt();
    if (colorExist && OLD_COLOR != colorID) MySql::product().ucolor(colorID);
    else if (!colorExist)
    {
        MySql::color()._insert(color).getlast().first();
        colorID = MySql::color().info().value(SQL_ID).toInt();
        MySql::product().ucolor(colorID);
    }

    const QString NEW_DESCRIPTION = this->ui->textEdit_descr_u->toPlainText().trimmed().isEmpty()
            ? "Нет описания" : this->ui->textEdit_descr_u->toPlainText();
    const QString OLD_DESCRIPTION = MySql::product().info().value(SQL_DESCRIPTION).toString();
    if (OLD_DESCRIPTION != NEW_DESCRIPTION) MySql::product().udescription(NEW_DESCRIPTION);

    MySql::product(PRODUCT)._update();

    if (!materialExist || !colorExist)
        this->floadUpdTab();
    else
        this->clearUpdTab();
}


void SellerDialog::on_pushButton_ok_d_clicked() //удаление товара
{
    const unsigned int PRODUCT = this->ui->comboBox_product_d->currentData().toUInt();
    const QString PRODUCT_NAME = this->ui->comboBox_product_d->currentText();

    QMessageBox::StandardButton sb = QMessageBox::question(this, "Предупреждение",
                                                           QString("Вы уверены, что хотите удалить " \
                                                                   "\"%1\" из каталога?").arg(PRODUCT_NAME));
    if (sb == QMessageBox::No) return;

    MySql::product(PRODUCT)._delete();
    this->setProductToComboBox(MySql::product(), {this->ui->comboBox_product_d});
}

void SellerDialog::openCatalog(unsigned int seller, bool meUser, unsigned int user) //открыть каталог
{
    if (this->catalog < 1 && seller == this->id)
    {
        QMessageBox::warning(this, "Ошибка", "Ваш каталог пуст!");
        return;
    }
    if (this->cd == nullptr)
    {
        this->cd = new CatalogDialog(this);
    }

    if (meUser && user > 0)
    {
        this->cd->setUser(user);
    }
    this->cd->setSeller(seller);
    this->cd->setIsUser(meUser);
    this->cd->open();
}


void SellerDialog::on_pushButton_name_clicked() //изменение имени
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
        const bool SELLER_EXIST = MySql::seller().wname(newName).get().first();
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
        this->setWindowTitle(QString("Добро пожаловать, %1!").arg(this->name));
        this->ui->label_myname->setText(this->name);
        MySql::seller(this->id).uname(this->name)._update();
    }
}


void SellerDialog::on_pushButton_exit_clicked() //выход из аккаунта
{
    QMessageBox::StandardButton sb = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти из аккаунта?");
    if (sb == QMessageBox::No) return;

    this->id = 0;
    this->catalog = 0;
    this->name = QString();

    this->accept();
}


void SellerDialog::on_pushButton_catalog_clicked() //каталог
{
    this->openCatalog(this->id, false);
}

