#ifndef TABLE_H
#define TABLE_H

#include <QGroupBox>
#include <QSharedPointer>
#include <QWidget>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Table;
}
QT_END_NAMESPACE

class Table : public QWidget
{
    Q_OBJECT

private:
    Ui::Table *ui;
    QSharedPointer<Game> game_;

public:
    explicit Table(int numberOfPlayers = 3, QWidget *parent = nullptr);
    ~Table();
    void initializeGame(int numberOfPlayers);
    void openReadmeFile();

protected:
    void keyPressEvent(QKeyEvent *event) override; // Special Keys -> extra cards for testing

private:
    void addSpecialCardsToHand(QKeyEvent *event);

signals:
    // void mouseClicked();
    void cbVisible(bool isVisible);

public slots:
    void onRbNumPlayers2();                // Table initializes new game
    void onRbNumPlayers3();                // Table initializes new game
    void onCbSound();                      // Transfer to Game
    void onCbVisible();                    // Transfer to Game
    void onRbSuit();                       // Transfer to Game
    void onRbRank();                       // Transfer to Game
    void onResetCbVisible(bool isVisible); // forAndoid
    void onNextClicked();                  // Game Control
    void onDrawClicked();                  // Game Control
    void onHelpClicked();                  // Game Control
    void onChooserToggled(); // if (isAndroidVersion) -> refresh Jsuit, Qute, Eights, Jpoints, Round
};

#endif // TABLE_H
