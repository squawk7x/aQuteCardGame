#ifndef HANDDECK_H
#define HANDDECK_H

#include <QObject>
#include "card.h"
#include "cardvec.h"

enum class SortOption { Suit, Rank };

class Handdeck : public CardVec
{
    Q_OBJECT

public:

    explicit Handdeck(QWidget* parent = nullptr);
    virtual void addCard(QSharedPointer<Card> card) override;
    virtual void removeCard(QSharedPointer<Card> card) override;
    void playThisCard(const Card& cardToPlay);
    QVector<QString> patternByRankPoints();
    void prependRank(QVector<QString>& pattern, const QString& rank);
    void appendRank(QVector<QString>& pattern, const QString& rank);
    void permuteRanks(const QString& rank,
                      const QSharedPointer<Card>& stackCard,
                      const QString& stackSuit);
    int pointsOnHand();

signals:
    void handCardClicked(const QSharedPointer<Card>& card);

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card) override;
    virtual void onToggleCardsVisible(bool isVisible) override;
    void sortCardsBy(SortOption);
};

#endif // HANDDECK_H
