#include "table.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Table table;
    table.setFixedSize(QSize(1800, 1024));
    table.show();

    return app.exec();
}
