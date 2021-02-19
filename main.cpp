#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name().section('_',0,0);

    QTranslator translator;
    translator.load(QString("carreChinois_")+locale);
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
