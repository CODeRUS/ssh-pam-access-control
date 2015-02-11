#ifndef DBUSWATCHER_H
#define DBUSWATCHER_H

#include <QObject>
#include <QQuickView>

#include <mlite5/MGConfItem>
#include <mlite5/MNotification>

#include "envstore.h"

class DBusWatcher : public QObject
{
    Q_OBJECT
public:
    explicit DBusWatcher(QObject *parent = 0);
    virtual ~DBusWatcher();

signals:

private slots:
    void onRequestAccess(const QString &pamRUser, const QString &pamRHost, const QString &pamUser,
                         const QString &pamType, const QString &pamService, const QString &pamTty,
                         const QString &service);

private slots:
    void processResult(int code, const QString &message = QString());
    void sendResult(int code, const QString &service, const QString &message = QString());
    void onClosing(QQuickCloseEvent *);
    void viewDestroyed();

private:
    void showDialog();
    void notifyConnection(const QVariantMap &connection, bool pending = true);

    EnvStore *envStore;
    QVariantList pendingConnections;
    QQuickView *view;
    QScopedPointer<MGConfItem> autoAllow;
    QScopedPointer<MGConfItem> allowedHosts;
    QHash<QString, MNotification*> pendingNotifications;
};

#endif // DBUSWATCHER_H
