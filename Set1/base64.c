/**
    base64.c

    The hard way to do:
    http://cryptopals.com/sets/1/challenges/1/

    @author: Luke Knight
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char PAD = '=';
const char BASE64_TABLE[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                               'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                               'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                               'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                               'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                               'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                               'w', 'x', 'y', 'z', '0', '1', '2', '3',
                               '4', '5', '6', '7', '8', '9', '+', '/'};

/**
    @TODO
    Builds a character array of bytes from a hexadecimal string.
    Assumes valid hex input.

    @param hexstring a hexadecimal string represented with characters.
    @return byte array from the hexadecimal string.
 */
static char *hexstring_to_bytes(char *hexstring)
{

}

/**
    Obtains base64 length from string length.

    @param decoded_length length of the string to be encoded.
    @return length of encoded string after a string of length
    'decoded_length' has been encoded.
 */
static int get_encoded_length(int decoded_length)
{
    uint8_t padding = (decoded_length % 3 ? 3 - (decoded_length % 3) : 0);
    uint8_t code_length = 4 * ((float)(decoded_length + padding)/3);
    return code_length;
}

/**
    Encodes a string given by a pointer to the start of an array
    into base64 as per https://tools.ietf.org/html/rfc4648 with
    no consideration of line breaks yet.

    @param bytes pointer to the start of the string
    @return pointer to the start of base64 encoding of input string

 */
static char *bytes_to_base64(char *bytes)
{
    uint32_t group = 0; // 24-bit group
    size_t decoded_length = strlen(bytes);
    size_t encoded_length = get_encoded_length(decoded_length);
    // allocate bytes according to anticipated length
    char *base64 = malloc(sizeof(char) * encoded_length);
    size_t index = 0;
    if (sizeof(bytes) > 1) {
        for (size_t i = 0; i < decoded_length; i += 3) {
            // build 24-bit group
            group = (uint32_t)bytes[i] << 16;
            if (i + 1 < decoded_length) {
                group |= (uint32_t)bytes[i+1] << 8;
            }
            if (i + 2 < decoded_length) {
                group |= (uint32_t)bytes[i+2];
            }
            // separate into 6 bit numbers using 111111 mask
            base64[index++] = BASE64_TABLE[(uint8_t)(group >> 18) & 63];
            base64[index++] = BASE64_TABLE[(uint8_t)(group >> 12) & 63];
            base64[index++] = BASE64_TABLE[(uint8_t)(group >> 6) & 63];
            base64[index++] = BASE64_TABLE[(uint8_t)(group >> 0) & 63];
        }
        // replace meaningless bytes with padding
        uint8_t padding = (decoded_length % 3 ? 3 - (decoded_length % 3) : 0);
        while (padding > 0) {
            base64[encoded_length - padding] = PAD;
            padding--;
        }
        return base64;
    } else {
        return "";
    }
}

/**
    Utilises two functions to obtain base64 from a hexadecimal string.

    @param hexstring pointer to the start of the hexadecimal string.
    @return base64 encoding of the hexadecimal.
*/
static char *hexstring_to_base64(char *hexstring)
{
    return bytes_to_base64(hexstring_to_bytes(hexstring));
}

/**
    Will print the answer to the challenge.
*/
int main()
{
    char *test_str = "Man";
    printf("ENCODING %s\n", test_str);
    char *test_encode = bytes_to_base64(test_str);
    printf("BASE64 STRING: ");
    for (int i = 0; i < get_encoded_length(strlen(test_str)); i++) {
        printf("%c", test_encode[i]);
    }
    free(test_encode);
    printf("\n");

    return 0;
}
