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
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void rightMouseClicked();
    void cbVisibleStatus(int state);

public slots:
    void onRbNumPlayers2();
    void onRbNumPlayers3();
    // void onSetBlindRed(bool red);
};

#endif // TABLE_H
