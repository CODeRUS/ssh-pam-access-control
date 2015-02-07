#include "dbusmain.h"
#include "adaptor.h"

#include <QDebug>
#include <QDBusError>
#include <QCoreApplication>

DBusMain::DBusMain(QObject *parent) :
    QObject(parent)
{
}

DBusMain::~DBusMain()
{
}

void DBusMain::start()
{
    bool success = false;
    QString baseService("org.coderus.pam.sshd%1");
    int serviceIndex = 0;
    QString serviceName;

    while (!success) {
        serviceName = baseService.arg(serviceIndex);
        success = QDBusConnection::sessionBus().registerService(serviceName);
        if (!success) {
            serviceIndex++;
            if (serviceIndex > 100) {
                QCoreApplication::exit(0);
                return;
            }
        }
    }

    Adaptor *adaptor = new Adaptor(serviceName, this);

    QDBusConnection::sessionBus().registerObject("/", this);

    adaptor->sendRequest();
}
