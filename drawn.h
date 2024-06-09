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
    void onCardMustFromBlind(const QSharedPointer<Card> &card);
    virtual void onToggleIsTableCardsVisible(bool isTableCardsVisible) override;
};

#endif // DRAWN_H
