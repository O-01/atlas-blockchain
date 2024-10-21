#include "transaction.h"

/**
 * tx_in_create - allocates and initializes transaction input structure
 * @unspent: unpent transaction output to be converted to transaction input
 * Return: pointer to created transaction input upon success, otherwise NULL
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *add = NULL;

	if (!unspent)
		return (NULL);
	add = calloc(1, sizeof(tx_in_t));
	if (!add)
		return (NULL);
	memcpy(add->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(add->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(add->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	memset(&add->sig, '\x00', sizeof(sig_t));
	return (add);
}
