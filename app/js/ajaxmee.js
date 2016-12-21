.pragma library

function request(method, url, params, successCallback, errorCallback) {

    var size = function(ar) {
        var len = ar.length ? --ar.length : -1;
        for (var k in ar) {
            len++;
        }
        return len;
    }

    var serialize = function(obj, prefix) {
        var str = [];
        for(var p in obj) {
            var k = prefix ? prefix + "[" + p + "]" : p, v = obj[p];
            str.push(typeof v == "object" ?
                         serialize(v, k) :
                         encodeURIComponent(k) + "=" + encodeURIComponent(v));
        }
        return str.join("&");
    }

    var _params = serialize(params)
    var doc = new XMLHttpRequest();
    console.log(method + " " + url);
    if (method === 'GET' && _params !== '') {
        url = url +'?'+ _params
        _params = ''
    }

    console.log(url);

    doc.onreadystatechange = function() {
        if (doc.readyState === XMLHttpRequest.HEADERS_RECEIVED) {
            var status = doc.status;
            if(status!=200) {
                var contentType = doc.getResponseHeader("Content-Type");
                errorCallback(doc);
            }
        } else if (doc.readyState === XMLHttpRequest.DONE) {
            var contentType = doc.getResponseHeader("Content-Type");
            if(status>=200 && status < 300) {
                successCallback(doc);
            } else {
                errorCallback(doc);
            }
        }
    }

    doc.open(method, url);
    /*for (var key in headers) {
            doc.setRequestHeader(String(key), String(headers[key]));
        }*/

    if (_params.length > 0) {
        doc.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        doc.setRequestHeader("Content-Length", String(_params.length));
        doc.send(_params);
    } else {
        doc.send();
    }
}
