TARGET = ssh-pam-access-control
target.path = /usr/bin

INSTALLS = target

QT += dbus

SOURCES += \
    src/main.cpp \
    src/adaptor.cpp \
    src/dbusmain.cpp

HEADERS += \
    src/adaptor.h \
    src/dbusmain.h
