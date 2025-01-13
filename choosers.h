#ifndef CHOOSER_H
#define CHOOSER_H

#include <QPushButton>
#include "card.h"

// -----------------------------------------------------------------------
class Chooser : public QPushButton
{
    Q_OBJECT

private:
    QVector<QString> decs_;
    QString decision_;
    QString str_;
    QString name_;

protected:
    CardType cardType_;

public:
    explicit Chooser(QVector<QString> decs = {""}, QObject* parent = nullptr);

    // Methods
    virtual void toggle();
    void toggle_to(const QString& target_decision);
    void toggleRandom();

    // Getters
    QVector<QString>& decs();
    QString decision();
    QString str();
    QString name();

    //Setters
    void setCardType(CardType type);
    virtual void loadImage();

protected:
    QString imagePath_ = "";
    void setDecision();
    void setStr();
    void setData();

signals:
    void chooserToggled(); // needed for ANDROID
};

// -----------------------------------------------------------------------

class QuteChooser : public Chooser
{
    Q_OBJECT

public:
    explicit QuteChooser(QVector<QString> decs = {"QUTE", "CONTINUE"}, QObject* parent = nullptr);

    virtual void toggle();

signals:
    void quteDecisionChanged(const QString& quteDec);
};

// -----------------------------------------------------------------------

class EightsChooser : public Chooser
{
    Q_OBJECT

public:
    explicit EightsChooser(QVector<QString> decs = {"ALL", "NEXT"}, QObject* parent = nullptr);
};

// -----------------------------------------------------------------------

class JpointsChooser : public Chooser
{
    Q_OBJECT

public:
    explicit JpointsChooser(QVector<QString> decs = {"", "MINUS", "PLUS"},
                            QObject* parent = nullptr);

    virtual void toggle();

public slots:
    void onQuteDecisionChanged(const QString& quteDec);
};

// -----------------------------------------------------------------------
class RoundChooser : public Chooser
{
    Q_OBJECT

public:
    explicit RoundChooser(QVector<QString> decs = {"", "FINISH", "NEW", "GAME"},
                          QObject* parent = nullptr);

signals:
    void finishRound();
    void newRound();
    void newGame();
    void roundDecisionChanged(const QString& dec);

public slots:
    void onQuteDecisionChanged(const QString& quteDec);
};

// -----------------------------------------------------------------------
class JsuitChooser : public Chooser
{
    Q_OBJECT

public:
    explicit JsuitChooser(QVector<QString> decs = {"♦", "♠", "♥", "♣"}, QObject* parent = nullptr);

    QString suit();
    virtual void loadImage();

public slots:
    void onCardAddedToStack(const QSharedPointer<Card>& card);
};

// -----------------------------------------------------------------------

#endif // CHOOSER_H
