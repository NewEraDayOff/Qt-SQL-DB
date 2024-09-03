#include "mysql.h"

//объявление статического поля
MySql *MyQuery::database = nullptr;

void MyQuery::initialize(MySql *db) //присвоение бд к запросам
{ database = db; }

void MyUser::decl(MySql *db) //объявление объектов класса MyUser
{
    if (db != nullptr)
        MyQuery::initialize(db);
    if (MySql::myuser == nullptr)
        MySql::myuser = new MyUser("user");
    if (MySql::myseller == nullptr)
        MySql::myseller = new MyUser("seller");
}

void MyProduct::decl(MySql *db) //объявление объектов класса MyProduct
{
    if (db != nullptr)
        MyQuery::initialize(db);
    if (MySql::myproduct == nullptr)
        MySql::myproduct = new MyProduct("product");
}

void MyProperty::decl(MySql *db) //объявление объектов класса MyProperty
{
    if (db != nullptr)
        MyQuery::initialize(db);
    if (MySql::mymaterial == nullptr)
        MySql::mymaterial = new MyProperty("material");
    if (MySql::mycolor == nullptr)
        MySql::mycolor = new MyProperty("color");
}

void MyCollection::decl(MySql *db) //объявление объектов класса MyCollection
{
    if (db != nullptr)
        MyQuery::initialize(db);
    if (MySql::mycart == nullptr)
        MySql::mycart = new MyCollection("cart_elements");
    if (MySql::myhistory == nullptr)
        MySql::myhistory = new MyCollection("collection_elements");
}

void MyReview::decl(MySql *db) //объявление объектов класса MyReview
{
    if (db != nullptr)
        MyQuery::initialize(db);
    if (MySql::myreview == nullptr)
        MySql::myreview = new MyReview("review");
}

void MyQuery::_insert(const Table &table, const QList<QVariant> &var) //вставка новых элементов
{
    QString request = QUERY_INSERT;
    this->q = QSqlQuery(*MyQuery::database);
    QStringList lcol;
    switch (table)
    {
    case USER: Q_FALLTHROUGH();
    case SELLER: lcol = QStringList{SQL_EMAIL, SQL_PASSWORD, SQL_NAME, SQL_DATE}; break;
    case PRODUCT: lcol = QStringList{SQL_SELLER, SQL_MATERIAL, SQL_COLOR, SQL_NAME, SQL_PRICE, SQL_DESCRIPTION, SQL_DATE}; break;
    case PROPERTY: lcol = QStringList{SQL_NAME, SQL_DATE}; break;
    case COLLECTION: lcol = QStringList{SQL_USER, SQL_PRODUCT, SQL_DATE}; break;
    case REVIEW: lcol = QStringList{SQL_PRODUCT, SQL_USER, SQL_RATING, SQL_CONTENT, SQL_DATE}; break;
    }

    QStringList tobind;
    for (int i = 0; i < var.size(); i++)
        tobind.append(QString(":%1").arg(i));
    tobind.append("CURRENT_DATE");

    q.prepare(request.arg(lcol.join(", "), tobind.join(", ")));
    for (int i = 0; i < var.size(); i++)
        q.bindValue(QString(":%1").arg(i), var[i]);
    q.exec();
}
 //вставка новых элементов в MyUser
void MyUser::_insert(const QString &login, const QString &password, const QString &name, const QString &date)
{
    const QString TNAME = name.isEmpty() ? login : name;

    QList<QVariant> var = {login, password, TNAME};
    if (!date.isEmpty()) var.append(date);

    MyQuery::_insert(USER, var);
}
 //вставка новых элементов в MyProduct
void MyProduct::_insert(unsigned int seller, unsigned int material, unsigned int color, const QString &name, unsigned int price, const QString &description, const QString &date)
{
    QList<QVariant> var = {seller, material, color, name, price};
    description.isEmpty() ? var.append("Нет описания") : var.append(description);
    if (!date.isEmpty()) var.append(date);

    MyQuery::_insert(PRODUCT, var);
}
 //вставка новых элементов в MyCollection
MyCollection &MyCollection::_insert(unsigned int user, unsigned int product, const QString &date)
{
    QList<QVariant> var = {user, product};
    if (!date.isEmpty()) var.append(date);

    MyQuery::_insert(COLLECTION, var);
    return *this;
}
 //вставка новых элементов в MyReview
MyReview &MyReview::_insert(unsigned int user, unsigned int product, unsigned int rating, const QString &content, const QString &date)
{
    QList<QVariant> var = {product, user, rating, content};
    if (!date.isEmpty()) var.append(date);

    MyQuery::_insert(REVIEW, var);
    return *this;
}
 //вставка новых элементов в MyUser
MyProperty &MyProperty::_insert(const QString &name, const QString &date)
{
    QList<QVariant> var = {name};
    if (!date.isEmpty()) var.append(date);

    MyQuery::_insert(PROPERTY, var);
    return *this;
}

void MyQuery::_delete() //удаление элементов
{
    if (whereDataset.isEmpty())
        return;

    this->q = QSqlQuery(*MyQuery::database);

    QStringList whereL;
    unsigned int index = 0;
    foreach (auto val, this->whereDataset)
    {
        whereL.append(QString("%1 %2 :w%3").arg(val.col, val.sign, QString::number(index)));
        ++index;
    }

    this->q.prepare(QString("%1;").arg(this->QUERY_DELETE.arg(whereL.join(" AND "))));

    index = 0;
    foreach (auto val, this->whereDataset)
    {
        this->q.bindValue(QString(":w%1").arg(index), val.data);
        ++index;
    }

    this->q.exec();

    this->whereDataset.clear();
}

void MyQuery::_update() //изменение элементов
{
    if (this->updateDataset.isEmpty() || this->whereDataset.isEmpty())
        return;

    this->q = QSqlQuery(*MyQuery::database);

    QStringList updateL;
    unsigned int index = 0;
    foreach (auto val, this->updateDataset)
    {
        updateL.append(QString("%1 = :u%2").arg(val.col, QString::number(index)));
        ++index;
    }

    QStringList whereL;
    index = 0;
    foreach (auto val, this->whereDataset)
    {
        whereL.append(QString("%1 %2 :w%3").arg(val.col, val.sign, QString::number(index)));
        ++index;
    }

    this->q.prepare(QString("%1;").arg(this->QUERY_UPDATE.arg(updateL.join(", "), whereL.join(" AND "))));

    index = 0;
    foreach (auto val, this->updateDataset)
    {
        this->q.bindValue(QString(":u%1").arg(index), val.data);
        ++index;
    }

    index = 0;
    foreach (auto val, this->whereDataset)
    {
        this->q.bindValue(QString(":w%1").arg(index), val.data);
        ++index;
    }

    this->q.exec();

    this->updateDataset.clear();
    this->whereDataset.clear();
}

QSqlQuery &MyQuery::get() //запрос элементов
{
    QString request = QUERY_SELECT;
    this->q = QSqlQuery(*MyQuery::database);
    if (!this->whereDataset.isEmpty())
    {
        request.append(" WHERE ");
        QStringList whereL;
        unsigned int index = 0;
        foreach (auto val, this->whereDataset)
        {
            whereL.append(QString("%1 %2 :%3").arg(val.col, val.sign, QString::number(index)));
            ++index;
        }
        request.append(whereL.join(" AND "));
    }
    QString selectL = QStringList(this->selectDataset.isEmpty() ? QStringList("*") : this->selectDataset.toList()).join(", ");
    this->q.prepare(QString("%1;").arg(request.arg(selectL)));
    if (!this->whereDataset.isEmpty())
    {
        QStringList whereL;
        unsigned int index = 0;
        foreach (auto val, this->whereDataset)
        {
            this->q.bindValue(QString(":%1").arg(QString::number(index)), val.data);
            ++index;
        }
    }
    this->q.exec();

    this->whereDataset.clear();
    this->selectDataset.clear();

    return this->q;
}

QSqlQuery &MyQuery::getlast() //запрос последнего элемента
{
    QString request = this->QUERY_SELECT;
    this->q = QSqlQuery(*MyQuery::database);
    QString selectL = QStringList(this->selectDataset.isEmpty() ? QStringList("*") : this->selectDataset.toList()).join(", ");

    this->q.prepare(QString("%1 ORDER BY id DESC LIMIT 1;").arg(request.arg(selectL)));
    this->q.exec();

    this->selectDataset.clear();
    return this->q;
}

//объявление статических полей
MySql *MySql::database = nullptr;
MySqlDestroyer MySql::destroyer;
MyUser *MySql::myuser = nullptr;
MyUser *MySql::myseller = nullptr;
MyProduct *MySql::myproduct = nullptr;
MyProperty *MySql::mymaterial = nullptr;
MyProperty *MySql::mycolor = nullptr;
MyCollection *MySql::mycart = nullptr;
MyCollection *MySql::myhistory = nullptr;
MyReview *MySql::myreview = nullptr;

MySqlDestroyer::~MySqlDestroyer() //деструктор бд
{ delete database; }

void MySqlDestroyer::initialize(MySql *db) //присвоение бд классу деструктору
{ database = db; }

MySql &MySql::db() //инициализация бд
{
    if (database == nullptr)
    {
        database = new MySql(QSqlDatabase::addDatabase("QPSQL"));
        database->setDatabaseName("Wedding Shop");
        database->setHostName("127.0.0.1");
        database->setUserName("postgres");
        database->setPassword("1234");
        database->setPort(5432);
        database->open();
        MyQuery::initialize(database);
        MyUser::decl(database);
        MyProduct::decl(database);
        MyProperty::decl(database);
        MyCollection::decl(database);
        MyReview::decl(database);
        destroyer.initialize(database);
    }
    return *database;
}
