import QtQuick 2.1
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { FirstPage { } }
    cover: undefined
    property bool done: false
    signal accessResult(int code, string message)
    function sendResult(code, message) {
        done = true
        accessResult(code, message)
    }
}


