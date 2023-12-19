#include "lawyers.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lawyers w;
    w.show();
    return a.exec();
}
