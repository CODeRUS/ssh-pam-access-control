import QtQuick 2.1
import Sailfish.Silica 1.0

Page {
    id: page

    PageHeader {
        id: header
        anchors.top: page.top
        title: qsTr("Incoming SSH connection")
    }

    SilicaFlickable {
        anchors {
            fill: parent
            topMargin: header.height
            bottomMargin: buttonsRow.height + messageField.height
        }

        contentHeight: column.height

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }

            spacing: Theme.paddingLarge

            Label {
                width: parent.width
                wrapMode: Text.Wrap
                text: pamEnv.pamRUser.toString().length > 0 ? "Remote: %1@%2".arg(pamEnv.pamRUser).arg(pamEnv.pamRHost)
                                                            : "Remote: %1".arg(pamEnv.pamRHost)
            }

            Label {
                width: parent.width
                wrapMode: Text.Wrap
                text: "Local user: %1".arg(pamEnv.pamUser)
            }

            Label {
                width: parent.width
                wrapMode: Text.Wrap
                text: "Connection: %1".arg(pamEnv.pamTty)
            }
        }
    }

    TextField {
        id: messageField
        anchors.bottom: buttonsRow.top
        width: parent.width
        placeholderText: "Optional message"
    }

    Item {
        id: buttonsRow
        anchors.bottom: page.bottom
        width: parent.width
        height: Theme.itemSizeSmall

        Button {
            text: "Allow"
            anchors.right: parent.horizontalCenter
            width: parent.width / 2
            onClicked: appWindow.accessResult(0, messageField.text)
        }

        Button {
            text: "Deny"
            anchors.left: parent.horizontalCenter
            width: parent.width / 2
            onClicked: appWindow.accessResult(1, messageField.text)
        }
    }
}


