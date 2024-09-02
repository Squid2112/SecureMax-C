#ifndef SMCIPHERKEY_H
#define SMCIPHERKEY_H

typedef struct {
    unsigned long keyBlock[4];
    int isKeySet;
} smCipherKey;

// Function to initialize smCipherKey without a specific key string
void smCipherKey_Init(smCipherKey* cipherKey);

// Function to initialize smCipherKey with a specific key string
void smCipherKey_InitWithKey(smCipherKey* cipherKey, const char* keyString);

// Function to reset and generate a new random key
void smCipherKey_Reset(smCipherKey* cipherKey);

// Function to reset and generate a key based on a specific key string
void smCipherKey_ResetWithKey(smCipherKey* cipherKey, const char* keyString);

// Internal function to build a random key
void smCipherKey_BuildKey(smCipherKey* cipherKey);

// Internal function to build a key based on a string
void smCipherKey_BuildKeyWithString(smCipherKey* cipherKey, const char* keyString);

#endif // SMCIPHERKEY_H
