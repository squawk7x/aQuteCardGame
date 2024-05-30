#ifndef PLAYED_H
#define PLAYED_H

#include <QSharedPointer> // Include for QSharedPointer
#include "cardvec.h"

class Played : public CardVec
{
    Q_OBJECT

public:
    explicit Played(QWidget *parent = nullptr);
    virtual ~Played();

    virtual void addCard(QSharedPointer<Card> card); // Method to add a smart pointer to a card

public slots:
    void onCardAddedToStack(const QSharedPointer<Card> &card); // Slot to handle card added to stack
};

#endif // PLAYED_H
