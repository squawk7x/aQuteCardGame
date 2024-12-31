#ifndef CARD_H
#define CARD_H

#include <QPushButton>
#include <QSharedPointer>
#include <QString>
#include <QVector>

// Definitions for the global card properties
extern QVector<QString> suits;
extern QVector<QString> ranks;
extern QVector<QString> ranknames;
extern QVector<QString> suitnames;

// cards and choosers use text not image
extern bool isAndroidVersion;

enum class cardType { small, normal };

class Card : public QPushButton
{
    Q_OBJECT

private:
    QString suit_;
    QString rank_;
    QString suitname_;
    QString rankname_;
    QString str_;
    int value_;

public:
    explicit Card(const QString& suit, const QString& rank, QWidget* parent = nullptr);
    explicit Card(const QString& cardStr, QWidget* parent = nullptr);
    Card(const Card& other);                // Copy constructor
    Card& operator=(const Card& other);     // Copy assignment operator
    Card(Card&& other) noexcept;            // Move constructor
    Card& operator=(Card&& other) noexcept; // Move assignment operator

    bool operator==(const Card& other) const;
    bool operator<(const Card& other) const;
    bool operator>(const Card& other) const;
    QSharedPointer<Card> clone(QWidget* parent = nullptr) const;

    // Getters
    QString suit() const;
    QString rank() const;
    QString suitname() const;
    QString rankname() const;
    QString str() const;
    int value() const;

    // Setters
    void loadImage(bool isCardFaceVisible = true, cardType type = cardType::small);

private:
    void initCard();
    void setSuitname(const QString& suit);
    void setRankname(const QString& rank);
    void setStr();
    void setValue(const QString& rank);

signals:
    void cardClicked(const QSharedPointer<Card>& card);
};

#endif // CARD_H
