import QtQuick 2.4
import QtQuick.Dialogs 1.2
import QtWebView 1.0

Dialog {
    standardButtons: StandardButton.Close | StandardButton.Retry
    width: 600
    height: 400

    WebView {
        id: browser
        anchors.fill: parent

        property string trigger: "test"
        property var callback

        onLoadProgressChanged: function() {
            if (url && url.toString().indexOf(trigger) != -1) {
                close();
                if ( callback ) {
                    callback(url);
                }
            }
        }
    }

    function openDialog( url, trigger, callback ) {
        browser.trigger = trigger
        browser.callback = callback;
        browser.url = url;
        open();
    }
}
