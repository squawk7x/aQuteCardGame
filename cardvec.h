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
    QHBoxLayout* layout_;

    explicit CardVec(QWidget* parent = nullptr, QVector<QSharedPointer<Card>> rhs = {});
    virtual ~CardVec();

    virtual void addCard(QSharedPointer<Card> card);
    void removeCard(QSharedPointer<Card> card);
    void clearCards();
    QString cardsAsString();
    void moveCardTo(QSharedPointer<Card> card, CardVec* targetVec);
    void moveTopCardTo(CardVec* targetVec);
    void copyCardTo(QSharedPointer<Card> card, CardVec* targetVec);
    void copyTopCardTo(CardVec* targetVec);
    QSharedPointer<Card> drawTopCard();
    void toggleIsVisible();
    void sortCards(int pattern); // Declaration of the new method

    // Getters
    QVector<QSharedPointer<Card>>& cards();
    bool isVisible() const;

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card);

protected:
    QVector<QSharedPointer<Card>> cards_;
    bool isVisible_;

private:
    // Setters
    void setIsVisible(bool isVisible);
};

#endif // CARDVEC_H
