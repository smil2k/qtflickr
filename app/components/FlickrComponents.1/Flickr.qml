import QtQuick 2.0
import Social 1.0

OAuth {
    consumerKey: 'fb1a4db841af7d7c04a0da65081e8e26'
    consumerSecret: '524a496d5f624f28'
    callbackUrl: 'http://trust.me.its.ok/'

    property string permission: "read";

    property string userId;
    property string userName;
    property string fullname;

    onRequestTokenReceived: function(map, string) {
        console.log("Success");
        console.log(map.oauth_token);

        oauthTokenSecret = map.oauth_token_secret;

        accessDialog.openDialog(
                    "https://www.flickr.com/services/oauth/authorize?perms="+permission+"&oauth_token=" + map.oauth_token,
                    "oauth_verifier",
                    verifierReceived);

    }

    function verifierReceived(url) {
        var query = url.toString().replace(/^.*\?/,"");
        var response = parseRestResponse(query);
        oauthToken = response.oauth_token;
        var verifier = response.oauth_verifier;

        callbackUrl = '';

        accessToken("GET", "https://www.flickr.com/services/oauth/access_token",
                    oauthToken,
                    oauthTokenSecret,
                    verifier);
    }

    function parseRestResponse(body) {
        if(!body) {
            return false;
        }
        console.log("query:"+body);
        var constituents = body.split("&"),
                response = {};

        constituents.forEach(function(pair) {
            var keyval = pair.split("=");
            console.log(keyval[0] + "=" + keyval[1]);
            response[keyval[0]] = decodeURIComponent(keyval[1]);
        });
        return response;
    }

    onAccessTokenReceived: function(map, data ) {
        userId = decodeURIComponent(map.user_nsid);
        userName = decodeURIComponent(map.username);
        fullname = decodeURIComponent(map.fullname);
    }

    property var accessDialog: AccessPrompt {
    }

    function initialize(readyCallback, errorCallback) {
        console.log("Requesing token...");
        callbackUrl = 'http://trust.me.its.ok/';
        requestToken("GET", "https://www.flickr.com/services/oauth/request_token");
    }
}
