#include "reviewdialog.h"
#include "ui_reviewdialog.h"

ReviewDialog::ReviewDialog(QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::ReviewDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Отзыв");

    QObject::connect(this->ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

ReviewDialog::~ReviewDialog() //деструктор
{
    delete this->ui;
}

void ReviewDialog::setUser(bool me) //сеттер кого отзыв был открыт, свой или чужой
{
    this->me = me;
    this->ui->label_userR->setVisible(!me);
    this->ui->comboBox->setVisible(me);
    this->ui->pushButton_ok->setVisible(me);
    this->ui->textEdit->setReadOnly(!me);
    this->ui->textEdit->clear();

    const bool REVIEW_EXIST = MySql::review().wuser(this->id).wproduct(this->product).get().first();
    this->myReviewExist = REVIEW_EXIST && me;
    if (this->myReviewExist)
    {
        this->myReviewID = MySql::review().info().value(SQL_ID).toUInt();
    }
    if (REVIEW_EXIST)
    {
        if (!me)
        {
            MySql::user(this->id).name().get().first();
            this->ui->label_re->setText(QString("Отзыв от %1").arg(MySql::user().info().value(SQL_NAME).toString()));
        }
        else
            this->ui->label_re->setText("Отзыв");
        const unsigned int RATING = MySql::review().info().value(SQL_RATING).toUInt();
        this->ui->label_userR->setText(QString("%1/10").arg(RATING));
        this->ui->comboBox->setCurrentIndex(RATING - 1);
        this->ui->textEdit->append(MySql::review().info().value(SQL_CONTENT).toString());
    }
}
void ReviewDialog::on_pushButton_ok_clicked() //сохранить отзыв
{
    const unsigned int RATING = this->ui->comboBox->currentIndex() + 1;
    const QString CONTENT = this->ui->textEdit->toPlainText();
    if (this->myReviewExist)
    {
        MySql::review(this->myReviewID).urating(RATING).ucontent(CONTENT)._update();
    }
    else
    {
        MySql::review()._insert(this->id, this->product, RATING, CONTENT);
    }

    QMessageBox::information(this, "Отзыв", "Спасибо, что оставили отзыв!");
    this->accept();
}

