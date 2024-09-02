#include <stdio.h>
#include "smCipherKey.h"

int main() {
    smCipherKey cipherKey;
    smCipherKey_Init(&cipherKey);

    printf("Key Block After Reset:\n");
    for (int i = 0; i < 4; i++) {
        printf("%08lx ", cipherKey.keyBlock[i]);
    }
    printf("\n");

    smCipherKey_ResetWithKey(&cipherKey, "ExampleKeyString");
    printf("Key Block After Reset with String:\n");
    for (int i = 0; i < 4; i++) {
        printf("%08lx ", cipherKey.keyBlock[i]);
    }
    printf("\n");

    return 0;
}
