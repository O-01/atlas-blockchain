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
	if (!key || !msg || (msg && !msglen))
		return (NULL);
	ECDSA_sign(0, msg, msglen, sig->sig, (unsigned int *)&sig->len, (EC_KEY *)key);
	return (sig->sig);
}
