#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "table.h"
#include <qstyle.h>

// Written by Andreas Ottburg, 66123 Saarbrücken, Germany
// Source code available here:
// https://github.com/squawk7x/aQuteCardGame

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/res/styles/main.css");
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
    Table table(3, &mainWindow);

    layout->addWidget(&table);

    // Set layout margins and spacing
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // shrink MainWindow to minimum size
    mainWindow.adjustSize();

    // Show the main window
    mainWindow.show();

    return app.exec();
}
