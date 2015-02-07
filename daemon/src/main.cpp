#include <QGuiApplication>
#include <QScopedPointer>

#include "sailfishapp.h"
#include "dbuswatcher.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<DBusWatcher> dbus(new DBusWatcher(app.data()));

    return app->exec();
}

