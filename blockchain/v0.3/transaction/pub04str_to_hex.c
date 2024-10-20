#include "transaction.h"

#define IS_NO(c) ((c) >= '0' && (c) <= '9')
#define IS_AF(c) ((c) >= 'a' && (c) <= 'f')
#define IS_X(c) (IS_NO((c)) || IS_AF((c)))

#define X_NO(c) ((c) - '0')
#define X_AF(c) ((c) - 'W')

void pub04str_to_hex(uint8_t const *src, uint8_t hex[EC_PUB_LEN]);
uint8_t to_hex4(uint8_t c, uint8_t *hex, uint8_t set);
uint8_t atoh(uint8_t c);
void print_hex_buf(uint8_t const *buf, size_t len);

/**
 * pub04str_to_hex - converts hash string to its hex form
 * @src: hash string in decimal form
 * @hex: array to contain hash in converted hex form
 */
void pub04str_to_hex(uint8_t const *src, uint8_t hex[EC_PUB_LEN]) {
	uint8_t iter = 2, set = 0;

	hex[set++] = '\x04';
	for (; iter < 129; iter += 2, ++set)
		hex[set] = (to_hex4(src[iter], hex, set) << 4) | to_hex4(src[iter + 1], hex, set);
}

/**
 * to_hex4 - manages conversion of hash string char to hex form
 * @c: hash string char to be converted
 * @hex: array containing hash in converted hex form in its current state
 * @set: current index of char to be converted
 * Return: hex form upon success, otherwise exit with code 1
 */
uint8_t to_hex4(uint8_t c, uint8_t *hex, uint8_t set) {
	if (!IS_X(c)) {
		print_hex_buf(hex, set);
		fprintf(stderr, "\x1b[1;31m@\n[*] Invalid pub - ERROR: Invalid hash character at index %d\n\x1b[3mFAILURE!\x1b[0m\n", set);
		exit(EXIT_FAILURE);
	}
	return (atoh(c));
}

/**
 * atoh - converts hash string char to hex form
 * @c: hash string char to be converted
 * Return: hex form of input char
 */
uint8_t atoh(uint8_t c) { return (IS_NO(c) ? X_NO(c) : X_AF(c)); }

/**
 * print_hex_buf - prints buffer in hexadecimal form
 * @buf: buffer to be printed
 * @len: number of bytes from buf to be printed
 */
void print_hex_buf(uint8_t const *buf, size_t len) {
	size_t i = 0;

	for (; buf && i < len; i++)
		printf("%02x", buf[i]);
}
