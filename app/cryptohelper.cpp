#include "cryptohelper.h"

QString CryptoHelper::hmacSha1Base64(const QString & key, const QString & data){

    QByteArray ipad;
    QByteArray opad;
    QByteArray ctx;
    QByteArray sha1;
    QByteArray k;

    k = key.toLocal8Bit();

    int keyLen = key.size();

    if(keyLen > 64){

        QByteArray tempKey;
        tempKey.append(key);
        k = QCryptographicHash::hash(tempKey, QCryptographicHash::Sha1);

        keyLen = 20;
    }

    ipad.fill( 0, 64);
    opad.fill(0, 64);

    ipad.replace(0, keyLen, k);
    opad.replace(0, keyLen, k);

    for (int i=0; i<64; i++)
    {
        ipad[i] = ipad[i] ^ 0x36;
        opad[i] = opad[i] ^ 0x5c;
    }

    ctx.append(ipad,64);
    ctx.append(data);

    sha1 = QCryptographicHash::hash(ctx, QCryptographicHash::Sha1);

    ctx.clear();
    ctx.append(opad,64);
    ctx.append(sha1);

    sha1.clear();

    sha1 = QCryptographicHash::hash(ctx, QCryptographicHash::Sha1);

    return sha1.toBase64();
}
