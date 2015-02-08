import QtQuick 2.1
import Sailfish.Silica 1.0
import Sailfish.Media 1.0
import com.jolla.settings.system 1.0
import org.nemomobile.systemsettings 1.0
import org.nemomobile.configuration 1.0
import org.nemomobile.dbus 2.0

Page {
    id: page

    ConfigurationGroup {
        id: dconf
        path: "/apps/ssh-pam-access-control"
        property bool allowAuto: false
        property bool allowedIp: []
    }

    DBusInterface {
        id: profiled
        service: "com.nokia.profiled"
        path: "/com/nokia/profiled"
        iface: "com.nokia.profiled"

        property string profileName: "general"
        property string pendingAlertKey: "ssh.pending.alert"
        property string notifyAlertKey: "ssh.notify.alert"
        property string pendingEnabledKey: "ssh.pending.enabled"
        property string notifyEnabledKey: "ssh.notify.enabled"

        property string pendingAlertTone: ""
        property string notifyAlertTone: ""
        property bool pendingAlertEnabled: true
        property bool notifyAlertEnabled: true

        Component.onCompleted: {
            profiled.typedCall("get_value",
                               [
                                   {"type": "s", "value": profileName},
                                   {"type": "s", "value": pendingEnabledKey}
                               ],
                               function (result) {
                                   pendingAlertEnabled = result;
                               });
            profiled.typedCall("get_value",
                               [
                                   {"type": "s", "value": profileName},
                                   {"type": "s", "value": notifyEnabledKey}
                               ],
                               function (result) {
                                   notifyAlertEnabled = result;
                               });
            profiled.typedCall("get_value",
                               [
                                   {"type": "s", "value": profileName},
                                   {"type": "s", "value": pendingAlertKey}
                               ],
                               function (result) {
                                   pendingAlertTone = result;
                               });
            profiled.typedCall("get_value",
                               [
                                   {"type": "s", "value": profileName},
                                   {"type": "s", "value": notifyAlertKey}
                               ],
                               function (result) {
                                   notifyAlertTone = result;
                               });
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height
        interactive: contentHeight > height

        Column {
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: "SSH Access confirmation"
            }

            TextSwitch {
                width: parent.width
                text: "Don't ask for confirmation, only notify"
                checked: dconf.allowAuto
                onClicked: dconf.allowAuto = checked
            }

            ValueButton {
                label: "Confirmation required tone"
                value: profiled.pendingAlertTone ? metadataReader.getTitle(profiled.pendingAlertTone) : "no sound"
                onClicked: {
                    var dialog = pageStack.push(dialogComponent, {
                        activeFilename: profiled.pendingAlertTone,
                        activeSoundTitle: value,
                        activeSoundSubtitle: "Confirmation required tone",
                        noSound: !profiled.pendingAlertEnabled
                        })

                    dialog.accepted.connect(
                        function() {
                            profiled.pendingAlertEnabled = !dialog.noSound
                            profiled.typedCall("set_value",
                                               [
                                                   {"type": "s", "value": profiled.profileName},
                                                   {"type": "s", "value": profiled.pendingEnabledKey},
                                                   {"type": "s", "value": profiled.pendingAlertEnabled ? "On" : "Off"}
                                               ])
                            if (profiled.pendingAlertEnabled) {
                                profiled.pendingAlertTone = dialog.selectedFilename
                                profiled.typedCall("set_value",
                                                   [
                                                       {"type": "s", "value": profiled.profileName},
                                                       {"type": "s", "value": profiled.pendingAlertKey},
                                                       {"type": "s", "value": profiled.pendingAlertTone}
                                                   ])
                            }
                        })
                }
            }

            ValueButton {
                label: "Automatically accepted tone"
                value: profiled.notifyAlertTone ? metadataReader.getTitle(profiled.notifyAlertTone) : "no sound"
                onClicked: {
                    var dialog = pageStack.push(dialogComponent, {
                        activeFilename: profiled.notifyAlertTone,
                        activeSoundTitle: value,
                        activeSoundSubtitle: "Automatically accepted tone",
                        noSound: !profiled.notifyAlertEnabled
                        })

                    dialog.accepted.connect(
                        function() {
                            profiled.notifyAlertEnabled = !dialog.noSound
                            profiled.typedCall("set_value",
                                               [
                                                   {"type": "s", "value": profiled.profileName},
                                                   {"type": "s", "value": profiled.notifyEnabledKey},
                                                   {"type": "s", "value": profiled.notifyAlertEnabled ? "On" : "Off"}
                                               ])
                            if (profiled.notifyAlertEnabled) {
                                profiled.notifyAlertTone = dialog.selectedFilename
                                profiled.typedCall("set_value",
                                                   [
                                                       {"type": "s", "value": profiled.profileName},
                                                       {"type": "s", "value": profiled.notifyAlertKey},
                                                       {"type": "s", "value": profiled.notifyAlertTone}
                                                   ])
                            }
                        })
                }
            }
        }
    }

    AlarmToneModel {
        id: alarmToneModel

        Component.onCompleted: {
            myToneModel.append({"title": "confirmation-tone", "filename": "/usr/share/ssh-pam-access-daemon/sounds/confirmation-tone.wav"})
            myToneModel.append({"title": "notification-tone", "filename": "/usr/share/ssh-pam-access-daemon/sounds/notification-tone.wav"})
            for (var i = 0; i < count; i++) {
                myToneModel.append(get(i))
            }
        }
    }

    ListModel {
        id: myToneModel
    }

    MetadataReader {
        id: metadataReader
    }

    Component {
        id: dialogComponent

        SoundDialog {
            alarmModel: myToneModel
        }
    }
}
