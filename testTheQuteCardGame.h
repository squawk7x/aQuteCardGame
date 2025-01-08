#ifndef TESTTHEQUTECARDGAME_H
#define TESTTHEQUTECARDGAME_H

#include <QObject>
#include <QtTest/QtTest>

class MyTest : public QObject
{
    Q_OBJECT

private slots:

    void testCardValues();
    void testCardsInGame();
};

#endif // TESTTHEQUTECARDGAME_H
