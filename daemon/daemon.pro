TARGET = ssh-pam-access-daemon
target.path = /usr/bin

QT += quick qml dbus
CONFIG += link_pkgconfig
PKGCONFIG += sailfishapp mlite5
INCLUDEPATH += /usr/include/mlite5
INCLUDEPATH += /usr/include/sailfishapp

SOURCES += \
    src/main.cpp \
    src/dbuswatcher.cpp \
    src/envstore.cpp

HEADERS = \
    src/dbuswatcher.h \
    src/envstore.h

notification.files = notification/org.coderus.pam.sshd.pending.conf \
                     notification/org.coderus.pam.sshd.notify.conf
notification.path = /usr/share/lipstick/notificationcategories/

events.files = events/ssh_pam_access_control_pending.ini \
               events/ssh_pam_access_control_notify.ini
events.path = /usr/share/ngfd/events.d/

profiled.files = profiled/60.ssh-pam-access-control.ini
profiled.path = /etc/profiled

systemd.files = systemd/ssh-pam-access-control.service
systemd.path = /usr/lib/systemd/user

icons.files = icons/notification.png icons/settings.png
icons.path = /usr/share/ssh-pam-access-daemon/icons

sounds.files = sounds/confirmation-tone.wav \
               sounds/notification-tone.wav
sounds.path = /usr/share/ssh-pam-access-daemon/sounds

settings.files = settings/ssh-security-settings.json
settings.path = /usr/share/jolla-settings/entries/

qml.files = qml/*
qml.path = /usr/share/ssh-pam-access-daemon/qml

INSTALLS = target notification events profiled systemd icons sounds qml settings

OTHER_FILES += \
    qml/main.qml \
    qml/settings/main.qml \
    qml/pages/MainPage.qml \
    qml/cover/CoverPage.qml
