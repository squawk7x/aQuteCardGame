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
    virtual void clearCards();
    QString cardsAsString() const;
    void moveCardTo(QSharedPointer<Card> card, CardVec* targetVec);
    void moveTopCardTo(CardVec* targetVec);
    void copyCardTo(const QSharedPointer<Card>& card, CardVec* targetVec);
    void copyTopCardTo(CardVec* targetVec);
    virtual QSharedPointer<Card> drawTopCard();
    QSharedPointer<Card> topCard();
    QString mostCommonSuit() const;
    void toggleIsVisible();
    void sortCards(int pattern);

    // Getters
    QVector<QSharedPointer<Card>>& cards();
    bool isVisible() const;

protected:
    QHBoxLayout* layout_;
    QVector<QSharedPointer<Card>> cards_;
    bool isVisible_;

private:
    // Setters
    void setIsVisible(bool isVisible);

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card);
};

#endif // CARDVEC_H
