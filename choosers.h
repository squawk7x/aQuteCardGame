#ifndef CHOOSERS_H
#define CHOOSERS_H

#include <QPushButton>
#include <QString>
#include <QVector>
#include "card.h"

class BaseChooser : public QPushButton
{
    Q_OBJECT

signals:
    void chooserToggled();
};

// -----------------------------------------------------------------------

class JsuitChooser : public BaseChooser
{
    Q_OBJECT

private:
    QString suit_;
    QString suitname_;
    QString str_;

public:
    explicit JsuitChooser(QString suit = "♥", QWidget* parent = nullptr);

    // Getters
    QString suit();
    QString suitname();
    QString str();

    // Methods
    void toggle();
    void toggle_to(const QString& target_suit);

private:
    // Setters
    void setSuit(const QString& suit);
    void setSuitname(const QString& suit);
    void setStr();
    void loadImage();

    // signals:
    //     void jsuitToggled();

public slots:
    void onCardAddedToStack(const QSharedPointer<Card>& card);
};

// -----------------------------------------------------------------------

class EightsChooser : public BaseChooser
{
    Q_OBJECT

private:
    QString decision_;
    QString str_;

public:
    explicit EightsChooser(QString decision = "a", QWidget* parent = nullptr);

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);
    void toggleRandom(const QString& dec1 = "a", const QString& dec2 = "n");

    // Getters
    QString str();
    QString decision();

private:
    // Setters
    void setStr();
    void loadImage();

    // public slots:
    //     void onEightsInMonitor();
};

// -----------------------------------------------------------------------

class QuteChooser : public BaseChooser
{
    Q_OBJECT

private:
    QString decision_;
    QString str_;

public:
    explicit QuteChooser(QString decision = "y", QWidget* parent = nullptr);

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);
    void toggleRandom(const QString& dec1 = "y", const QString& dec2 = "n");

    // Getters
    QString str();
    QString decision();

private:
    // Setters
    void setStr();
    void loadImage();

signals:
    void quteDecisionChanged(const QString& dec);

public slots:
    // void onFourCardsInMonitor(); // this is handled otherwise
};

// -----------------------------------------------------------------------

class JpointsChooser : public BaseChooser
{
    Q_OBJECT

private:
    QString decision_;
    QString str_;

public:
    explicit JpointsChooser(QString decision = "m", QWidget* parent = nullptr);

    // Methods
    void toggle();
    void toggle_to(const QString& target_decision);
    void toggleRandom(const QString& dec1 = "m", const QString& dec2 = "p");

    // Getters
    QString str();
    QString decision();

private:
    // Setters
    void setStr();
    void loadImage();

public slots:
    void onQuteDecisionChanged(const QString& dec);
};

// -----------------------------------------------------------------------

class RoundChooser : public BaseChooser
{
    Q_OBJECT

private:
    QString decision_;
    QString str_;

public:
    explicit RoundChooser(QString decision = "f", QWidget* parent = nullptr);

    // Getters
    QString str();
    QString decision();

    // Setters
    void setStr();
    void loadImage();
    void setDecision(const QString& target_decision);

signals:
    void finishRound();
    void newRound();
    void newGame();

public slots:
    void onQuteDecisionChanged(const QString& dec);
    // Qute rejected -> Roundchooser is removed
};

// -----------------------------------------------------------------------

#endif //CHOOSERS_H
