#ifndef MYSQL_H
#define MYSQL_H

#include <QDebug>
#include <QDate>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#define SQL_ALL "*"
#define SQL_ID "id"
#define SQL_NAME "name"
#define SQL_EMAIL "email"
#define SQL_PASSWORD "password"
#define SQL_PRICE "price"
#define SQL_SELLER "seller_id"
#define SQL_MATERIAL "material_id"
#define SQL_COLOR "color_id"
#define SQL_USER "user_id"
#define SQL_PRODUCT "product_id"
#define SQL_DESCRIPTION "description"
#define SQL_DATE "date"
#define SQL_RATING "rating"
#define SQL_CONTENT "content"


class MySql;

class MyQuery
{
public:
    enum Table { USER, SELLER, PRODUCT, PROPERTY, COLLECTION, REVIEW };
    enum Sign { EQUAL, NEQUAL, BIGGER, EBIGGER, SMALLER, ESMALLER, LIKE };

    struct whereset //набор данных для условия
    {
        whereset() {} //конструктор
        whereset(const QString &col, const QVariant &data, const Sign &sign)
            : col(col), data(data) //конструктор
        {
            switch (sign)
            {
            case EQUAL: this->sign = "="; break;
            case NEQUAL: this->sign = "!="; break;
            case BIGGER: this->sign = ">"; break;
            case EBIGGER: this->sign = ">="; break;
            case SMALLER: this->sign = "<"; break;
            case ESMALLER: this->sign = "<="; break;
            case LIKE: this->sign = "LIKE"; break;
            }
        }
        QString col = QString();
        QString sign = QString();
        QVariant data = QVariant();
    };

    struct updateset //набор данных для обновления информации
    {
        updateset() {} //конструктор
        updateset(const QString &col, const QVariant &data) : col(col), data(data) {} //конструктор
        QString col = QString();
        QVariant data = QVariant();
    };

private:
    MyQuery() {}
    MyQuery(const MyQuery &);
    MyQuery &operator=(MyQuery &);

    const QString QUERY_SELECT;
    const QString QUERY_INSERT;
    const QString QUERY_UPDATE;
    const QString QUERY_DELETE;

    QSet<QString> selectDataset;
    QVector<whereset> whereDataset;
    QVector<updateset> updateDataset;

    static MySql *database;
    QSqlQuery q;

protected:
    ~MyQuery() { q.clear(); } //деструктор
    explicit MyQuery(const QString &table) //конструктор с инициализацией констант с конкретно заданной таблицей
        : QUERY_SELECT(QString("SELECT %2 FROM public.%1").arg(table)),
          QUERY_INSERT(QString("INSERT INTO public.%1 (%2) VALUES (%3);").arg(table)),
          QUERY_UPDATE(QString("UPDATE public.%1 SET %2 WHERE %3").arg(table)),
          QUERY_DELETE(QString("DELETE FROM public.%1 WHERE %2").arg(table)) {}

    inline void addWhere(const QString &col, const QVariant &data, const Sign &sign)
    { this->whereDataset.append(whereset(col, data, sign)); } //добавление условия и знака условия *where*
    inline void AddUpdate(const QString &col, const QVariant &data)
    { this->updateDataset.append(updateset(col, data)); } //добавление параметра обновления информации *update*
    void AddSelect(const QString &col) //добавление параметра выбора *select*
    {
        if (this->selectDataset.contains("*")) return;//если собирается вся информация
        if (col == "*" && !this->selectDataset.isEmpty()) this->selectDataset.clear(); //если добавляется выбор всех кортежей, то стираем выборочные
        this->selectDataset.insert(col);
    }

    /*! \brief SELECT all FROM ...; */
    void all() { this->AddSelect("*"); } //запрос всех кортежей
    /*! \brief SELECT id FROM ...; */
    void id() { this->AddSelect("id"); } //запрос кортежа с индексом
    /*! \brief SELECT date FROM ...; */
    void date() { this->AddSelect("date"); } //запрос кортежа с датой

    /*! \brief SELECT ... FROM ... WHERE id sign(=) 'id'; */
    void wid(unsigned int id, const Sign &sign = EQUAL) //условие, где индекс сравнивается с заданным значением
    {
        if (!id) return;
        this->addWhere(SQL_ID, id, sign);
    }
    /*! \brief SELECT ... FROM ... WHERE date sign(=) 'date'; */
    void wdate(const QDate &date, const Sign &sign = EQUAL) //условие, где дата сравнивается с заданным значением
    {
        if (!date.isValid()) return;
        this->addWhere(SQL_DATE, date, sign);
    }

    void _insert(const Table &table, const QList<QVariant> &var);


public:
    QSqlQuery &get();
    QSqlQuery &getlast();
    QSqlQuery &info() { return q; } //геттер запроса
    void _update();
    void _delete();

    static void initialize(MySql *);
};

class MyUser : public MyQuery
{
private:
    MyUser();
    MyUser(const QString &name) : MyQuery(name) {} //конструктор
    MyUser(const MyUser &);
    MyUser &operator=(MyUser &);

public:
    /*! \brief SELECT all FROM user; */
    MyUser &all() { MyQuery::all(); return *this; } //запрос всех кортежей
    /*! \brief SELECT id FROM user; */
    MyUser &id() { MyQuery::id(); return *this; } //запрос индекса
    /*! \brief SELECT login FROM user; */
    MyUser &login() { MyQuery::AddSelect(SQL_EMAIL); return *this; } //запрос почты/логина
    /*! \brief SELECT password FROM user; */
    MyUser &password() { MyQuery::AddSelect(SQL_PASSWORD); return *this; } //запрос пароля
    /*! \brief SELECT name FROM user; */
    MyUser &name() { MyQuery::AddSelect(SQL_NAME); return *this; } //запрос имени
    /*! \brief SELECT date FROM user; */
    MyUser &date() { MyQuery::date(); return *this; } //запрос даты

    /*! \brief SELECT ... FROM user WHERE id sign(=) 'id'; */
    MyUser &wid(unsigned int id, const Sign &sign = EQUAL) { MyQuery::wid(id, sign); return *this; } //условие, где индекс сравнивается с заданным значением
    /*! \brief SELECT ... FROM user WHERE login sign(=) 'login'; */
    MyUser &wlogin(const QString &login) //условие, где почта/логин сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_EMAIL, login, EQUAL);
        return *this;
    }
    /*! \brief SELECT ... FROM user WHERE password = 'password'; */
    MyUser &wpassword(const QString &password) //условие, где пароль сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_PASSWORD, password, EQUAL);
        return *this;
    }
    /*! \brief SELECT ... FROM user WHERE name = 'name'; */
    MyUser &wname(const QString &name) //условие, где имя сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_NAME, name, EQUAL);
        return *this;
    }
    /*! \brief UPDATE user SET name = 'name' WHERE ... */
    MyUser &uname(const QString &name) //изменение имени
    {
        MyQuery::AddUpdate(SQL_NAME, name);
        return *this;
    }

    void _insert(const QString &login, const QString &password, const QString &name = QString(), const QString &date = QString());
    static void decl(MySql *);
};

class MyProduct : public MyQuery
{
    enum { MAX_MATERIAL = 7, MAX_COLOR = 6 };
private:
    MyProduct();
    MyProduct(const QString &name) : MyQuery(name) {} //конструктор
    MyProduct(const MyProduct &);
    MyProduct &operator=(MyProduct &);

public:
    /*! \brief SELECT all FROM product; */
    MyProduct &all() { MyQuery::all(); return *this; } //запрос всех кортежей
    /*! \brief SELECT id FROM product; */
    MyProduct &id() { MyQuery::id(); return *this; } //запрос индекса
    /*! \brief SELECT name FROM product; */
    MyProduct &name() { MyQuery::AddSelect(SQL_NAME); return *this; } //запрос имени
    /*! \brief SELECT price FROM product; */
    MyProduct &price() { MyQuery::AddSelect(SQL_PRICE); return *this; } //запрос цены
    /*! \brief SELECT seller_id FROM product; */
    MyProduct &seller() { MyQuery::AddSelect(SQL_SELLER); return *this; } //запрос индекса продавца

    /*! \brief SELECT ... FROM product WHERE id sign(=) 'id'; */
    MyProduct &wid(unsigned int id, const Sign &sign = EQUAL) { MyQuery::wid(id, sign); return *this; } //условие, где индекс сравнивается с заданным значением
    /*! \brief SELECT ... FROM product WHERE seller_id sign(=) 'seller'; */
    MyProduct &wseller(unsigned int seller, const Sign &sign = EQUAL) //условие, где индекс продавца сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_SELLER, seller, sign);
        return *this;
    }
    /*! \brief SELECT ... FROM product WHERE name = 'name'; */
    MyProduct &wname(const QString &name) //условие, где имя сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_NAME, name, EQUAL);
        return *this;
    }
    /*! \brief SELECT ... FROM product WHERE price sign(=) 'price'; */
    MyProduct &wprice(unsigned int price, const Sign &sign = EQUAL) //условие, где цена сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_PRICE, price, sign);
        return *this;
    }
    /*! \brief SELECT ... FROM product WHERE material_id sign(=) 'material'; */
    MyProduct &wmaterial(unsigned int material, const Sign &sign = EQUAL) //условие, где индекс материала сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_MATERIAL, material, material == MAX_MATERIAL ? EBIGGER : sign);
        return *this;
    }
    /*! \brief SELECT ... FROM product WHERE color_id sign(=) 'color'; */
    MyProduct &wcolor(unsigned int color, const Sign &sign = EQUAL) //условие, где индекс цвета сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_COLOR, color, color == MAX_COLOR ? EBIGGER : sign);
        return *this;
    }

    /*! \brief SELECT ... FROM product WHERE name LIKE '%name%'; */
    MyProduct &wcname(const QString &name) //условие, что имя будет содержать заданное значение
    {
        const QString NAME = QString("%%1%").arg(name);
        MyQuery::addWhere(SQL_NAME, NAME, LIKE);
        return *this;
    }
    /*! \brief UPDATE product SET name = 'name' WHERE ... */
    MyProduct &uname(const QString &name) //изменение имени
    {
        MyQuery::AddUpdate(SQL_NAME, name);
        return *this;
    }
    /*! \brief UPDATE product SET price = 'price' WHERE ... */
    MyProduct &uprice(unsigned int price) //изменение цены
    {
        MyQuery::AddUpdate(SQL_PRICE, price);
        return *this;
    }
    /*! \brief UPDATE product SET material_id = 'material' WHERE ... */
    MyProduct &umaterial(unsigned int material) //изменение индекса материала
    {
        MyQuery::AddUpdate(SQL_MATERIAL, material);
        return *this;
    }
    /*! \brief UPDATE product SET name = 'name' WHERE ... */
    MyProduct &ucolor(unsigned int color) //изменение индекса цвета
    {
        MyQuery::AddUpdate(SQL_COLOR, color);
        return *this;
    }
    /*! \brief UPDATE product SET description = 'description' WHERE ... */
    MyProduct &udescription(const QString &description) //изменение описания
    {
        MyQuery::AddUpdate(SQL_DESCRIPTION, description);
        return *this;
    }

    void _insert(unsigned int seller, unsigned int material, unsigned int color, const QString &name, unsigned int price, const QString &description = QString(), const QString &date = QString());
    static void decl(MySql *);
};

class MyProperty : public MyQuery
{
private:
    MyProperty();
    MyProperty(const QString &name) : MyQuery(name) {} //конструктор
    MyProperty(const MyProperty &);
    MyProperty &operator=(MyProperty &);

public:
    /*! \brief SELECT id FROM ...; */
    MyProperty &id() { MyQuery::id(); return *this; } //запрос на индекс
    /*! \brief SELECT name FROM ...; */
    MyProperty &name() { MyQuery::AddSelect(SQL_NAME); return *this; } //запрос на имя

    /*! \brief SELECT ... FROM ... WHERE id sign(=) 'id'; */
    MyProperty &wid(unsigned int id, const Sign &sign = EQUAL) { MyQuery::wid(id, sign); return *this; } //условие, где индекс сравнивается с заданным значением
    /*! \brief SELECT ... FROM ... WHERE name = 'name'; */
    MyProperty &wname(const QString &name) //условие, где имя сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_NAME, name, EQUAL);
        return *this;
    }

    MyProperty &_insert(const QString &name, const QString &date = QString());
    static void decl(MySql *);
};

class MyCollection : public MyQuery
{
private:
    MyCollection();
    MyCollection(const QString &name) : MyQuery(name) {}
    MyCollection(const MyCollection &);
    MyCollection &operator=(MyCollection &);

public:
    /*! \brief SELECT id FROM ...; */
    MyCollection &id() { MyQuery::id(); return *this; } //запрос на индекс
    /*! \brief SELECT product_id FROM ...; */
    MyCollection &product() { MyQuery::AddSelect(SQL_PRODUCT); return *this; } //запрос на индекс продукта

    /*! \brief SELECT ... FROM ... WHERE id sign(=) 'id'; */
    MyCollection &wid(unsigned int id, const Sign &sign = EQUAL) { MyQuery::wid(id, sign); return *this; } //условие, где индекс сравнивается с заданным значением
    /*! \brief SELECT ... FROM ... WHERE user_id sign(=) 'user'; */
    MyCollection &wuser(unsigned int user, const Sign &sign = EQUAL) //условие, где индекс пользователя сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_USER, user, sign);
        return *this;
    }
    /*! \brief SELECT ... FROM ... WHERE id sign(=) 'product'; */
    MyCollection &wproduct(unsigned int product, const Sign &sign = EQUAL) //условие, где индекс товара сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_PRODUCT, product, sign);
        return *this;
    }

    MyCollection &_insert(unsigned int user, unsigned int product, const QString &date = QString());
    static void decl(MySql *);
};

class MyReview : public MyQuery
{
private:
    MyReview();
    MyReview(const QString &name) : MyQuery(name) {} //конструктор
    MyReview(const MyReview &);
    MyReview &operator=(MyReview &);

public:
    /*! \brief SELECT id FROM review */
    MyReview &id() { MyQuery::id(); return *this; } //запрос на индекс
    /*! \brief SELECT product_id FROM review; */
    MyReview &product() { MyQuery::AddSelect(SQL_PRODUCT); return *this; } //запрос на индекс товара
    /*! \brief SELECT user_id FROM review; */
    MyReview &user() { MyQuery::AddSelect(SQL_USER); return *this; } //запрос на индекс пользователя
    /*! \brief SELECT rating FROM review */
    MyReview &rating() { MyQuery::AddSelect(SQL_RATING); return *this; } //запрос на оценку
    /*! \brief SELECT content FROM review; */
    MyReview &content() { MyQuery::AddSelect(SQL_CONTENT); return *this; } //запрос на отзыв

    /*! \brief SELECT ... FROM review WHERE id sign(=) 'id'; */
    MyReview &wid(unsigned int id, const Sign &sign = EQUAL) { MyQuery::wid(id, sign); return *this; } //условие, где индекс сравнивается с заданным значением
    /*! \brief SELECT ... FROM review WHERE user_id sign(=) 'user'; */
    MyReview &wuser(unsigned int user, const Sign &sign = EQUAL) //условие, где индекс пользователя сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_USER, user, sign);
        return *this;
    }
    /*! \brief SELECT ... FROM review WHERE product_id sign(=) 'product'; */
    MyReview &wproduct(unsigned int product, const Sign &sign = EQUAL) //условие, где индекс товара сравнивается с заданным значением
    {
        MyQuery::addWhere(SQL_PRODUCT, product, sign);
        return *this;
    }
    /*! \brief UPDATE review SET content = 'content' WHERE ... */
    MyReview &ucontent(const QString &content) //изменение отзыва
    {
        MyQuery::AddUpdate(SQL_CONTENT, content);
        return *this;
    }
    /*! \brief UPDATE review SET rating = 'rating' WHERE ... */
    MyReview &urating(unsigned int rating) //изменение оценки
    {
        MyQuery::AddUpdate(SQL_RATING, rating);
        return *this;
    }

    MyReview &_insert(unsigned int user, unsigned int product, unsigned int rating, const QString &content = QString(), const QString &date = QString());
    static void decl(MySql *);
};

class MySqlDestroyer
{
private:
    MySql *database = nullptr;

public:
    ~MySqlDestroyer();
    void initialize(MySql *);
};

class MySql : public QSqlDatabase
{
private:
    MySql() {} //конструктор
    MySql(const MySql &);
    MySql(const QSqlDatabase &db) : QSqlDatabase(db) {} //конструктор
    MySql &operator=(MySql &);
    ~MySql() //деструктор
    {
        if (myuser != nullptr) delete myuser;
        if (myseller != nullptr) delete myseller;
        if (myproduct != nullptr) delete myproduct;
        if (mymaterial != nullptr) delete mymaterial;
        if (mycolor != nullptr) delete mycolor;
        if (mycart != nullptr) delete mycart;
        if (myhistory != nullptr) delete myhistory;
        if (myreview != nullptr) delete  myreview;
    }

    static MySql *database;
    static MySqlDestroyer destroyer;
    static MyUser *myuser;
    static MyUser *myseller;
    static MyProduct *myproduct;
    static MyProperty *mymaterial;
    static MyProperty *mycolor;
    static MyCollection *mycart;
    static MyCollection *myhistory;
    static MyReview *myreview;

    friend class MySqlDestroyer;
    friend void MyUser::decl(MySql *);
    friend void MyProduct::decl(MySql *);
    friend void MyProperty::decl(MySql *);
    friend void MyCollection::decl(MySql *);
    friend void MyReview::decl(MySql *);

public:
    static MySql &db();
    static MyUser &user(unsigned int id = 0) { return id ? myuser->wid(id) : *myuser; } //геттер запросов пользователя
    static MyUser &seller(unsigned int id = 0) { return id ? myseller->wid(id) : *myseller; } //геттер запросов продавца
    static MyProduct &product(unsigned int id = 0) { return id ? myproduct->wid(id) : *myproduct; } //геттер запросов товара
    static MyProperty &material(unsigned int id = 0) { return id ? mymaterial->wid(id) : *mymaterial; } //геттер запросов материала
    static MyProperty &color(unsigned int id = 0) { return id ? mycolor->wid(id) : *mycolor; } //геттер запросов цвета
    static MyCollection &cart(unsigned int id = 0) { return id ? mycart->wid(id) : *mycart; } //геттер запросов корзины
    static MyCollection &history(unsigned int id = 0) { return id ? myhistory->wid(id) : *myhistory; } //геттер запросов истории
    static MyReview &review(unsigned int id = 0) { return id ? myreview->wid(id) : *myreview; } //геттер запросов отзывов
};

#endif // MYSQL_H
