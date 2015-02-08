import QtQuick 2.1
import Sailfish.Silica 1.0

CoverBackground {
    Column {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: Theme.paddingLarge
        }
        spacing: Theme.paddingSmall

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: "Incoming SSH connection"
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: pamEnv.pamRUser.toString().length > 0 ? "Remote: %1@%2".arg(pamEnv.pamRUser).arg(pamEnv.pamRHost)
                                                        : "Remote: %1".arg(pamEnv.pamRHost)
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: "Local user: %1".arg(pamEnv.pamUser)
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: "Connection: %1".arg(pamEnv.pamTty)
            font.pixelSize: Theme.fontSizeSmall
        }
    }

    CoverActionList {
        id: coverAction
        enabled: true

        CoverAction {
            iconSource: "image://theme/icon-cover-play"
            onTriggered: appWindow.accessResult(0, "")
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
            onTriggered: appWindow.accessResult(1, "")
        }
    }
}


