#include "smBase64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


static const char MAP1[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

#if defined(_WIN32) || defined(_WIN64)
// Windows-specific headers and definitions
#include <windows.h>
static unsigned char MAP2[128];
static bool map_initialized = false;
static CRITICAL_SECTION map_critical_section;

void init_map2(void) {
    static bool critical_section_initialized = false;
    if (!critical_section_initialized) {
        InitializeCriticalSection(&map_critical_section);
        critical_section_initialized = true;
    }

    EnterCriticalSection(&map_critical_section);
    if (!map_initialized) {  // Double-check after acquiring the lock
        memset(MAP2, 255, 128);
        for (int i = 0; i < 64; i++) {
            MAP2[(unsigned char)MAP1[i]] = (unsigned char)i;
        }
        map_initialized = true;
    }
    LeaveCriticalSection(&map_critical_section);
}

#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
// C11-specific headers and definitions
#include <threads.h>
static unsigned char MAP2[128];
static once_flag map_once_flag = ONCE_FLAG_INIT;

void init_map2_function(void) {
    memset(MAP2, 255, 128);
    for (int i = 0; i < 64; i++) {
        MAP2[(unsigned char)MAP1[i]] = (unsigned char)i;
    }
}

void init_map2(void) {
    call_once(&map_once_flag, init_map2_function);
}

#else
#error "No suitable threading model found. Please define a supported threading model."
#endif


char* encode(const unsigned char* input, int len) {
    if (input == NULL || len < 0) {
        return NULL;  // Invalid input
    }

    int iLen = len;
    int oDataLen = (iLen * 4 + 2) / 3;
    int oLen = ((iLen + 2) / 3) * 4;
    char* output = (char*)malloc(oLen + 1);
    if (!output) return NULL;  // Memory allocation failed

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

unsigned char* decode(const char* input, int* out_len) {
    if (input == NULL || out_len == NULL) {
        return NULL;  // Invalid input
    }

    int iLen = strlen(input);
    if (iLen % 4 != 0) {
        return NULL;  // Invalid Base64 length
    }

    // Validate input contains only valid Base64 characters or '=' padding
    for (int i = 0; i < iLen; i++) {
        if ((input[i] > 127 || MAP2[(unsigned char)input[i]] == 255) && input[i] != '=') {
            return NULL;  // Invalid character in input
        }
    }

    while (iLen > 0 && input[iLen - 1] == '=')
        iLen--;

    int oLen = (iLen * 3) / 4;
    unsigned char* output = (unsigned char*)malloc(oLen);
    if (!output) return NULL;  // Memory allocation failed

    int ip = 0, op = 0;

    while (ip < iLen) {
        int i0 = input[ip++];
        int i1 = input[ip++];
        int i2 = ip < iLen ? input[ip++] : 'A';
        int i3 = ip < iLen ? input[ip++] : 'A';

        if (i0 > 127 || i1 > 127 || i2 > 127 || i3 > 127 ||
            MAP2[i0] == 255 || MAP2[i1] == 255 || MAP2[i2] == 255 || MAP2[i3] == 255) {
            free(output);
            return NULL;  // Invalid character in Base64 data
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
