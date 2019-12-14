#ifndef DHPLAYCORE_H
#define DHPLAYCORE_H

#include "DhPlayCore_global.h"
#include <QWidget>
#include <QTimer>
#include "playcore.h"


class DHPLAYCORE_EXPORT DhPlayCore : public PlayCore
{
    Q_OBJECT
public:
    DhPlayCore(QObject *parent = nullptr);
    ~DhPlayCore();

    bool openFile(const QString fileName);
    void closeFile();
    void free();
    bool play();
    bool play(const QString fileName);
    void stop();
    void pause(); // s = 1,暂停 0，恢复
    void rePlay();
    void oneByOne();
    void oneByOneBack();
    void playSound();
    void stopSound();
    void setPlaySpeed(int s);

public slots:
    void setPlayedTime(int t);
private:
    QTimer *timer;
private slots:
    void onTimer();
};

#endif // DHPLAYCORE_H
