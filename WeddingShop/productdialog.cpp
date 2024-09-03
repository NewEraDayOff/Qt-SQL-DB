#include "productdialog.h"
#include "ui_productdialog.h"

ProductDialog::ProductDialog(QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::ProductDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Товар");
}

ProductDialog::~ProductDialog() //деструктор
{
    delete this->ui;
}

void ProductDialog::openDialog(const bool isUser) //открытие диалога
{
    this->ui->pushButton_buy->setVisible(isUser);
    this->ui->pushButton_rate->setVisible(isUser);
    this->isUser = isUser;

    MySql::product(this->product).get().first();
    const QString PRODUCT_NAME = MySql::product().info().value(SQL_NAME).toString();
    const QString DESCRIPTION = MySql::product().info().value(SQL_DESCRIPTION).toString();
    const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();
    this->seller = isUser ? MySql::product().info().value(SQL_SELLER).toUInt() : this->id;
    MySql::seller(this->seller).name().get().first();
    const QString SELLER_NAME = MySql::seller().info().value(SQL_NAME).toString();
    const unsigned int AMOUNT = MySql::review().wproduct(this->product).get().size();
    double rating = 0;
    while (MySql::review().info().next())
    {
        rating += MySql::review().info().value(SQL_RATING).toDouble();
    }

    this->ui->label_name->setText(QString("Название: %1").arg(PRODUCT_NAME));
    this->ui->label_seller->setText(QString("Продавец: %1").arg(SELLER_NAME));
    this->ui->label_price->setText(QString("Цена: %1 руб.").arg(PRICE));
    this->ui->label_rating->setText(AMOUNT > 0 ? QString("Рейтинг: %1/10").arg(rating / AMOUNT, 0, 'f', 2) : "Рейтинг: нет отзывов");

    this->ui->textEdit->clear();
    this->ui->textEdit->append(DESCRIPTION);

    this->updateReviewList();

    this->open();
}

void ProductDialog::updateReviewList() //обновление списка отзывов
{
    this->ui->listWidget->clear();
    MySql::review().wproduct(this->product).user().rating().get();
    while (MySql::review().info().next())
    {
        const unsigned int USER = MySql::review().info().value(SQL_USER).toUInt();
        const unsigned int RATING = MySql::review().info().value(SQL_RATING).toUInt();
        MySql::user(USER).name().get().first();
        const QString USER_NAME = MySql::user().info().value(SQL_NAME).toString();
        auto item = new QListWidgetItem(QString("%1\t%2/10").arg(USER_NAME, QString::number(RATING)));
        item->setData(Qt::UserRole, USER);
        this->ui->listWidget->addItem(item);
    }
}


void ProductDialog::on_pushButton_buy_clicked() //покупка
{
    if (!this->id)
    {
        QMessageBox::warning(this, "Ошибка", "Сначала войдите в свой аккаунт");
        return;
    }
    MySql::cart()._insert(this->id, this->product);
    QMessageBox::information(this, "Оповещение", "Товар добавлен в корзину");
}


void ProductDialog::on_pushButton_rate_clicked() //оценить товар
{
    if (!this->id)
    {
        QMessageBox::warning(this, "Ошибка", "Сначала войдите в свой аккаунт");
        return;
    }

    const bool PRODUCT_PURCHASED = MySql::history().wuser(this->id).wproduct(this->product).get().first();
    if (!PRODUCT_PURCHASED)
    {
        QMessageBox::warning(this, "Товар не приобретен", "Чтобы оставить отзыв приобретите товар");
        return;
    }

    if (this->rd == nullptr)
    {
        this->rd = new ReviewDialog(this);
    }

    this->rd->setID(this->id);
    this->rd->setProduct(this->product);
    this->rd->setUser(true);
    this->rd->open();
}


void ProductDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item) //открытие отзыва
{
    const unsigned int USER = item->data(Qt::UserRole).toUInt();

    if (this->rd == nullptr)
    {
        this->rd = new ReviewDialog(this);
        QObject::connect(this->rd, &QDialog::accepted, this, &ProductDialog::updateReviewList);
    }
    this->rd->setID(USER);
    this->rd->setProduct(this->product);
    this->rd->setUser(USER == this->id && this->isUser);
    this->rd->open();
}

