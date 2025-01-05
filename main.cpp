#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "table.h" // Ensure this is the correct path for Table
#include "ui_table.h"
#include <qstyle.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load the stylesheet from the resource file
    QFile file(":/res/styles/aQuteCardgame.css");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString stylesheet = file.readAll();
        app.setStyleSheet(stylesheet); // Apply the stylesheet globally
        file.close();
    } else {
        qDebug() << "Failed to load stylesheet.";
    }

    // Create the main window
    QWidget mainWindow;

    mainWindow.resize(1280, 720);

    // Create the layout
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    // Create the table widget and add it to the layout
    Table table;

    layout->addWidget(&table);

    // Set layout margins and spacing
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Show the main window
    mainWindow.show();

    return app.exec();
}
