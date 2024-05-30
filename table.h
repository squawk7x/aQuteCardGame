#ifndef TABLE_H
#define TABLE_H

#include <QGroupBox>
#include <QWidget>
#include "game.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class Table;
}
QT_END_NAMESPACE

class Table : public QWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr);
    ~Table();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void rightMouseClicked();

private:
    Ui::Table *ui;
    std::unique_ptr<Game> game;
};

#endif // TABLE_H
