#ifndef PASSWORD_CRYPT_H
#define PASSWORD_CRYPT_H

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

#include <string>

std::string password_crypt(std::string plain, std::string ivString, std::string sKey) {
    using namespace CryptoPP;

    SecByteBlock iv((const unsigned char*)(ivString.data()), ivString.size());


    SecByteBlock key((const unsigned char*)(sKey.data()), sKey.size());


    std::string cipher, recovered;

    //Try Encrypt
    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
                       new StreamTransformationFilter(e,
                                                      new StringSink(cipher)
                                                      )
                       );
    }
    catch (const Exception& e)
    {
        exit(1);
    }

    std::string ciphertxt;

    //HexEncode IV
    HexEncoder encoder(new FileSink(std::cout));
    encoder.Detach(new StringSink(ivString));
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();

    //HexEncode ciphertxt
    encoder.Detach(new StringSink(ciphertxt));
    encoder.Put((const byte*)&cipher[0], cipher.size());
    encoder.MessageEnd();

    std::transform(ciphertxt.begin(), ciphertxt.end(), ciphertxt.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return ciphertxt;
}


#endif // PASSWORD_CRYPT_H
