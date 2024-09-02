#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char MAP1[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

static unsigned char MAP2[128];

void init_map2() {
    memset(MAP2, 255, 128);
    for (int i = 0; i < 64; i++) {
        MAP2[(unsigned char)MAP1[i]] = (unsigned char)i;
    }
}

char *encode(const unsigned char *input, int len) {
    int iLen = len;
    int oDataLen = (iLen * 4 + 2) / 3;
    int oLen = ((iLen + 2) / 3) * 4;
    char *output = (char *)malloc(oLen + 1);
    if (!output)
        return NULL;

    int ip = 0, op = 0;
    while (ip < iLen) {
        int i0 = input[ip++] & 0xff;
        int i1 = ip < iLen ? input[ip++] & 0xff : 0;
        int i2 = ip < iLen ? input[ip++] & 0xff : 0;
        int o0 = i0 >> 2;
        int o1 = ((i0 & 3) << 4) | (i1 >> 4);
        int o2 = ((i1 & 0xf) << 2) | (i2 >> 6);
        int o3 = i2 & 0x3F;

        output[op++] = MAP1[o0];
        output[op++] = MAP1[o1];
        output[op] = op < oDataLen ? MAP1[o2] : '=';
        op++;
        output[op] = op < oDataLen ? MAP1[o3] : '=';
        op++;
    }
    output[op] = '\0';
    return output;
}

unsigned char *decode(const char *input, int *out_len) {
    int iLen = strlen(input);
    if (iLen % 4 != 0) {
        return NULL;
    }

    while (iLen > 0 && input[iLen - 1] == '=') 
        iLen--;

    int oLen = (iLen * 3) / 4;
    unsigned char *output = (unsigned char *)malloc(oLen);
    if (!output)
        return NULL;

    int ip = 0, op = 0;

    while (ip < iLen) {
        int i0 = input[ip++];
        int i1 = input[ip++];
        int i2 = ip < iLen ? input[ip++] : 'A';
        int i3 = ip < iLen ? input[ip++] : 'A';

        if (i0 > 127 || i1 > 127 || i2 > 127 || i3 > 127 || MAP2[i0] == 255 || MAP2[i1] == 255 || MAP2[i2] == 255 || MAP2[i3] == 255) {
            free(output);
            return NULL;
        }

        int b0 = MAP2[i0];
        int b1 = MAP2[i1];
        int b2 = MAP2[i2];
        int b3 = MAP2[i3];

        output[op++] = (unsigned char)((b0 << 2) | (b1 >> 4));
        if (op < oLen)
            output[op++] = (unsigned char)((b1 & 0xf) << 4 | (b2 >> 2));
        if (op < oLen)
            output[op++] = (unsigned char)((b2 & 3) << 6 | b3);
    }
    *out_len = oLen;
    return output;
}

int main() {
    init_map2();

    const char *test_str = "Hello, World!";
    char *encoded = encode((const unsigned char *)test_str, strlen(test_str));
    printf("Encoded: %s\n", encoded);

    int decoded_len;
    unsigned char *decoded = decode(encoded, &decoded_len);
    printf("Decoded: %.*s\n", decoded_len, decoded);

    free(encoded);
    free(decoded);

    return 0;
}
