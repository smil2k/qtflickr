import QtQuick 2.3
import QtTest 1.0
import "../app/js/md5.js" as MD5
import "../app/js/flickr.js" as Flickr

TestCase {
    name: "InfraTests"

    function test_md5hash() {
        compare(MD5.md5("555555555api_key11111111methodflickr.auth.getFrob"), "603697fc6f7e3f5312ef163eb11463c7", "md5 is ok");
    }

    function test_FlickrApi_Sig() {
        var a = {
            "format": 'json',
            api_key: '11111111',
            method: 'flickr.auth.getFrob',
            api_sig: 'bla'
         };

        compare(Flickr.calculateHash(a, "555555555"),
                "603697fc6f7e3f5312ef163eb11463c7",
                "md5 is ok");
    }
}
