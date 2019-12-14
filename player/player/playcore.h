#ifndef PLAYCORE_H
#define PLAYCORE_H
#include<QObject>
#include "enum.h"

class QWidget;
class QString;

class PlayCore : public QObject
{
    Q_OBJECT
public:
    PlayCore(QObject *parent = nullptr);
    ~PlayCore();

    virtual bool openFile(const QString fileName) =0;
    virtual void closeFile() = 0;
    virtual void free() = 0;
    virtual bool play() = 0;
    virtual bool play(const QString fileName) = 0;
    virtual void stop() = 0;
    virtual void pause() = 0; // s = 1,暂停 0，恢复
    virtual void rePlay() = 0;
    virtual void oneByOne() = 0;
    virtual void oneByOneBack() = 0;
    virtual void playSound() = 0;
    virtual void stopSound() = 0;
    virtual void setPlaySpeed(int s) = 0;

    int  getPlaySpeed(){return speedLeve;}
    QWidget *getWidget(){return widget;}
    void setWidget(QWidget *w){widget = w;}
    PlayState getState(){return state;}
    void setState(PlayState s){state = s;}
    OneByOneState getOneByOneState(){return  oneByoneState;}
    void setOneByOneState(OneByOneState s){oneByoneState = s;}
    void setSpeedLeve(int s){speedLeve = s;}
    int getSpeedLeve(){return speedLeve;}
signals:
    void stampChanged(int n);
    void fileTimeChanged(int t);

public slots:
    virtual void setPlayedTime(int t) = 0;
private:
    QWidget *widget;
    PlayState state;
    OneByOneState oneByoneState;
    int speedLeve;//播放速度
};

#endif // PLAYCORE_H
