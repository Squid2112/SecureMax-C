#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "smBase64.h"

int main() {
    init_map2();

    const char* test_str = "Hello, World! .. This is a test of smBase64 encoding and decoding.";
    const char* expected_encoded = "SGVsbG8sIFdvcmxkISAuLiBUaGlzIGlzIGEgdGVzdCBvZiBzbUJhc2U2NCBlbmNvZGluZyBhbmQgZGVjb2Rpbmcu";

    char* encoded = encode((const unsigned char*)test_str, strlen(test_str));
    printf("Encoded: %s\n", encoded);

    if (strcmp(encoded, expected_encoded) == 0) {
        printf("Encoding passed!\n");
    } else {
        printf("Encoding failed!\n");
    }

    int decoded_len;
    unsigned char* decoded = decode(encoded, &decoded_len);
    printf("Decoded: %.*s\n", decoded_len, decoded);

    if (strncmp((char*)decoded, test_str, decoded_len) == 0) {
        printf("Decoding passed!\n");
    } else {
        printf("Decoding failed!\n");
    }

    free(encoded);
    free(decoded);

    return 0;
}
