#pragma once

#include <QQmlEngine>
#include <QObject>

using namespace Qt::StringLiterals;

class Klauncher : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Klauncher(QObject *parent = nullptr);

Q_SIGNALS:
    //void incrementCounter();

private:
    //void setupActions() override;
};
