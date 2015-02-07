#include <QCoreApplication>
#include <QScopedPointer>
#include <QTimer>

#include "dbusmain.h"

int main(int argc, char *argv[])
{
    if (qgetenv("PAM_TYPE") == "close_session") {
        return 0;
    }

    qputenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/user/100000/dbus/user_bus_socket");

    QScopedPointer<QCoreApplication> app(new QCoreApplication(argc, argv));
    QScopedPointer<DBusMain> dbus(new DBusMain(app.data()));
    QTimer::singleShot(100, dbus.data(), SLOT(start()));

    return app->exec();
}

