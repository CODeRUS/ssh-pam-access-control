#include "adaptor.h"

#include <QCoreApplication>
#include <QTextStream>

Adaptor::Adaptor(const QString &service, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    serviceName(service)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

Adaptor::~Adaptor()
{
}

void Adaptor::sendRequest()
{
    QString pamRUser = qgetenv("PAM_RUSER");
    QString pamRHost = qgetenv("PAM_RHOST");
    QString pamUser = qgetenv("PAM_USER");
    QString pamType = qgetenv("PAM_TYPE");
    QString pamService = qgetenv("PAM_SERVICE");
    QString pamTty = qgetenv("PAM_TTY");

    timer->start(3000);
    Q_EMIT requestAccess(pamRUser, pamRHost, pamUser, pamType, pamService, pamTty, serviceName);
}

void Adaptor::ping()
{
    timer->stop();
}

void Adaptor::onTimeout()
{
    accessResult(0, "Timeout");
}

void Adaptor::accessResult(int code, const QString &message)
{
    if (message.length() > 0) {
        QTextStream out(stdout);
        out << message << "\n";
    }

    QCoreApplication::exit(code);
}

