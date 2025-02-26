#ifndef GOT_H
#define GOT_H

#include <QObject>
#include "cardvec.h"

class Got : public CardVec
{
    Q_OBJECT

public:
    explicit Got(QWidget *parent = nullptr);
    virtual ~Got();

public slots:
    void onCardBadFromBlind(const QSharedPointer<Card> &card);
};

#endif // GOT_H
