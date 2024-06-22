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

    // Override the virtual addCard method
    void addCard(QSharedPointer<Card> card) override;

public slots:
    void onCardAddedToStack(const QSharedPointer<Card>& card);
    virtual void onToggleCardsVisible(bool isVisible) override;

signals:
    void eightsInMonitor();
    void fourCardsInMonitor();
};

#endif // MONITOR_H
