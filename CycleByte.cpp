// CycleByte.cpp : Defines the exported functions for the DLL application.
//
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "CycleByte.h"

#include "mtrand.h"
#include "SHA1.h"



// This is the constructor of a class that has been exported.
// see CycleByte.h for the class definition
CycleByte::CycleByte(const unsigned char * key, size_t size_key, unsigned int max_chunk_size)
{
	Key = (unsigned char *)key;
	KeySize = size_key;
	ChunkSize = max_chunk_size;
	return;
}

// encryptes an array of data to the out array
void CycleByte::encrypt(const unsigned char* in_data, unsigned char* out_data, unsigned long size)
{

	// we need arrays to map the ASCII values (really just raw byte valeus we dont care if they are used as ASCII)
	unsigned char encoding[256];
	// fill the array with the values with 0-255
	init_map(encoding);
	// scramble the array predictably with the key for thsi encryption
	randomise_arr(encoding, 256, Key, KeySize);

	// SHA1 hash of key
	// generate a SHA1 of the encryption key
	CSHA1 sha1;
	sha1.Update(Key, KeySize);
	sha1.Final();
	// get the hash out
	unsigned char hash[20];
	sha1.GetHash(hash);
	// we need to convert that hash to a seed for our generator which needs to be a long or array of longs
	unsigned long hash_long[20];
	// convert the array of chars to an array of longs
	for (char n = 0; n < 20; n++)
	{
		hash_long[n] = hash[n];
	}

	// Seed the random with the key
	MTRand mt(hash_long, 20);

	// A shift point is a point in the data where we regenerate the encryption mapping
	// each continious section of one encryption mapping is know as a chunk
	// get the first shift point from the generator, needs to be the first number we pull off that key
	unsigned int shift = mt() * ChunkSize;
	// set the last shit point, it's 0 becasue we havn't shifted yet
	unsigned long last_shift = 0;
	// make a pointer to store a new encryption key
	unsigned char* new_key;

	// loop through the data
	for (unsigned long i = 0; i < size; i ++)
	{
		//if we have reached our shift point
		if (i - last_shift == shift)
		{
			// make space to store a new encryption key key
			new_key = new unsigned char[shift + KeySize];
			// copy the real key to the new key
			memcpy(new_key, Key, KeySize);
			// copy the encrypted data form the last chunk to the end of the key to make a key for the next chunk
			memcpy(&new_key[KeySize], &out_data[last_shift], shift);
			// re init the encoding map
			init_map(encoding);
			// rescrable the map with the new key for this next chunk
			randomise_arr(encoding, 256, new_key, KeySize + shift);
			// record this shift point
			last_shift += shift;
			// get the next shift point
			shift = mt() * ChunkSize;
			// delete the the generated key
            delete[] new_key;
		}
		//get the encrypted charater for this bit if the data and store it
		out_data[i] = encoding[in_data[i]];
	}

}

// decryptes an array of data to the out array
void CycleByte::decrypt(const unsigned char* in_data, unsigned char* out_data, unsigned long size) 
{
	// we need arrays to map the ASCII values (really just raw byte valeus we dont care if they are used as ASCII)
	unsigned char encoding[256];
	// fill the array with the values with 0-255
	init_map(encoding);
	// scramble the array predictably with the key for thsi encryption
	randomise_arr(encoding, 256, Key, KeySize);

    // SHA1 hash of key
	CSHA1 sha1;
	sha1.Update(Key, KeySize);
	// get the hash out
	sha1.Final();
	unsigned char hash[20];
	sha1.GetHash(hash);
	// we need to convert that hash to a seed for our generator which needs to be a long or array of longs
	unsigned long hash_long[20];
	// convert the array of chars to an array of longs
	for (char n = 0; n < 20; n++)
	{
		hash_long[n] = hash[n];
	}

	// Seed the random with the key
	MTRand mt(hash_long, 20);

	// A shift point is a point in the data where we regenerate the encryption mapping
	// each continious section of one encryption mapping is know as a chunk
	// get the first shift point from the generator, needs to be the first number we pull off that key
    unsigned int shift = mt() * ChunkSize;
	// set the last shit point, it's 0 becasue we havn't shifted yet
	unsigned long last_shift = 0;
	// makea pointer to store a new encryption key
	unsigned char* new_key;

	// loop through the data
    for (unsigned long i = 0; i < size; i ++)
	{
		// if we have reached our shift point
		if (i - last_shift == shift)
		{
			// make space to store a new encryption key key
			new_key = new unsigned char[shift + KeySize];
			// copy the real key to the new key
			memcpy(new_key, Key, KeySize);
			// copy the encrypted data form the last chunk to the end of the key to make a key for the next chunk
			memcpy(&new_key[KeySize], &in_data[last_shift], shift);
			// re init the encoding map
			init_map(encoding);
			// rescrable the map with the new key for this next chunk
			randomise_arr(encoding, 256, new_key, KeySize + shift);
			// record this shift point
			last_shift += shift;
			// get the next shift point
			shift = mt() * ChunkSize;
			// delete the the generated key
            delete[] new_key;
		}
		// get the decrypted charater for this part of the data by finding the index of the value in the mapping
		out_data[i] = std::distance(encoding, std::find(encoding, encoding + 256, in_data[i]));
	}
}

// encryptes an array of data inplace modifying the original array
void CycleByte::encrypt_inplace(unsigned char* data, unsigned long size)
{
	// we need arrays to map the ASCII values (really just raw byte valeus we dont care if they are used as ASCII)
	unsigned char encoding[256];
	// fill the array with the values with 0-255
	init_map(encoding);
	// scramble the array predictably with the key for thsi encryption
	randomise_arr(encoding, 256, Key, KeySize);

	// SHA1 hash of key
	// generate a SHA1 of the encryption key
	CSHA1 sha1;
	sha1.Update(Key, KeySize);
	sha1.Final();
	// get the hash out
	unsigned char hash[20];
	sha1.GetHash(hash);
	// we need to convert that hash to a seed for our generator which needs to be a long or array of longs
	unsigned long hash_long[20];
	// convert the array of chars to an array of longs
	for (char n = 0; n < 20; n++)
	{
		hash_long[n] = hash[n];
	}

	// Seed the random with the key
	MTRand mt(hash_long, 20);

	// A shift point is a point in the data where we regenerate the encryption mapping
	// each continious section of one encryption mapping is know as a chunk
	// get the first shift point from the generator, needs to be the first number we pull off that key
	unsigned int shift = mt() * ChunkSize;
	// set the last shit point, it's 0 becasue we havn't shifted yet
	unsigned long last_shift = 0;
	// make a pointer to store a new encryption key
	unsigned char* new_key;

	// loop through the data
	for (unsigned long i = 0; i < size; i ++)
	{
		//if we have reached our shift point
		if (i - last_shift == shift)
		{
			// make space to store a new encryption key key
			new_key = new unsigned char[shift + KeySize];
			// copy the real key to the new key
			memcpy(new_key, Key, KeySize);
			// copy the encrypted data form the last chunk to the end of the key to make a key for the next chunk
			memcpy(&new_key[KeySize], &data[last_shift], shift);
			// re init the encoding map
			init_map(encoding);
			// rescrable the map with the new key for this next chunk
			randomise_arr(encoding, 256, new_key, KeySize + shift);
			// record this shift point
			last_shift += shift;
			// get the next shift point
			shift = mt() * ChunkSize;
			// delete the the generated key
            delete[] new_key;
		}
		//get the encrypted charater for this bit if the data and store it
		data[i] = encoding[data[i]];
	}

}

// decryptes an array fo data inplace modifying the original array
void CycleByte::decrypt_inplace(unsigned char* data, unsigned long size) 
{
    // we need arrays to map the ASCII values (really just raw byte valeus we dont care if they are used as ASCII)
	unsigned char encoding[256];
	// fill the array with the values with 0-255
	init_map(encoding);
	// scramble the array predictably with the key for thsi encryption
	randomise_arr(encoding, 256, Key, KeySize);

    // SHA1 hash of key
	CSHA1 sha1;
	sha1.Update(Key, KeySize);
	// get the hash out
	sha1.Final();
	unsigned char hash[20];
	sha1.GetHash(hash);
	// we need to convert that hash to a seed for our generator which needs to be a long or array of longs
	unsigned long hash_long[20];
	// convert the array of chars to an array of longs
	for (char n = 0; n < 20; n++)
	{
		hash_long[n] = hash[n];
	}

	// Seed the random with the key
	MTRand mt(hash_long, 20);

	// A shift point is a point in the data where we regenerate the encryption mapping 
	// each continious section of one encryption mapping is know as a chunk
	// get the first shift point from the generator, needs to be the first number we pull off that key
    unsigned int shift = mt() * ChunkSize;
	// set the last shit point, it's 0 becasue we havn't shifted yet
	unsigned long last_shift = 0;
	// makea pointer to store a new encryption key
	unsigned char* new_key;

	// we are modifing data in place by wee still need the encrypted data from the last chunk to get the key for the next chunk so we will copy it out
	unsigned char* part;
	// copy out the first encrypted chuk for use when we shift chunks
	part = new unsigned char[shift];
	memcpy(part, data, shift);

	// loop through the data
    for (unsigned long i = 0; i < size; i ++)
	{
		// if we have reached our shift point
		if (i - last_shift == shift)
		{
			// make space to store a new encryption key key
			new_key = new unsigned char[shift + KeySize];
			// copy the real key to the new key
			memcpy(new_key, Key, KeySize);
			// copy the encrypted data form the last chunk to the end of the key to make a key for the next chunk
			memcpy(&new_key[KeySize], part, shift);
			// re init the encoding map
			init_map(encoding);
			// rescrable the map with the new key for this next chunk
			randomise_arr(encoding, 256, new_key, KeySize + shift);
			// record this shift point
			last_shift += shift;
			// get the next shift point
			shift = mt() * ChunkSize;
			// delete the the generated key
            delete[] new_key;
			// we need to copy out the next encrypted chunk before we modify it to use in getting the key for the chunk after
			// first deleate the current part
			delete[] part;
			// next recreate it and copy out the next chunk
			part = new unsigned char[shift];
			memcpy(part, data, shift);
		}
		// get the decrypted charater for this part of the data by finding the index of the value in the mapping
		data[i] = std::distance(encoding, std::find(encoding, encoding + 256, data[i]));
	}

	//we are done so lets clean up the copyed out encrypted data
	delete[] part;
}

void CycleByte::init_map(unsigned char out[256])
{
	// loop and fill the array witht eh values 0-255 (the possible value range for a char value)
	for (int i = 0; i < 256; i++)
	{
		out[i] = i;
	}
}

void CycleByte::randomise_arr(unsigned char* arr, unsigned int size_arr, const unsigned char* key, unsigned int size_key)
{
	if (size_arr > 1) 
    {
		// SHA1 hash of key
		// generate a SHA1 of the encryption key
		CSHA1 sha1;
		sha1.Update((unsigned char*)key, size_key);
		sha1.Final();
		// get the hash out
		unsigned char hash[20];
		sha1.GetHash(hash);
		// we need to convert that hash to a seed for our generator which needs to be a long or array of longs
	    unsigned long hash_long[20];
		// convert the array of chars to an array of longs
		for (char n = 0; n < 20; n++)
		{
			hash_long[n] = hash[n];
		}

		// Seed the random with the key
		MTRand mt(hash_long, 20);

		//scramble array
        unsigned int i;
        for (i = 0; i < size_arr - 1; i++) 
        {

			unsigned int j = mt() * size_arr;
            char t = arr[j];
            arr[j] = arr[i];
            arr[i] = t;
        }
    }
}