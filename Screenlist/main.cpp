#include "about.h"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain(APP_URL);
    QCoreApplication::setOrganizationName(APP_COMPANY);
    QCoreApplication::setApplicationName(APP_NAME);

    MainWindow w;
    w.show();
    return a.exec();
}
