import QtQuick 2.3
import QtTest 1.0
import "../app/components/FlickrComponents.1"

TestCase {
    name: "InfraTests"

    Flickr {
        id: f
    }

    function test_FlickrApi_call() {
        f.initialize();

        wait(5000);

        console.log("Hello?!");
    }
}
