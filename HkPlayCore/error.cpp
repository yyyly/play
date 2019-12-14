#include "error.h"
#include <QDebug>

QMap<DWORD,QString> Error::errorMap = {{0,"没有错误"},
                                      {1,"输入参数非法"},
                                       {2,"调用顺序不对"},
                                      {3,"多媒体时钟设置失效"},
                                      {4,"视频解码失败"},
                                      {5,"音频解码失败"},
                                      {6,"分配内存失败"},
                                      {7,"文件操作失败"},
                                      {8,"创建线程事件失败"},
                                      {9,"创建DierctDraw失败"},
                                       {10,"创建后端缓存失败"},
                                       {11,"缓冲区满，输入流失败"},
                                       {12,"创建音频设备失败"},
                                       {13,"设置音量失败"},
                                       {14,"只能在播放文件时才能使用此接口"},
                                       {19,"初始化解码器失败"}};

Error::Error(QObject *parent) : QObject(parent)
{

}

void Error::sendError(DWORD e)
{
    qDebug() << QString("错误：%1").arg(e) << "       "  << errorMap.value(e);
    QString str = errorMap.value(e);
    emit errorSended(e,str);
}
