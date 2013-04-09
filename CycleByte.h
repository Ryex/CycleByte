

#ifndef CYCLEBYTE_H
#define CYCLEBYTE_H

#ifdef CYCLEBYTE_EXPORTS
#define CYCLEBYTE_API __declspec(dllexport)
#else
#define CYCLEBYTE_API __declspec(dllimport)
#endif



// This class is exported from the CycleByte.dll

/****************************************************************************************************************************
 * CycleByte
 * An Encryption system that encrypts raw data useing a shifting encryption that uses the encrypted data from the last chunk
 * to change the encryption for the next chunk, as such the key AND the content being encrypted determin the final encryption.
 * Thus the method is resuilent both to bruteforce and packet sniffing
 ****************************************************************************************************************************/


class CYCLEBYTE_API CycleByte {
public:

	CycleByte(const char* key, size_t size_key, unsigned int max_chunk_size);
	~CycleByte();

	// TODO: add IO versions
	void encrypt(const unsigned char* in_data, unsigned char* out_data, unsigned long size);
	void decrypt(const unsigned char* in_data, unsigned char* out_data, unsigned long size);
	void encrypt_inplace(unsigned char* in_data, unsigned long size);
	void decrypt_inplace(unsigned char* in_data, unsigned long size);

private:

	unsigned char* Key;
	unsigned int KeySize;
	unsigned int ChunkSize;

	void randomise_arr(unsigned char* arr, unsigned int size, const unsigned char* key, size_t size_key);
	void init_map(unsigned char *out);

};

#endif