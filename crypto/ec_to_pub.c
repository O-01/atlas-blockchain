#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts public key from EC_KEY opaque structure
 * @key: pointer to EC_KEY structure from which to retrieve public key - if
 *       NULL, function must do nothing and fail
 * @pub: address at which to store extracted public key (key shall not be
 *       compressed)
 * Return: pointer to `pub` upon success, otherwise NULL
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	if (!key)
		return (NULL);
	EC_POINT_point2oct(
		EC_KEY_get0_group(key), 
		EC_KEY_get0_public_key(key),
		EC_KEY_get_conv_form(key),
		pub,
		EC_PUB_LEN,
		NULL);
	return (pub);
}
