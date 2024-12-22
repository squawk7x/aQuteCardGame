#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include "table.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window
    QWidget mainWindow;

    // Get the screen size and scaling factor
    QScreen *screen = app.primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Scale factor for adjusting the size for mobile screens
    // float scaleFactor = 1.0; // Adjust this value to fit your desired screen ratio

    // Set the window size based on the screen size (for mobile)
    // mainWindow.setMinimumSize(screenWidth * scaleFactor, screenHeight * scaleFactor);

    // Create the layout
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    // Create the table widget and add it to the layout
    Table table;

    // Adjust table size relative to the screen (scaled version)
    // table.setFixedSize(screenWidth * scaleFactor, screenHeight * scaleFactor);

    layout->addWidget(&table);

    // Optionally, set layout margins and spacing if necessary
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Show the main window
    mainWindow.show();

    return app.exec();
}
