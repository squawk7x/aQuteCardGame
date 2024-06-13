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
    bool isCardFaceVisible_;

public:
    explicit Table(int numberOfPlayers = 2, QWidget *parent = nullptr);
    ~Table();
    void openReadmeFile();

public slots:
    void onSetIsCardVisible(bool isVisible);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void rightMouseClicked();
    void toggleIsCardFaceVisible(bool isVisible);
};

#endif // TABLE_H
