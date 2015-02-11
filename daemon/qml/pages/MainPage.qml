import QtQuick 2.1
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

Dialog {
    id: page
    objectName: "mainPage"

    onAccepted: {
        appWindow.accessResult(0, messageField.text)
    }

    onRejected: {
        appWindow.accessResult(1, messageField.text)
    }

    ConfigurationValue {
        id: allowedHosts
        key: "/apps/ssh-pam-access-control/allowedHosts"
        defaultValue: []
    }

    SilicaFlickable {
        width: parent.width
        height: column.height
        contentHeight: column.height
        pressDelay: 0

        PullDownMenu {
            MenuItem {
                text: "Add to whitelist"
                onClicked: {
                    var val = allowedHosts.value
                    if (val.indexOf(pamEnv.pamRHost) == -1) {
                        val.splice(0, 0, pamEnv.pamRHost)
                        allowedHosts.value = val
                    }
                    page.accept()
                }
            }
        }

        Column {
            id: column

            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                id: header
                title: qsTr("Incoming SSH connection")
            }

            Flickable {
                width: parent.width
                contentHeight: inner.height
                interactive: height < inner.height
                height: page.height - header.height - Theme.paddingLarge
                boundsBehavior: Flickable.StopAtBounds
                pressDelay: 0
                clip: true

                Column {
                    id: inner
                    width: parent.width
                    spacing: Theme.paddingLarge

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: Theme.paddingLarge
                        }
                        wrapMode: Text.Wrap
                        text: pamEnv.pamRUser.toString().length > 0 ? "Remote: %1@%2".arg(pamEnv.pamRUser).arg(pamEnv.pamRHost)
                                                                    : "Remote: %1".arg(pamEnv.pamRHost)
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: Theme.paddingLarge
                        }
                        wrapMode: Text.Wrap
                        text: "Local user: %1".arg(pamEnv.pamUser)
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: Theme.paddingLarge
                        }
                        wrapMode: Text.Wrap
                        text: "Connection: %1".arg(pamEnv.pamTty)
                    }

                    TextField {
                        id: messageField
                        width: parent.width
                        placeholderText: "Optional message"
                        EnterKey.enabled: true
                        EnterKey.iconSource: "image://theme/icon-m-enter-next"
                        EnterKey.onClicked: focus = false
                    }
                }
            }
        }
    }
}


