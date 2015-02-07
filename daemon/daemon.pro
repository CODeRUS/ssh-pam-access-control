TARGET = ssh-pam-access-daemon

QT += dbus
CONFIG += sailfishapp link_pkgconfig
PKGCONFIG += sailfishapp mlite5
INCLUDEPATH += /usr/include/mlite5

SOURCES += \
    src/main.cpp \
    src/dbuswatcher.cpp

HEADERS = \
    src/dbuswatcher.h

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

icons.files = icons/notification.png
icons.path = /usr/share/ssh-pam-access-daemon/icons

sounds.files = sounds/pending.wav \
               sounds/notify.wav
sounds.path = /usr/share/ssh-pam-access-daemon/sounds

INSTALLS += notification events profiled systemd icons sounds

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/main.qml
