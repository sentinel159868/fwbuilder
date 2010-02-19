#ifndef TUTORIALANIMATOR_H
#define TUTORIALANIMATOR_H

#include <QMenu>
#include <QPoint>
#include <QThread>
#include <QObject>
#include <QStringList>
#include <QAction>

#include "TutorialHelper.h"

class TutorialAnimator : public QThread
{
Q_OBJECT
    TutorialHelper *helper;
    QWidget *widget;

    QStringList commands;
    int currentCommand;

    void animate(int command);
    void moveMouse(QStringList input);
    void clickWidget(QStringList input);
    void typeWidget(QStringList input);
    void wait(QStringList input);
    void hoverMenuItem(QStringList input);
    void clickMenuItem(QStringList input);

    QObject* findChild(QObject *parent, QString name);
    QWidget* findWidget(QString name);
    QPoint findMenuItemPos(QMenu *menu, QString item);
    QObject* getWidget(QStringList input);
public:
    explicit TutorialAnimator(QObject *parent, QString commands);
    void run();

public slots:
    void scenarioFinished();

};

#endif // TUTORIALANIMATOR_H