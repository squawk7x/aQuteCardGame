#ifndef PLAYABLE_H
#define PLAYABLE_H

#include <QSharedPointer>
#include "cardvec.h"

class Playable : public CardVec
{
    Q_OBJECT

public:
    explicit Playable(QWidget* parent = nullptr);
    virtual ~Playable();

    virtual void addCard(QSharedPointer<Card> card) override;
    void togglePlayableCards();

signals:
    void handCardClicked(QSharedPointer<Card> card);

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card) override;
    virtual void onToggleCardsVisible(bool isVisible) override;
};

#endif // PLAYABLE_H
