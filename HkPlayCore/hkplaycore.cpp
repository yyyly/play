#include "hkplaycore.h"
#include "error.h"
#include "HCNetSDK.h"
#include "PlayM4.h"

#include<QTextCodec>

HkPlayCore *gObject = nullptr;
int count = 0;
LONG nPort = -1;
static  void CALLBACK fDecCBFun(long nPort,char *pBuf,long nSize,FRAME_INFO *pFrameInfo,long nUser,long nReserved2)
{
    long stamp = pFrameInfo->nStamp;

    emit gObject->stampChanged(stamp);
}

HkPlayCore::HkPlayCore(QObject *parent)
    : PlayCore(parent)
{
    BOOL result = PlayM4_GetPort(&nPort);
    if(!result)
    {
        DWORD lastError = PlayM4_GetLastError(nPort);
        Error::getInstance()->sendError(lastError);
        return;
    }
    PlayM4_SetDecCallBackExMend(nPort,fDecCBFun,NULL,0,-1);
    gObject = this;

}

HkPlayCore::~HkPlayCore()
{

}

bool HkPlayCore::openFile(const QString fileName)
{
    QTextCodec *code = QTextCodec::codecForName("GBK");
    QByteArray fileArray = code->fromUnicode(fileName);
    LPSTR pFileName = fileArray.data();
    if(!PlayM4_OpenFile(nPort,pFileName))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return false;
    }
    return true;
}

bool HkPlayCore::play()
{
    if(!getWidget())
        return false;
    if(!PlayM4_Play(nPort,(HWND)getWidget()->winId()))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
    }
    setState(PLAYER_PLAY);

    DWORD time = PlayM4_GetFileTime(nPort);
    if(time == 0xffffffff)
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return false;
    }
    else {
        emit fileTimeChanged(time);
    }
    return true;

}

void HkPlayCore::stop()
{
    if(!PlayM4_Stop(nPort))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return;
    }
}

void HkPlayCore::closeFile()
{
    if(!PlayM4_CloseFile(nPort))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return;
    }

}

void HkPlayCore::free()
{
    if(!PlayM4_FreePort(nPort))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return;
    }
    BOOL result = PlayM4_GetPort(&nPort);
    if(!result)
    {
        DWORD lastError = PlayM4_GetLastError(nPort);
        Error::getInstance()->sendError(lastError);
        return;
    }
    PlayM4_SetDecCallBackExMend(nPort,fDecCBFun,NULL,0,-1);

}

bool HkPlayCore::play(const QString fileName)
{
    openFile(fileName);
    return play();
}

void HkPlayCore::pause()
{
   if(nPort != -1)
   {
      if(!PlayM4_Pause(nPort,1))
      {
          Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
          return;
      }
        setState(PLAYER_PAUSE);
   }
}

void HkPlayCore::rePlay()
{
    if(getOneByOneState() == IN_OneByOne && nPort != -1)
    {
        play();
    }
    else if(getOneByOneState() == OUT_OneByne && nPort != -1)
    {
       if(!PlayM4_Pause(nPort,0))
       {
           Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
           return;
       }

         setState(PLAYER_PLAY);
    }
}


void HkPlayCore::oneByOne()
{
    if(nPort != -1)
    {
        if(!PlayM4_OneByOne(nPort))
        {
                Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
                return;
        }
        setState(PLAYER_PAUSE);
        setSpeedLeve(1);
        setOneByOneState(IN_OneByOne);
    }
}

void HkPlayCore::oneByOneBack()
{
    if(nPort != -1)
    {
        if(!PlayM4_OneByOneBack(nPort))
        {
            DWORD e =  PlayM4_GetLastError(nPort);
            Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
            return;
        }
    }
}



void HkPlayCore::setPlayedTime(int t) //slot ms
{
    if(!PlayM4_SetPlayedTimeEx(nPort,t))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
    }

}


void HkPlayCore::setPlaySpeed(int s)
{
    setState(PLAYER_PLAY);
    int speedLeve = getSpeedLeve();
    if(s > speedLeve && speedLeve !=8)
    {

        while (speedLeve < s) {
            speedLeve = speedLeve * 2;
            if(!PlayM4_Fast(nPort))
            {

                    Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
                    return;
            }

        }

    }
    else if(speedLeve == 8)
    {
        for(int i = 0;i < 3; i++)
        {
            if(!PlayM4_Slow(nPort))
            {
                Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
                return;
            }
        }
        speedLeve = 1;
    }
}

void HkPlayCore::playSound()
{
    if(!PlayM4_PlaySound(nPort))
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return;
    }
}

void HkPlayCore::stopSound()
{
    if(!PlayM4_StopSound())
    {
        Error::getInstance()->sendError(PlayM4_GetLastError(nPort));
        return;
    }
}
