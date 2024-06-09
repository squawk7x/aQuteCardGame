#include "table.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Table table;
    table.setFixedSize(QSize(1280, 768));
    table.show();

    return app.exec();
}
