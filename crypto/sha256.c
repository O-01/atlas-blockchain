#include "hblk_crypto.h"

/**
 * sha256 - computes hash of sequence of bytes using SHA256 algorithm
 * @s: sequence of bytes to be hashed
 * @len: number of bytes to hash in `s`
 * @digest: uint8_t array for storage of result hash
 * Return: pointer to `digest` upon success, otherwise NULL
 */
uint8_t *sha256(
	int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	;

	if (!digest)
		return (NULL);
}
