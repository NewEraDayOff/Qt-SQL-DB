#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Вход");
    this->setFixedSize(this->size());
    this->ui->lineEdit_email->setFocus();

    QObject::connect(this->ui->pushButton_cancel, &QPushButton::clicked, this, &QWidget::close);

    QObject::connect(&this->rd, &QDialog::finished, this, &LoginDialog::open);
    QObject::connect(&this->rd, &RegDialog::sendLogin, this->ui->lineEdit_email, &QLineEdit::setText);
}

LoginDialog::~LoginDialog() //деструктор
{
    delete this->ui;
}

void LoginDialog::open() //переопределение открытия диалога
{
    this->ui->lineEdit_email->setFocus();
    this->ui->label_err_email->clear();
    this->ui->label_err_pwrd->clear();
    this->ui->lineEdit_pwrd->clear();

    QDialog::open();
}

void LoginDialog::on_pushButton_login_clicked() //вход
{
    this->ui->label_err_email->clear();
    this->ui->label_err_pwrd->clear();

    const QString LOGIN = this->ui->lineEdit_email->text();
    bool nologin = false;
    if (LOGIN.trimmed().isEmpty())
    {
        this->ui->label_err_email->setText("Введите почту");
        nologin = !nologin;
    }

    const QString PWRD = this->ui->lineEdit_pwrd->text();
    bool nopwrd = false;
    if (PWRD.trimmed().isEmpty())
    {
        this->ui->label_err_pwrd->setText("Введите пароль");
        nopwrd = !nopwrd;
    }

    if (nologin || nopwrd) return;

    unsigned int id = 0;
    QString name = QString();
    const bool IS_USER = MySql::user().id().name().wlogin(LOGIN).wpassword(PWRD).get().first();
    if (IS_USER)
    {
        id = MySql::user().info().value(SQL_ID).toInt();
        name = MySql::user().info().value(SQL_NAME).toString();
    }
    const bool IS_SELLER = MySql::seller().id().name().wlogin(LOGIN).wpassword(PWRD).get().first();
    if (IS_SELLER)
    {
        id = MySql::seller().info().value(SQL_ID).toInt();
        name = MySql::seller().info().value(SQL_NAME).toString();
    }

    if (!id)
    {
        QMessageBox::warning(this, "Ошибка", "Почта или пароль введены неверно!");
        return;
    }

    if (IS_USER)
    {
        emit LoginDialog::user(id);
        emit LoginDialog::user(name);
    }
    else if (IS_SELLER)
    {
        emit LoginDialog::seller(id);
        emit LoginDialog::seller(name);
    }
    this->accept();
}

void LoginDialog::on_pushButton_reg_clicked() //регистрация
{
    this->hide();
    this->rd.open();
}

