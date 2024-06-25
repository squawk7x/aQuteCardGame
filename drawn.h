#ifndef DRAWN_H
#define DRAWN_H

#include <QObject>
#include "cardvec.h"

class Drawn : public CardVec
{
    Q_OBJECT

public:
    explicit Drawn(QWidget *parent = nullptr);
    virtual ~Drawn();

    void addCard(QSharedPointer<Card> card);

public slots:
    void onCardMustFromBlind(const QSharedPointer<Card> &card);
};

#endif // DRAWN_H
