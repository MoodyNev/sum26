//gcc -Wall -Wextra -std=c11 arrays.c -o arrays

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Definition of useful constants */
/* Maximal length of the codeword */
#define MAXCODE 10

/* Maximal length of the text/cipher */
#define MAXTEXT 100

/* Operation CODE = encrypt, DECODE = decrypt */
#define CODE 0
#define DECODE 1

/* This reads from the keyboard "length" characters at most
   and saves them into the variable txt. */
void readFromKeyboard(char *txt, int length) {
    int count = 0;
    int c;

    /* Read first character before loop */
    c = getchar();

    /* Read until newline or maximum length */
    while (count < length && c != '\n' && c != EOF) { // EOF = -1 (end of file, specical character) and \n for new line
        txt[count] = (char)c;
        count++;
        c = getchar();
    }

    /* End string */
    txt[count] = '\0';
}

/* Read codeword */
void readCodeword(char *txt) {
    printf("Enter a codeword <= %d characters: ", MAXCODE); //the %d is used to replace with the variable in the string 
    readFromKeyboard(txt, MAXCODE);
}

/* Read plaintext or ciphertext */
void readPlaintext(char *txt) {
    printf("Enter a plaintext or ciphertext <= %d characters: ", MAXTEXT);
    readFromKeyboard(txt, MAXTEXT);
}

/* Ask whether to encrypt or decrypt */
int readOperation(void) {
    char operation;

    printf("Enter (e) to encrypt and (d) to decrypt: ");
    scanf(" %c", &operation);

    if (operation == 'e') {
        return CODE;
    } else {
        return DECODE;
    }
}

/* Prepare text:
   - convert lowercase letters to uppercase
   - remove special characters
   - keep only uppercase letters and numbers
*/
void prepareText(char *text, char *clean) { //simply translates the text into an array to make the encryption easier
    int i;
    int n = 0;

    for (i = 0; i < (int)strlen(text); i++) {
        if (isalnum((unsigned char)text[i])) {
            if (islower((unsigned char)text[i])) {
                clean[n] = text[i] + 'A' - 'a'; //ascii table, this is used to make the lowercase letters uppercase
            } else {
                clean[n] = text[i];
            }
            n++;
        }
    }

    clean[n] = '\0';
}

/* Map allowed character to code:
   A-Z -> 0-25
   0-9 -> 26-35
*/
int computeCode(char plain) {
    if (plain >= 'A' && plain <= 'Z') {
        return plain - 'A';
    } else if (isdigit((unsigned char)plain)) {
        return 26 + (plain - '0');
    }

    return plain;
}

/* Reverse mapping:
   0-25 -> A-Z
   26-35 -> 0-9
*/
char computeCharacters(int code) {
    if (code >= 0 && code <= 25) {
        return 'A' + code;
    } else if (code >= 26 && code <= 35) {
        return '0' + (code - 26);
    }

    return (char)code;
}

/* Encrypt/decrypt plaintext using codeword */
void coding(char *codeword, char *plaintext, char *result, int operation) {
    int i;
    int codeText;
    int codeCode;
    int codeResult;
    int codeLength;

    codeLength = strlen(codeword);

    /* Avoid division by zero if codeword becomes empty after cleaning */
    if (codeLength == 0) {
        result[0] = '\0';
        return;
    }

    printf("Coding:\n");

    for (i = 0; i < (int)strlen(plaintext); i++) {
        codeText = computeCode(plaintext[i]);
        codeCode = computeCode(codeword[i % codeLength]);

        if (operation == CODE) {
            codeResult = (codeText + codeCode) % 36;
        } else {
            codeResult = (codeText - codeCode) % 36;
            if (codeResult < 0) {
                codeResult += 36;
            }
        }

        result[i] = computeCharacters(codeResult);
    }

    result[i] = '\0';
}

int main(void) {
    /* Codeword */
    char codeword[MAXCODE + 1];
    /* Codeword after preparation */
    char cleancodeword[MAXCODE + 1];
    /* Text / cipher */
    char plaintext[MAXTEXT + 1];
    /* Text / cipher after preparation */
    char cleanPlaintext[MAXTEXT + 1];
    /* Result of encryption / decryption */
    char result[MAXTEXT + 1];
    /* Operation */
    int operation;

    /* Read inputs */
    readCodeword(codeword);
    readPlaintext(plaintext);
    operation = readOperation();

    /* Prepare text and codeword */
    prepareText(plaintext, cleanPlaintext);
    prepareText(codeword, cleancodeword);

    /* Execute encryption / decryption */
    coding(cleancodeword, cleanPlaintext, result, operation);

    /* Output */
    printf("Result:\n");
    printf("Codeword: %s\n", codeword);
    printf("Clean text: %s\n", cleanPlaintext);
    printf("Clean codeword: %s\n", cleancodeword);
    printf("---------------------------------------------\n");
    printf("Result: %s\n", result);

    return 0;
}