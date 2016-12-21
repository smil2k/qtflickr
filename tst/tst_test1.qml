import QtQuick 2.3
import QtTest 1.0
import "../app/js/flickr.js" as Flickr
import FlickrHelper 1.0
import Social 1.0

TestCase {
    name: "InfraTests"

    OAuth {
        id: "flickr"
        consumerKey: 'fb1a4db841af7d7c04a0da65081e8e26'
        consumerSecret: '524a496d5f624f28'

        onRequestTokenReceived: function (map, string) {
            console.log(string);
        }
    }

    function test_FlickrApi_Sig() {
        var a = {
            oauth_callback: "http://www.example.com",
            oauth_consumer_key: "653e7a6ecc1d528c516cc8f92cf98611",
            oauth_nonce: "95613465",
            oauth_signature_method: "HMAC-SHA1",
            oauth_timestamp: "1305586162",
            oauth_version: "1.0"
         };

        compare(Flickr.calculateHMAC("GET","https://www.flickr.com/services/oauth/request_token", Flickr._formQueryString(a), "653e7a6ecc1d528c516cc8f92cf98611","maci"),
                "PXtROW/Qdi3Y8WCL3G+tl/cqOco=",
                "apisig is ok");
    }

    function test_sha1HMAC() {
        var s = CryptoHelper.hmacSha1Base64(
                    'fb1a4db841af7d7c04a0da65081e8e26&524a496d5f624f28',
                    'GET&https%3A%2F%2Fwww.flickr.com%2Fservices%2Foauth%2Frequest_token&oauth_callback%3Dhttp%253A%252F%252Fpurevirtual.thanks.for.calling%252F%26oauth_consumer_key%3Dfb1a4db841af7d7c04a0da65081e8e26%26oauth_nonce%3D1482321561165%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1482321561165%26oauth_version%3D1.0');
        compare(s, "oPRyJogntS5H23kKzIjef9AoGi4=", "sha1 b64");
    }

    function test_FlickrApi_callNative() {
        var data = {
            'oauth_callback': "http://trust.me.its.ok/"
        }

        flickr.requestToken("GET", "https://www.flickr.com/services/oauth/request_token", data);

        wait(5000);

        console.log("Hello?!");
    }

   /* function test_FlickrApi_call() {
        var f = new Flickr.Flickr("fb1a4db841af7d7c04a0da65081e8e26", "524a496d5f624f28")
        var err = 0;
        f.requestToken(function(t) {

        }, function() {
            err = 1;
        }
        );

        wait(5000);

        if (err) {
            fail("error!")
        }
    }*/
}
