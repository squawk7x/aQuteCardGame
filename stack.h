#ifndef STACK_H
#define STACK_H

#include <QSharedPointer>
#include "cardvec.h"

class Stack : public CardVec
{
    Q_OBJECT

public:
    explicit Stack(QWidget* parent = nullptr, const QVector<QSharedPointer<Card>>& rhs = {});
    virtual ~Stack();

    virtual void addCard(QSharedPointer<Card> card) override;
};

#endif // STACK_H
