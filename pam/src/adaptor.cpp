#include "adaptor.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

Adaptor::Adaptor(const QString &service, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    serviceName(service)
{
    qDebug() << "Adaptor constructor";

    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

Adaptor::~Adaptor()
{
    qDebug() << "Adaptor destructor";
}

void Adaptor::sendRequest()
{
    qDebug() << "sendRequest";

    QString pamRUser = qgetenv("PAM_RUSER");
    QString pamRHost = qgetenv("PAM_RHOST");
    QString pamUser = qgetenv("PAM_USER");
    QString pamType = qgetenv("PAM_TYPE");
    QString pamService = qgetenv("PAM_SERVICE");
    QString pamTty = qgetenv("PAM_TTY");

    timer->start(5000);
    Q_EMIT requestAccess(pamRUser, pamRHost, pamUser, pamType, pamService, pamTty, serviceName);

    qDebug() << "timer" << timer->isActive();
}

void Adaptor::ping()
{
    qDebug() << "ping";

    timer->stop();
}

void Adaptor::onTimeout()
{
    qDebug() << "onTimeout";

    accessResult(0, "Timeout");
}

void Adaptor::accessResult(int code, const QString &message)
{
    qDebug() << "accessResult" << code << message;

    if (message.length() > 0) {
        QTextStream out(stdout);
        out << message;
    }

    QCoreApplication::exit(code);
}

