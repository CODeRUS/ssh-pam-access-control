#ifndef ADAPTOR_H
#define ADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QTimer>

class Adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.coderus.pam.sshd")

public:
    explicit Adaptor(const QString &service, QObject *parent = 0);
    virtual ~Adaptor();
    void sendRequest();

signals:
    void requestAccess(const QString &pamRUser, const QString &pamRHost, const QString &pamUser,
                       const QString &pamType, const QString &pamService, const QString &pamTty,
                       const QString &service);

public slots:
    void ping();
    void accessResult(int code, const QString &message = QString());

private slots:
    void onTimeout();

private:
    QString serviceName;
    QTimer *timer;

};

#endif // ADAPTOR_H
