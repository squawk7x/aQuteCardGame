#include "table.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Table table;
    table.show();

    return app.exec();
}
