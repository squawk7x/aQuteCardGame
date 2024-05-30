#ifndef CHOOSERS_H
#define CHOOSERS_H

#include <QPushButton>
#include <QString>
#include <QVector>
#include "card.h"

/* ************************************************************************* */

class JsuitChooser : public QPushButton
{
    Q_OBJECT

private:
    QString suit_;
    QString suitname_;
    QString str_;

    // Setters
    void setSuit(const QString& suit);
    void setSuitname(const QString& suit);
    void setStr();
    void loadImage();

public:
    explicit JsuitChooser(QString suit = "♥", QWidget* parent = nullptr);

    // Getters
    QString suit();
    QString suitname();
    QString str();

    // Methods
    void toggle();
    void toggle_to(const QString& target_suit);

public slots:
    void onCardAddedToStack(const QSharedPointer<Card>& card);
};

/* ************************************************************************* */

class EightsChooser : public QPushButton
{
    Q_OBJECT

private:
    QString decision_;

    // Setters
    void loadImage();

public:
    explicit EightsChooser(QString decision = "a", QWidget* parent = nullptr);

    // Getters
    QString decision();

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);

public slots:
    void onEightsInMonitor();
};

/* ************************************************************************* */

class QuteChooser : public QPushButton
{
    Q_OBJECT

private:
    QString decision_;

    // Setters
    void loadImage();

public:
    explicit QuteChooser(QString decision = "y", QWidget* parent = nullptr);

    // Getters
    QString decision();

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);

public slots:
    void onFourCardsInMonitor();
};

/* ************************************************************************* */

class JpointsChooser : public QPushButton
{
    Q_OBJECT

private:
    QString decision_;

    // Setters
    void loadImage();

public:
    explicit JpointsChooser(QString decision = "m",
                            QWidget* parent = nullptr);

    // Getters
    QString decision();

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);

public slots:
    void onJpoints();
};

/* ************************************************************************* */

class RoundChooser : public QPushButton
{
    Q_OBJECT

private:
    QString decision_;

    // Setters
    void loadImage();

public:
    explicit RoundChooser(QString decision = "c", QWidget* parent = nullptr);

    // Getters
    QString decision();

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);

public slots:
    void onNewRound();
};

/* ************************************************************************* */

#endif //CHOOSERS_H
