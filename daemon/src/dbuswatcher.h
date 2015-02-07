#ifndef DBUSWATCHER_H
#define DBUSWATCHER_H

#include <QObject>

class DBusWatcher : public QObject
{
    Q_OBJECT
public:
    explicit DBusWatcher(QObject *parent = 0);

signals:

public slots:

};

#endif // DBUSWATCHER_H
