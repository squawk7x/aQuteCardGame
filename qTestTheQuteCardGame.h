#ifndef QTESTTHEQUTECARDGAME_H
#define QTESTTHEQUTECARDGAME_H

#include <QObject>
#include <QtTest/QtTest>

class MyQTest : public QObject
{
    Q_OBJECT

private slots:

    void qTestCardValues();
    void qTestCardsInGame();
};

#endif // TESTTHEQUTECARDGAME_H