#include "smCipherKey.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void smCipherKey_Init(smCipherKey* cipherKey) {
    cipherKey->isKeySet = 0;
    smCipherKey_Reset(cipherKey);
}

void smCipherKey_InitWithKey(smCipherKey* cipherKey, const char* keyString) {
    cipherKey->isKeySet = 0;
    smCipherKey_ResetWithKey(cipherKey, keyString);
}

void smCipherKey_Reset(smCipherKey* cipherKey) {
    srand((unsigned)time(NULL));  // Seed with current time
    smCipherKey_BuildKey(cipherKey);
}

void smCipherKey_ResetWithKey(smCipherKey* cipherKey, const char* keyString) {
    smCipherKey_BuildKeyWithString(cipherKey, keyString);
}

void smCipherKey_BuildKey(smCipherKey* cipherKey) {
    for (int i = 0; i < 4; i++) {
        cipherKey->keyBlock[i] = (unsigned long)(rand() & 0xFFFFFFFFUL);
    }
    cipherKey->isKeySet = 1;
}

void smCipherKey_BuildKeyWithString(smCipherKey* cipherKey, const char* keyString) {
    char adjustedKeyString[17];
    strncpy_s(adjustedKeyString, sizeof(adjustedKeyString), keyString, 16);

    if (strlen(keyString) < 16) {
        memset(adjustedKeyString + strlen(keyString), keyString[0], 16 - strlen(keyString));
    }
    adjustedKeyString[16] = '\0';

    for (int i = 0; i < 4; i++) {
        cipherKey->keyBlock[i] = 0UL;
        for (int j = 0; j < 4; j++) {
            cipherKey->keyBlock[i] |= (unsigned long)(adjustedKeyString[i * 4 + j] & 0xFF) << (j * 8);
        }
    }
    cipherKey->isKeySet = 1;
}

