#ifndef BLIND_H
#define BLIND_H

#include <QSharedPointer>
#include "cardvec.h"

class Blind : public CardVec
{
    Q_OBJECT

public:
    explicit Blind(QWidget* parent = nullptr, const QVector<QSharedPointer<Card>>& rhs = {});
    virtual ~Blind();

    void shuffle();
    QSharedPointer<Card> drawCardFromBlind();
};

#endif // BLIND_H
