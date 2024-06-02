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
    explicit Table(QWidget *parent = nullptr);
    ~Table();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void rightMouseClicked();

private:
    Ui::Table *ui;
    QSharedPointer<Game> game; // Changed from std::unique_ptr to QSharedPointer
};

#endif // TABLE_H
