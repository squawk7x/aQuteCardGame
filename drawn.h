#ifndef DRAWN_H
#define DRAWN_H

#include <QObject>
#include "cardvec.h"

class Drawn : public CardVec
{
public:
    explicit Drawn(QWidget *parent = nullptr);
    virtual ~Drawn();

public slots:
    void onCardDrawnFromBlind(const QSharedPointer<Card> &card);
};

#endif // DRAWN_H
