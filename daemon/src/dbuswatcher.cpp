#include "dbuswatcher.h"

#include <QDBusConnection>
#include <QDBusInterface>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickItem>

#include <QDebug>

#include <mlite5/MNotification>

#include "sailfishapp.h"

DBusWatcher::DBusWatcher(QObject *parent) :
    QObject(parent)
{
    qDebug() << "DBusWatcher constructor";

    dconf.reset(new MGConfItem("/apps/ssh-pam-access-control/allow", this));

    QDBusConnection::sessionBus().connect("", "/", "org.coderus.pam.sshd", "requestAccess", this,
                                          SLOT(onRequestAccess(QString,QString,QString,QString,QString,QString,QString)));
}

DBusWatcher::~DBusWatcher()
{
    qDebug() << "DBusWatcher destructor";

    QDBusConnection::sessionBus().disconnect("", "/", "org.coderus.pam.sshd", "requestAccess", this,
                                             SLOT(onRequestAccess(QString,QString,QString,QString,QString,QString,QString)));

    for (int i = 0; i < pendingConnections.length(); i++) {
        QVariantMap connection = pendingConnections.at(i).toMap();
        sendResult(0, connection["service"].toString(), "Destruction");
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

    notifyConnection(connection, !dconf->value(false).toBool());

    if (!dconf->value(false).toBool()) {
        pendingConnections.append(connection);

        if (pendingConnections.length() == 1) {
            showDialog();
        }
    }
    else {
        sendResult(0, service);
    }
}

void DBusWatcher::processResult(int code, const QString &message)
{
    if (pendingConnections.length() > 0) {
        QVariant connVar = pendingConnections.takeFirst();
        QVariantMap connection = connVar.toMap();

        sendResult(code, connection["service"].toString(), message);

        if (view) {
            qDebug() << "destroy old view";
            view->destroy();
            view = NULL;
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

    if (view) {
        qDebug() << "destroy";

        view->destroy();
        view = NULL;

        processResult(1);
    }
}

void DBusWatcher::viewDestroyed()
{
    qDebug() << "viewDestroyed";

    view = NULL;
}

void DBusWatcher::showDialog()
{
    if (pendingConnections.length() > 0) {
        qDebug() << "showDialog";
        view = SailfishApp::createView();
        //view = new QQuickView();
        qDebug() << "connect events";
        QObject::connect(view, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onClosing(QQuickCloseEvent*)));
        QObject::connect(view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
        qDebug() << "set context";
        view->rootContext()->setContextProperty("pamEnv", pendingConnections.at(0));
        qDebug() << "set source";
        view->setSource(SailfishApp::pathTo("qml/main.qml"));
        QObject *item = view->rootObject();
        QObject::connect(item, SIGNAL(accessResult(int, QString)), this, SLOT(processResult(int, QString)));
        qDebug() << "show";
        view->show();
    }
}

void DBusWatcher::notifyConnection(const QVariantMap &connection, bool pending)
{
    MNotification notification(QString("org.coderus.pam.sshd.%1").arg(pending ? "pending" : "notify"));
    notification.setSummary(QString("Incoming: %1").arg(connection["pamRHost"].toString()));
    notification.setBody(QString("Requested login: %1").arg(connection["pamUser"].toString()));
    notification.setImage("/usr/share/ssh-pam-access-daemon/icons/notification.png");
    notification.publish();
}
