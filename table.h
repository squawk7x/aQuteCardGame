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

public:
    explicit Table(int numberOfPlayers = 3, QWidget *parent = nullptr);
    ~Table();
    void initializeGame(int numberOfPlayers);
    void openReadmeFile();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void onRbNumPlayers2();
    void onRbNumPlayers3();

private:
    void addSpecialCardsToHand(QKeyEvent *event);

signals:
    void mouseClicked();
    void cbVisibleStatus(int state);

private:
    Ui::Table *ui;
    QSharedPointer<Game> game_;
};

#endif // TABLE_H
