#include "regdialog.h"
#include "ui_regdialog.h"

RegDialog::RegDialog(QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::RegDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Регистрация");
    this->setFixedSize(this->size());
    this->ui->lineEdit_email->setFocus();

    QObject::connect(this->ui->pushButton_cancel, &QPushButton::clicked, this, &QWidget::close);
}

RegDialog::~RegDialog() //деструктор
{
    delete this->ui;
}

void RegDialog::open() //переопределение открытия диалога
{
    this->ui->lineEdit_email->setFocus();
    this->ui->lineEdit_email->clear();
    this->ui->lineEdit_pwrd->clear();
    this->ui->lineEdit_pwrd2->clear();
    this->ui->label_err_email->clear();
    this->ui->label_err_pwrd->clear();
    this->ui->label_err_pwrd2->clear();

    QDialog::open();
}

void RegDialog::on_pushButton_login_clicked() //вход
{
    this->ui->label_err_email->clear();
    this->ui->label_err_pwrd->clear();
    this->ui->label_err_pwrd2->clear();

    const QString LOGIN = this->ui->lineEdit_email->text();
    if (LOGIN.trimmed().isEmpty())
    {
        this->ui->label_err_email->setText("Введите логин");
        return;
    }
    if (!LOGIN.contains(QRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}$")))
    {
        this->ui->label_err_email->setText("Неправильная форма");
        return;
    }

    const bool USER_EXIST = MySql::user().wlogin(LOGIN).get().first();
    const bool SELLER_EXIST = MySql::seller().wlogin(LOGIN).get().first();
    if (USER_EXIST || SELLER_EXIST)
    {
        this->ui->label_err_email->setText("Пользователь уже существует");
        return;
    }

    const QString PWRD = this->ui->lineEdit_pwrd->text();
    if (PWRD.trimmed().isEmpty())
    {
        this->ui->label_err_pwrd->setText("Введите пароль");
        return;
    }

    const QString PWRD2 = this->ui->lineEdit_pwrd2->text();
    if (PWRD2.trimmed().isEmpty())
    {
        this->ui->label_err_pwrd2->setText("Повторите пароль");
        return;
    }

    if (PWRD != PWRD2)
    {
        this->ui->label_err_pwrd->setText("Пароли не совпадают");
        return;
    }

    this->ui->checkBox_isSeller->isChecked() ? MySql::seller()._insert(LOGIN, PWRD)
                                             : MySql::user()._insert(LOGIN, PWRD);

    emit RegDialog::sendLogin(LOGIN);
    this->accept();
}

