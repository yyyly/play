#ifndef HKPLAYCORE_H
#define HKPLAYCORE_H

#include "HkPlayCore_global.h"
#include <QObject>
#include <QWidget>


#include"playcore.h"


class HKPLAYCORE_EXPORT HkPlayCore : public PlayCore
{
    Q_OBJECT
public:
    HkPlayCore(QObject *parent = nullptr);
    ~HkPlayCore();

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


};

#endif // HKPLAYCORE_H
