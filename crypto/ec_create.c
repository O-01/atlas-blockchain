#include "hblk_crypto.h"

/**
 * ec_create - creates new EC key pair
 * Return: pointer to EC_KEY structure containing both public and private keys
 *         upon success, otherwise NULL
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key = NULL;

	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (!key)
		return (NULL);
	if (!EC_KEY_generate_key(key))
		return (NULL);
	return (key);
}
