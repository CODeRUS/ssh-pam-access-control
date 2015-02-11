import QtQuick 2.1
import Sailfish.Silica 1.0

Page {
    objectName: "emptyPage"

    onStatusChanged: {
        if (status == PageStatus.Active && pageStack.depth == 1) {
            if (!pageStack.busy) {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"))
            }
            else {
                pageStack.busyChanged.connect(function() {
                    if (!pageStack.busy) {
                        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
                    }
                })
            }
        }
    }

    /*Item {
        width: parent.width
        height: Theme.itemSizeSmall
        anchors.bottom: parent.bottom

        Button {
            anchors.right: parent.horizontalCenter
            text: "Accept"
            onClicked: appWindow.accessResult(0, "")
        }

        Button {
            anchors.left: parent.horizontalCenter
            text: "Reject"
            onClicked: appWindow.accessResult(1, "")
        }
    }*/
}
