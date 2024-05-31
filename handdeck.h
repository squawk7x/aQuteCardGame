#ifndef HANDDECK_H
#define HANDDECK_H

#include <QObject>
#include "card.h"
#include "cardvec.h"

class Handdeck : public CardVec
{
    Q_OBJECT

public:
    explicit Handdeck(QWidget* parent = nullptr);
    virtual void addCard(QSharedPointer<Card> card) override;
    virtual void removeCard(QSharedPointer<Card> card) override;

signals:
    void handCardClicked(const QSharedPointer<Card>& card);

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card) override;
};

#endif // HANDDECK_H
