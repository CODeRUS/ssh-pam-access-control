#include "dbusmain.h"
#include "adaptor.h"

#include <QDebug>
#include <QDBusError>
#include <QCoreApplication>

DBusMain::DBusMain(QObject *parent) :
    QObject(parent)
{
    qDebug() << "DBusMain constructor";
}

DBusMain::~DBusMain()
{
    qDebug() << "DBusMain destructor";
}

void DBusMain::start()
{
    bool success = false;
    QString baseService("org.coderus.pam.sshd%1");
    int serviceIndex = 0;
    QString serviceName;

    while (!success) {
        serviceName = baseService.arg(serviceIndex);
        qDebug() << "trying" << serviceName;
        success = QDBusConnection::sessionBus().registerService(serviceName);
        if (!success) {
            qDebug() << "error:" << QDBusConnection::sessionBus().lastError().message();
            serviceIndex++;
            if (serviceIndex > 100) {
                QCoreApplication::exit(0);
                return;
            }
        }
        else {
            qDebug() << "succcess!";
        }
    }

    QScopedPointer<Adaptor> adaptor(new Adaptor(serviceName, this));

    QDBusConnection::sessionBus().registerObject("/", this);

    adaptor->sendRequest();
}
