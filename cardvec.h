#ifndef CARDVEC_H
#define CARDVEC_H

#include <QHBoxLayout>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>
#include <QWidget>
#include "card.h"

class CardVec : public QWidget
{
    Q_OBJECT

public:
    explicit CardVec(QWidget* parent = nullptr, QVector<QSharedPointer<Card>> rhs = {});
    virtual ~CardVec();

    virtual void addCard(QSharedPointer<Card> card);
    virtual void removeCard(QSharedPointer<Card> card);
    virtual QString cardsAsString() const;
    void copyCardTo(const QSharedPointer<Card>& card, CardVec* targetVec);
    void copyTopCardTo(CardVec* targetVec);
    void moveCardTo(QSharedPointer<Card> card, CardVec* targetVec);
    void moveTopCardTo(CardVec* targetVec);
    QSharedPointer<Card> topCard();
    void clearCards();
    bool isCardInCards(const QSharedPointer<Card>& card);
    bool isSuitInCards(const QString& suit);
    bool isRankInCards(const QString& rank);
    void sortCardsByPattern(const QVector<QString>& pattern);
    int countCardsOfRank(const QString& rank) const;
    QString mostCommonSuit() const;
    QString suitOfRankWithMostPoints() const;
    void updateLayout();

    // Getters
    QVector<QSharedPointer<Card>>& cards();

protected:
    QHBoxLayout* layout_;
    QVector<QSharedPointer<Card>> cards_;
    bool isCardFaceVisible_;

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card);
    virtual void onToggleCardsVisible(bool isVisible);
    virtual void onToggleCardsType(cardType newType);
};

#endif // CARDVEC_H
