#ifndef DBUSMAIN_H
#define DBUSMAIN_H

#include <QObject>
#include <QDBusConnection>

class DBusMain : public QObject
{
    Q_OBJECT
public:
    explicit DBusMain(QObject *parent = 0);
    virtual ~DBusMain();

signals:

private slots:
    void start();

};

#endif // DBUSMAIN_H
