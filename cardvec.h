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
    bool isCardInCards(const QSharedPointer<Card>& card);
    virtual QString cardsAsString() const;
    void moveCardTo(QSharedPointer<Card> card, CardVec* targetVec);
    void moveTopCardTo(CardVec* targetVec);
    void copyCardTo(const QSharedPointer<Card>& card, CardVec* targetVec);
    void copyTopCardTo(CardVec* targetVec);
    QSharedPointer<Card> topCard();
    QString mostCommonSuit() const;
    void sortCardsByPattern(int pattern);
    void updateLayout();

    // Getters
    QVector<QSharedPointer<Card>>& cards();
    bool isCardFacesVisible() const;

protected:
    QHBoxLayout* layout_;
    QVector<QSharedPointer<Card>> cards_;
    bool isCardFacesVisible_;

private:
    // Setters
    void setIsCardFacesVisible(bool isVisible);

public slots:
    virtual void onCardClicked(const QSharedPointer<Card>& card);
    virtual void onToggleCardVisibility();
};

#endif // CARDVEC_H
