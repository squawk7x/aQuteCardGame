#ifndef HANDDECK_H
#define HANDDECK_H

#include <QObject>
#include "card.h"
#include "cardvec.h"

class Handdeck : public CardVec
{
    Q_OBJECT

public:
    enum class SortOption { Suit, Rank };

    explicit Handdeck(QWidget* parent = nullptr);
    virtual void addCard(QSharedPointer<Card> card) override;
    virtual void removeCard(QSharedPointer<Card> card) override;
    int pointsOnHand();
    // KI
    QSharedPointer<QVector<QSharedPointer<CardVec>>> filteredPermutationsFor(QString rank);
    QSharedPointer<CardVec> bestPermutation(const QSharedPointer<Card>& card,
                                            const QString& jsuit,
                                            int numPlayers);
    // end KI
public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card) override;
    virtual void onToggleCardsVisible(bool isVisible) override;
    void sortCardsBy(Handdeck::SortOption);

signals:
    void handCardClicked(const QSharedPointer<Card>& card);
};

#endif // HANDDECK_H
