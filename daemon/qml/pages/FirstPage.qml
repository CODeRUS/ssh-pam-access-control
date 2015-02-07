import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    PageHeader {
        id: header
        anchors.top: page
        title: qsTr("Incoming SSH connection")
    }

    SilicaFlickable {
        anchors {
            fill: parent
            topMargin: header.height
            bottomMargin: buttonsRow.height
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
                text: "Remote host: " + pamRHost
            }

            Label {
                width: parent.width
                wrapMode: Text.Wrap
                text: "Remote user: " + pamUser
            }
        }
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
            onClicked: apphelper.exit(0)
        }

        Button {
            text: "Deny"
            anchors.left: parent.horizontalCenter
            width: parent.width / 2
            onClicked: apphelper.exit(1)
        }
    }
}


