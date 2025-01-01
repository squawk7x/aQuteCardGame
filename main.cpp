#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include "table.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window
    QWidget mainWindow;

    mainWindow.resize(1280, 720); //

    // Create the layout
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    // Create the table widget and add it to the layout
    Table table;

    layout->addWidget(&table);

    // Optionally, set layout margins and spacing if necessary
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Show the main window
    mainWindow.show();

    return app.exec();
}
