TARGET = ssh-pam-access-daemon

QT += gui-private dbus
CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/dbuswatcher.cpp

HEADERS = \
    src/dbuswatcher.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/main.qml
