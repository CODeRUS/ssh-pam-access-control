#ifndef ENVSTORE_H
#define ENVSTORE_H

#include <QObject>
#include <QVariantMap>

class EnvStore : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString pamRUser MEMBER m_pamRUser NOTIFY pamRUserChanged)
    Q_PROPERTY(QString pamRHost MEMBER m_pamRHost NOTIFY pamRHostChanged)
    Q_PROPERTY(QString pamUser MEMBER m_pamUser NOTIFY pamUserChanged)
    Q_PROPERTY(QString pamType MEMBER m_pamType NOTIFY pamTypeChanged)
    Q_PROPERTY(QString pamService MEMBER m_pamService NOTIFY pamServiceChanged)
    Q_PROPERTY(QString pamTty MEMBER m_pamTty NOTIFY pamTtyChanged)
    Q_PROPERTY(QString service MEMBER m_service NOTIFY serviceChanged)

    explicit EnvStore(QObject *parent = 0);
    void fromConnection(const QVariantMap &connection);

signals:
    void pamRUserChanged();
    void pamRHostChanged();
    void pamUserChanged();
    void pamTypeChanged();
    void pamServiceChanged();
    void pamTtyChanged();
    void serviceChanged();

private:
    QString m_pamRUser;
    QString m_pamRHost;
    QString m_pamUser;
    QString m_pamType;
    QString m_pamService;
    QString m_pamTty;
    QString m_service;

};

#endif // ENVSTORE_H
