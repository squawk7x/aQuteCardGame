#ifndef DECK_H
#define DECK_H

#include <QObject>

class Deck : public QObject
{
    Q_OBJECT
public:
    explicit Deck(QObject *parent = nullptr);

signals:
};

#endif // DECK_H
