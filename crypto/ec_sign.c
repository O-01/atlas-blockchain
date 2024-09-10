#include "hblk_crypto.h"

/**
 * ec_sign - signs given set of bytes, using given EC_KEY private key
 * @key: points to EC_KEY structure containing private key to be used to
 *       perform signature
 * @msg: points to characters of len `msglen` to be signed
 * @msglen: length of `msg` char array
 * @sig: holds address at which to store signature
 * Return: pointer to signature buffer upon success, otherwise NULL
 */
uint8_t *ec_sign(
	EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)
{
	uint8_t digest[SHA256_DIGEST_LENGTH] = {0};

	if (!key || !msg || (msg && !msglen))
		return (NULL);
	SHA256(msg, msglen, digest);
	ECDSA_sign(0, digest, SHA256_DIGEST_LENGTH, sig->sig, (unsigned int *)&sig->len, (EC_KEY *)key);
	return (sig->sig);
}
