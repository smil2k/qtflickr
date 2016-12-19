import QtQuick 2.3
import QtTest 1.0
import "../app/js/md5.js" as MD5
import "../app/js/sha1.js" as SHA1
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

    function test_sha1Hash() {
        var s = new SHA1.Sha1();
        s.update("GET&https%3A%2F%2Fwww.flickr.com%2Fservices%2Foauth%2Frequest_token&oauth_callback%3Dhttp%253A%252F%252Fwww.example.com%26oauth_consumer_key%3D653e7a6ecc1d528c516cc8f92cf98611%26oauth_nonce%3D95613465%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1305586162%26oauth_version%3D1.0");
        compare(s.hex(), "5db1bc2a7d91ede509b60ed749972360220902ed", "sha1 hex");
        compare(s.base64(), "XbG8Kn2R7eUJtg7XSZcjYCIJAu0=", "sha1 b64");
    }
/*
    function test_FlickrApi_Sig() {
        var f = new Flickr.Flickr("fb1a4db841af7d7c04a0da65081e8e26", "524a496d5f624f28")
    }*/
}
