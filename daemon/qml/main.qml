import QtQuick 2.1
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { EmptyPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    signal accessResult(int code, string message)
}


