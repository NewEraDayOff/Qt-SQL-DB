#include "catalogdialog.h"
#include "ui_catalogdialog.h"

CatalogDialog::CatalogDialog(QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::CatalogDialog)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Каталог");
    this->ui->tableWidget->setColumnCount(6);
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->pd = new ProductDialog(this);
}

CatalogDialog::~CatalogDialog() //деструктор
{
    delete ui;
}

void CatalogDialog::open() //переопределение открытия диалога
{
    const int ROWS = MySql::product().wseller(this->id).get().size();
    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setRowCount(ROWS);

    unsigned int crow = 0;
    while (MySql::product().info().next())
    {
        for (int col = 0; col < this->ui->tableWidget->columnCount(); ++col)
        {
            if (this->ui->tableWidget->item(crow, col) == nullptr)
            {
                this->ui->tableWidget->setItem(crow, col, new QTableWidgetItem());
            }
        }

        const QString NAME = MySql::product().info().value(SQL_NAME).toString();
        const QDate DATE = MySql::product().info().value(SQL_DATE).toDate();
        const unsigned int PRICE = MySql::product().info().value(SQL_PRICE).toUInt();
        const unsigned int PRODUCT = MySql::product().info().value(SQL_ID).toUInt();

        const unsigned int MATERIAL = MySql::product().info().value(SQL_MATERIAL).toUInt();
        const unsigned int COLOR = MySql::product().info().value(SQL_COLOR).toUInt();

        MySql::material(MATERIAL).name().get().first();
        const QString MATERIAL_NAME = MySql::material().info().value(SQL_NAME).toString();

        MySql::color(COLOR).name().get().first();
        const QString COLOR_NAME = MySql::color().info().value(SQL_NAME).toString();

        this->ui->tableWidget->item(crow, 0)->setText(NAME);
        this->ui->tableWidget->item(crow, 0)->setToolTip(NAME);

        this->ui->tableWidget->item(crow, 1)->setText(QString("%1 руб.").arg(PRICE));
        this->ui->tableWidget->item(crow, 1)->setToolTip(QString("%1 руб.").arg(PRICE));

        this->ui->tableWidget->item(crow, 2)->setText(MATERIAL_NAME);
        this->ui->tableWidget->item(crow, 2)->setToolTip(MATERIAL_NAME);

        this->ui->tableWidget->item(crow, 3)->setText(COLOR_NAME);
        this->ui->tableWidget->item(crow, 3)->setToolTip(COLOR_NAME);

        this->ui->tableWidget->item(crow, 4)->setText(QLocale(QLocale::Russian).toString(DATE, QLocale::ShortFormat));
        this->ui->tableWidget->item(crow, 4)->setToolTip(QLocale(QLocale::Russian).toString(DATE, QLocale::LongFormat));

        auto pb = new QPushButton("Посмотреть", this->ui->tableWidget);
        QObject::connect(pb, &QPushButton::clicked, this, [=]()
        {
            this->pd->setID(this->isUser ? this->user : this->id);
            this->pd->setProduct(PRODUCT);
            this->pd->openDialog(this->isUser);
        });
        this->ui->tableWidget->setCellWidget(crow, 5, pb);

        ++crow;
    }

    QDialog::open();
}
