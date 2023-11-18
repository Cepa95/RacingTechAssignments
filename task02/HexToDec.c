#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h> 

#define MAX_HEX_LENGTH 20

unsigned long hexCharToDecimal(char hexChar) {
    if (isdigit(hexChar)) {
        return hexChar - '0';
    }
    else {
        hexChar = tolower(hexChar);

        if (hexChar >= 'a' && hexChar <= 'f') {
            return hexChar - 'a' + 10;
        }
        return ULONG_MAX;
    }
}

unsigned long hexStringToDecimal(const char* hexString) {
    unsigned long decimalNumber = 0;

    for (int i = 0; i < hexString[i] != '\0'; ++i) {
        unsigned long hexValue = hexCharToDecimal(hexString[i]);

        if (hexValue == ULONG_MAX) {
            printf("Invalid character '%c' at position %d\n", hexString[i], i + 1);
            return ULONG_MAX;
        }
        decimalNumber = decimalNumber * 16 + hexValue;
    }
    return decimalNumber;
}

int main() {
    char* hexInput = (char*)malloc(MAX_HEX_LENGTH + 1); 

    if (!hexInput) {
        printf("Memory allocation error.\n");
        return -1;
    }
    printf("Enter a hexadecimal string: ");
    scanf("%s", hexInput);

    if (strlen(hexInput) > MAX_HEX_LENGTH) {
        printf("Input string exceeds maximum allowed length.\n");
        free(hexInput); 
        return -1;
    }
    unsigned long decimalNumber = hexStringToDecimal(hexInput);

    if (decimalNumber != ULONG_MAX) {
        printf("Decimal number: %lu\n", decimalNumber);
    }
    free(hexInput);
    return 0;
}
