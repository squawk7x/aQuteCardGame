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
    virtual void removeCard(QSharedPointer<Card> card) override;

    void shuffle();
    void showTopCard();

public slots:
    virtual void onToggleIsCardFaceVisible(bool isVisible) override;
};

#endif // BLIND_H
