#include "dbuswatcher.h"

#include <QDBusConnection>
#include <QDBusInterface>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickItem>

#include <QDebug>

#include "sailfishapp.h"

DBusWatcher::DBusWatcher(QObject *parent) :
    QObject(parent)
{
    qDebug() << "DBusWatcher constructor";

    autoAllow.reset(new MGConfItem("/apps/ssh-pam-access-control/allowAuto", this));
    allowedHosts.reset(new MGConfItem("/apps/ssh-pam-access-control/allowedHosts", this));

    view = NULL;

    QDBusConnection::sessionBus().connect("", "/", "org.coderus.pam.sshd", "requestAccess", this,
                                          SLOT(onRequestAccess(QString,QString,QString,QString,QString,QString,QString)));

    envStore = new EnvStore(this);
}

DBusWatcher::~DBusWatcher()
{
    qDebug() << "DBusWatcher destructor";

    QDBusConnection::sessionBus().disconnect("", "/", "org.coderus.pam.sshd", "requestAccess", this,
                                             SLOT(onRequestAccess(QString,QString,QString,QString,QString,QString,QString)));

    for (int i = 0; i < pendingConnections.length(); i++) {
        QVariantMap connection = pendingConnections.at(i).toMap();
        QString service = connection["service"].toString();
        sendResult(0, service, "Destruction");

        MNotification *notification = pendingNotifications.take(service);
        if (notification->isPublished()) {
            notification->remove();
        }
    }
}

void DBusWatcher::onRequestAccess(const QString &pamRUser, const QString &pamRHost, const QString &pamUser,
                                  const QString &pamType, const QString &pamService, const QString &pamTty,
                                  const QString &service)
{
    qDebug() << "onRequestAccess" << service;
    qDebug() << pamRUser << pamRHost;
    qDebug() << pamUser << pamTty;
    qDebug() << pamService << pamType;

    QDBusInterface iface(service, "/", "org.coderus.pam.sshd");
    iface.call(QDBus::NoBlock, "ping");

    QVariantMap connection;
    connection["pamRUser"] = pamRUser;
    connection["pamRHost"] = pamRHost;
    connection["pamUser"] = pamUser;
    connection["pamType"] = pamType;
    connection["pamService"] = pamService;
    connection["pamTty"] = pamTty;
    connection["service"] = service;

    QStringList allowed = allowedHosts->value().toStringList();
    if (allowed.contains(pamRHost)) {
        sendResult(0, service);
    }
    else {
        bool allow = autoAllow->value(false).toBool();
        notifyConnection(connection, !allow);

        if (allow) {
            sendResult(0, service);
        }
        else {
            pendingConnections.append(connection);

            if (pendingConnections.length() == 1) {
                showDialog();
            }
        }
    }
}

void DBusWatcher::processResult(int code, const QString &message)
{
    if (pendingConnections.length() > 0) {
        QVariant connVar = pendingConnections.takeFirst();
        QVariantMap connection = connVar.toMap();

        QString service = connection["service"].toString();

        sendResult(code, service, message);

        if (pendingNotifications.contains(service)) {
            MNotification *notification = pendingNotifications.take(service);
            if (notification->isPublished()) {
                notification->remove();
            }
            delete notification;
        }

        if (view) {
            view->hide();
            view->close();
        }

        if (pendingConnections.length() > 0) {
            qDebug() << "show new dialog";
            showDialog();
        }
    }
}

void DBusWatcher::sendResult(int code, const QString &service, const QString &message)
{
    qDebug() << "sendResult" << code << service << message;

    QDBusInterface iface(service, "/", "org.coderus.pam.sshd");
    iface.call(QDBus::NoBlock, "accessResult", code, message);
}

void DBusWatcher::onClosing(QQuickCloseEvent*)
{
    qDebug() << "onClosing";

    processResult(1);
}

void DBusWatcher::viewDestroyed()
{
    qDebug() << "viewDestroyed";

    view = NULL;
}

void DBusWatcher::showDialog()
{
    if (pendingConnections.length() > 0) {
        envStore->fromConnection(pendingConnections.at(0).toMap());
        if (!view) {
            qDebug() << "create dialog";
            view = SailfishApp::createView();
            qDebug() << "connect events";
            QObject::connect(view, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onClosing(QQuickCloseEvent*)));
            QObject::connect(view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
            qDebug() << "set context";
            view->rootContext()->setContextProperty("pamEnv", envStore);
            qDebug() << "set source";
            view->setSource(SailfishApp::pathTo("qml/main.qml"));
            QObject *item = view->rootObject();
            QObject::connect(item, SIGNAL(accessResult(int, QString)), this, SLOT(processResult(int, QString)));
        }
        else {
            qDebug() << "create view";
            view->create();
        }
        qDebug() << "show dialog";
        view->show();
    }
}

void DBusWatcher::notifyConnection(const QVariantMap &connection, bool pending)
{
    MNotification *notification = new MNotification(QString("org.coderus.pam.sshd.%1").arg(pending ? "pending" : "notify"));
    notification->setSummary(QString("Incoming: %1").arg(connection["pamRHost"].toString()));
    notification->setBody(QString("Requested login: %1").arg(connection["pamUser"].toString()));
    notification->setImage("/usr/share/ssh-pam-access-daemon/icons/notification.png");
    notification->publish();
    if (pending) {
        pendingNotifications[connection["service"].toString()] = notification;
    }
}
