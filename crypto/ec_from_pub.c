#include "hblk_crypto.h"

/**
 * ec_from_pub - creates an EC_KEY structure containing given public key
 * @pub: contains the public key to be converted
 * Return: pointer to created EC_KEY structure upon success, otherwise NULL
 * Note: created EC_KEY‘s private key does not have to be initialized/set, we
 *       only care about public one
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	const EC_GROUP *group = NULL;
	EC_POINT *point = NULL;
	int yes = 0;

	if (!pub)
		return (NULL);
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	group = EC_KEY_get0_group(key);
	EC_POINT_oct2point(group, point = EC_POINT_new(group), pub, EC_PUB_LEN, NULL);
	yes = EC_KEY_set_public_key(key, point);
	EC_POINT_free(point);
	return (yes ? key : NULL);
}
