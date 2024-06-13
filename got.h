#ifndef GOT_H
#define GOT_H

#include <QObject>
#include "cardvec.h"

class Got : public CardVec
{
public:
    explicit Got(QWidget *parent = nullptr);
    virtual ~Got();

public slots:
    void onCardBadFromBlind(const QSharedPointer<Card> &card);
    // virtual void onToggleIsCardFaceVisible(bool isVisible) override;
};

#endif // GOT_H
