#include "transaction.h"

/**
 * tx_out_create - allocates and initializes transaction output structure
 * @amount: amount of transaction
 * @pub: public key of transaction receiver
 * Return: pointer to created transaction output upon success, otherwise NULL
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *add = NULL;

	if (!amount || !pub)
		return (NULL);
	add = calloc(1, sizeof(tx_out_t));
	if (!add)
		return (NULL);
	add->amount = amount;
	memcpy(add->pub, pub, EC_PUB_LEN);
	SHA256((uint8_t *)add, sizeof(uint32_t) + EC_PUB_LEN, add->hash);
	return (add);
}
