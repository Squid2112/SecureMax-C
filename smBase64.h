#pragma once
#ifndef SMBASE64_H
#define SMBASE64_H

// Function to initialize the MAP2 array
void init_map2(void);

// Function to encode a byte array to a Base64 string
char* encode(const unsigned char* input, int len);

// Function to decode a Base64 string to a byte array
unsigned char* decode(const char* input, int* out_len);

#endif // SMBASE64_H
