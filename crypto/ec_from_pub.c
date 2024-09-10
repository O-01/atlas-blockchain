#include "hblk_crypto.h"

/**
 * ec_from_pub -
 * @pub:
 * Return:
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;

	if (!pub)
		return (NULL);
	EC_KEY_new_by_curve_name(NID_secp256k1)
}
