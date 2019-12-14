#include<QDebug>

#include "dhplaycore.h"
#include "error.h"
#include "dhplay.h"

#include<QTextCodec>
#include <synchapi.h>

DhPlayCore *gObject = nullptr;
int count = 0;
LONG nPort = 0;//播放端口,本程序中保证唯一性，在构造函数中获取，对外隐藏这个。
void static CALLBACK DecCBFun(LONG Port, char *pBuf, LONG nSize, FRAME_INFO *pFrameInfo, void *pUserData, LONG nReserved2)
{
}

static  void CALLBACK FileRefDoneCBFunEx(DWORD nPort, BOOL bIndexCreated, void* pUserData)
{
    if(bIndexCreated)
    {
        qDebug() << "创建索引成功" << endl;

    }
}

DhPlayCore::DhPlayCore(QObject *parent)
    : PlayCore(parent),
      timer(new QTimer(this))
{
    nPort = -1;
    BOOL result = PLAY_GetFreePort(&nPort);
    if(!result)
    {
        DWORD lastError = PLAY_GetLastError(nPort);
        Error::getInstance()->sendError(lastError);
        return;
    }
    PLAY_SetDecCallBack(nPort,DecCBFun);
    gObject = this;

    connect(timer,&QTimer::timeout,this,&DhPlayCore::onTimer);
}

DhPlayCore::~DhPlayCore()
{

}

void DhPlayCore::onTimer()
{
    DWORD t = PLAY_GetPlayedTimeEx(nPort)  + 1000;
    emit stampChanged(t);
}

bool DhPlayCore::openFile(const QString fileName)
{
    PLAY_SetFileRefCallBackEx(nPort,FileRefDoneCBFunEx,NULL);
    QTextCodec *code = QTextCodec::codecForName("GBK");
    QByteArray fileArray = code->fromUnicode(fileName);
    LPSTR pFileName = fileArray.data();
    if(!PLAY_OpenFile(nPort,pFileName))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
        return false;
    }
    Sleep(100);
    DWORD time = PLAY_GetFileTime(nPort);
    if(time == 0)
    {
        DWORD e = PLAY_GetLastErrorEx();
        Error::getInstance()->sendError(e);
        return false;
    }
    else {
        emit stampChanged(0);
        emit fileTimeChanged(time);
    }
}


bool DhPlayCore::play()
{
    if(!getWidget())
        return false;

    if(!PLAY_Play(nPort,(HWND)getWidget()->winId()))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
    }
    setState(PLAYER_PLAY);
    if(timer->isActive())
    {
        timer->stop();
    }
    timer->start(100);
    return true;
}

void DhPlayCore::stop()
{
    if(!PLAY_Stop(nPort))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
        return;
    }
    timer->stop();
}

void DhPlayCore::closeFile()
{
    if(!PLAY_CloseFile(nPort))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
        return;
    }

}

void DhPlayCore::free()
{
    if(!PLAY_ReleasePort(nPort))
    {
        DWORD e = PLAY_GetLastError(nPort);
        Error::getInstance()->sendError(e);
        return;
    }
    BOOL result = PLAY_GetFreePort(&nPort);
    if(!result)
    {
        DWORD lastError = PLAY_GetLastError(nPort);
        Error::getInstance()->sendError(lastError);
        return;
    }
    PLAY_SetDecCallBack(nPort,DecCBFun);
}

bool DhPlayCore::play(const QString fileName)
{
    openFile(fileName);
    return play();
}

void DhPlayCore::pause()
{
   if(nPort != -1)
   {
      if(!PLAY_Pause(nPort,1))
      {
          Error::getInstance()->sendError(PLAY_GetLastError(nPort));
          return;
      }
      timer->stop();
      setState(PLAYER_PAUSE);
   }
}

void DhPlayCore::rePlay()
{
    if(getOneByOneState() == IN_OneByOne && nPort != -1)
    {
        play();
    }
    else if(getOneByOneState() == OUT_OneByne && nPort != -1)
    {
       if(!PLAY_Pause(nPort,0))
       {
           Error::getInstance()->sendError(PLAY_GetLastError(nPort));
           return;
       }

         setState(PLAYER_PLAY);
    }
    timer->start(100);
}


void DhPlayCore::oneByOne()
{
    if(nPort != -1)
    {
        if(!PLAY_OneByOne(nPort))
        {
                Error::getInstance()->sendError(PLAY_GetLastError(nPort));
                return;
        }
        setState(PLAYER_PAUSE);
        setSpeedLeve(1);
        setOneByOneState(IN_OneByOne);
        timer->stop();
        DWORD t = PLAY_GetPlayedTimeEx(nPort)  + 1000;
        emit stampChanged(t);
    }
}

void DhPlayCore::oneByOneBack()
{
    if(nPort != -1)
    {
        if(!PLAY_OneByOneBack(nPort))
        {
            Error::getInstance()->sendError(PLAY_GetLastError(nPort));
            return;
        }
    }
}



void DhPlayCore::setPlayedTime(int t) //slot ms
{
    if(!PLAY_SetPlayedTimeEx(nPort,t))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
    }

}


void DhPlayCore::setPlaySpeed(int s)
{
    setState(PLAYER_PLAY);
    int speedLeve = getSpeedLeve();
    if(s > speedLeve && speedLeve !=8)
    {

        while (speedLeve < s) {
            speedLeve = speedLeve * 2;
            if(!PLAY_Fast(nPort))
            {

                    Error::getInstance()->sendError(PLAY_GetLastError(nPort));
                    return;
            }

        }

    }
    else if(speedLeve == 8)
    {
        for(int i = 0;i < 3; i++)
        {
            if(!PLAY_Slow(nPort))
            {
                Error::getInstance()->sendError(PLAY_GetLastError(nPort));
                return;
            }
        }
        speedLeve = 1;
    }
}

void DhPlayCore::playSound()
{
    if(!PLAY_PlaySound(nPort))
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
        return;
    }
}

void DhPlayCore::stopSound()
{
    if(!PLAY_StopSound())
    {
        Error::getInstance()->sendError(PLAY_GetLastError(nPort));
        return;
    }
}


