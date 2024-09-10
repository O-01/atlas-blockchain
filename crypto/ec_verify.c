#include "hblk_crypto.h"

/**
 * ec_verify - verifies signature of given set of bytes, using given EC_KEY
 *             public key
 * @key: points to EC_KEY structure containing public key to be used to verify
 *       signature
 * @msg: points characters of length `msglen` to verify signature of
 * @msglen: length of `msg` character array
 * @sig: points to signature to be checked
 * Return: 1 upon valid signature, otherwise 0
 */
int ec_verify(
	EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t const *sig)
{
	if (!key || !msg || (msg && !msglen))
		return (0);
	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key));
}
