#ifndef QTESTTHEQUTECARDGAME_H
#define QTESTTHEQUTECARDGAME_H

#include <QObject>
#include <QTest>

class MyQTest : public QObject {
  Q_OBJECT  // 🔹 Add this macro!

 private slots:
  void qTestCardValues();
  void qTestCardsInGame();
};

#endif  // QTESTTHEQUTECARDGAME_H
