#ifndef ERROR_H
#define ERROR_H

#include <QObject>
#include <QMap>
#include <QString>
#include "HCNetSDK.h"

class Error : public QObject
{
    Q_OBJECT
private:
    explicit Error(QObject *parent = nullptr);
public:
    static Error* getInstance()
    {
        static Error * instance;
        if(instance == nullptr)
        {
            instance = new Error();
        }
        return instance;
    }
    void sendError(DWORD e);
signals:
    void errorSended(DWORD e,const QString describle);

public slots:
private:
    static QMap<DWORD,QString> errorMap;

};

#endif // ERROR_H
