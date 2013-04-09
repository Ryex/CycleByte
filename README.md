# CycleByte


A shifting Symmetric Encryption that uses the previous chunk of data in the encryption of the next.

It uses an implementation of SHA1 and the Mersenne Twister pseudorandom number generator


## Usage

### Encryption


Step 1) create an encryption
 
the chunk_size is the maximum number of bytes in a copntinues section of encryption. is is sugested to set this size to be about 1/4 to 1/16 the size
of the data to be encrypted. the smaller the number the more ties the encryption will change, the slower the encryption will happen, and the more theroeticly secure the data is.

be sure to use the same `max_chunk_size` to encrypt and decrypt a message

    char* key = "IAmAnEncryptionKeyChangeMe";
    long key_size = strlen(key);
    int max_chunk_size = 10;
    CycleByte cyc(key, key_size, max_chunk_size);

Step 2) encrypt some data

note that `data` can be anything as your just casting it to an array of unsigned chars and reading the raw memory

a copy encryption, you will need an array the same size as the data to be encrypted 


    char data[] = ";alksdhfp;oqnl;wkjnekjbqca;opsidfbqwlke"
    char result[40]
    int len = strlen(data);
    cyc.encrypt(static_cast<unsigned char*>data, result, len);


an inplace encryption, your array will be modifyed to contain the encrypted version 
(WARNING: DONT USE INPLACE ENCRYPTION ON AN OBJECT YOU WILL NEED AFTER AS IT WILL BE CORUPTED!) 


    char data[] = ";alksdhfp;oqnl;wkjnekjbqca;opsidfbqwlke"
    int len = strlen(data);
    cyc.encrypt_inplace(static_cast<unsigned char*>data, len);


### Decryption
Step 1) create an encryption 

the chunk_size is the maximum number of bytes in a copntinues section of encryption. is is sugested to set this size to be about 1/4 to 1/16 the size
of the data to be encrypted. the smaller the number the more ties the encryption will change, the slower the encryption will happen, and the more theroeticly secure the data is.

be sure to use the same `max_chunk_size` to encrypt and decrypt a message


    char* key = "IAmAnEncryptionKeyChangeMe";
    long key_size = strlen(key);
    int max_chunk_size = 10;
    CycleByte cyc(key, key_size, max_chunk_size);


Step 2) decrypt some data

note that `data` can be anything as your just casting it to an array of unsigned chars and reading the raw memory

a copy decryption, you will need an array the same size as the data to be decrypted 

    // encrypted data can use non printable charaters to lets encrypt some data then decrypt it
    char data[] = ";alksdhfp;oqnl;wkjnekjbqca;opsidfbqwlke"
    char result[40]
    int len = strlen(data);
    cyc.encrypt(static_cast<unsigned char*>data, result, len);

    // now to decrypt
    char decrypted[40]
    cyc.decrypt(static_cast<unsigned char*>result, decrypted, len)


an inplace encryption, your array will be modifyed to contain the encrypted version 
(WARNING: DONT USE INPLACE DECRYPTION ON AN OBJECT YOU WILL NEED AFTER AS IT WILL BE CORUPTED!) 


    // encrypted data can use non printable charaters to lets encrypt some data then decrypt it
    char data[] = ";alksdhfp;oqnl;wkjnekjbqca;opsidfbqwlke"
    int len = strlen(data);
    cyc.encrypt_inplace(static_cast<unsigned char*>data, len);

    //inspect your data array, it is now encrypted

    // now to decrypt
    char decrypted[40]
    cyc.decrypt_inplace(static_cast<unsigned char*>data, len)
