#ifndef TABLE_H
#define TABLE_H

#include <QGroupBox>
#include <QSharedPointer>
#include <QWidget>
// #include "card.h"
#include "game.h"

enum class ButtonColor { Grey, Yellow };

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
    void onRbNumPlayers2(); // from Game initializes new game
    void onRbNumPlayers3(); // from Game initializes new game

    void onCbSound();                      // Transfer to Game
    void onCbVisible();                    // Transfer to Game

    void onRbSuit();                       // Transfer to Game
    void onRbRank();                       // Transfer to Game

    void onHelpClicked();                  // Game Control
    void onNextClicked();                  // Game Control
    void onDrawClicked();                  // Game Control

    void onResetCbVisible(); // for Andoid Version
    void onChooserToggled();

    void onPaintDrawButton(DrawOption drawOption);
    void onPaintNextButton(NextOption nextOption);
};

#endif // TABLE_H
