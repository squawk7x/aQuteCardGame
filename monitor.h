#ifndef MONITOR_H
#define MONITOR_H

#include <QSharedPointer>
#include "cardvec.h"

class Monitor : public CardVec
{
    Q_OBJECT

public:
    explicit Monitor(QWidget* parent = nullptr);
    virtual ~Monitor();

    // Override the addCard method
    void addCard(QSharedPointer<Card> card) override;

signals:
    void eightsInMonitor();
    void fourCardsInMonitor();

public slots:
    void onCardAddedToStack(const QSharedPointer<Card>& card);
    virtual void onToggleCardsVisible(bool isVisible) override;
};

#endif // MONITOR_H
