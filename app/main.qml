import QtQuick 2.7
import QtQuick.Window 2.2
import "components/FlickrComponents.1" 1.0
import Qt.labs.settings 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(qsTr('Clicked on background. "'))

            if (api.oauthToken == "" || api.oauthTokenSecret == "") {
                api.oauthTokenSecret = "";
                api.oauthToken = "";
                api.initialize()
            } else {
                console.log("Already registered.");
            }
        }
    }

    Settings {
        category: "Flickr"

        property alias accessToken: api.oauthToken
        property alias accessTokenSecret: api.oauthTokenSecret
        property alias fullname: api.fullname
        property alias userId: api.userId
        property alias userName: api.userName
    }

    Flickr {
        id: api
        permission: "delete"
    }

}
