#include <stdio.h>
#include "smBase64.h"

int main() {
    init_map2();

    const char* test_str = "Hello, World! .. This is a test of smBase64 encoding and decoding.";
    char* encoded = encode((const unsigned char*)test_str, strlen(test_str));
    printf("Encoded: %s\n", encoded);

    int decoded_len;
    unsigned char* decoded = decode(encoded, &decoded_len);
    printf("Decoded: %.*s\n", decoded_len, decoded);

    free(encoded);
    free(decoded);

    return 0;
}
