#include <QGuiApplication>
#include <QScopedPointer>

#include "sailfishapp.h"
#include "dbuswatcher.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
    app->setQuitOnLastWindowClosed(false);
    QScopedPointer<DBusWatcher> dbus(new DBusWatcher(app.data()));

    return app->exec();
}

