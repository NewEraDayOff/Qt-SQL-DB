#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok = MySql::db().isOpen();
    while (!ok)
    {
        QMessageBox::StandardButton sb = QMessageBox::warning(nullptr,
                                                              "Ошибка", "Не удалось открыть базу данных",
                                                              QMessageBox::Retry | QMessageBox::Close);
        if (sb == QMessageBox::Close)
        {
            ok = false;
            break;
        }
        ok = MySql::db().open();
    }

    if (!ok) return 0;
    MainWindow w;
    w.show();
    return a.exec();
//    QMetaObject::invokeMethod(&a, "quit", Qt::QueuedConnection);
}
