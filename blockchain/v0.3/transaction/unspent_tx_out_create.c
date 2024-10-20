#include "transaction.h"

/**
 * unspent_tx_out_create - allocates and initializes unspent transaction output
 *                         structure
 * @block_hash: contains hash of Block where referenced transaction output is
 *              located
 * @tx_id: contains hash of transaction in Block block_hash, where referenced
 *         transaction output is located
 * @out: pointer to referenced transaction output
 * Return: pointer to created unspent transaction output upon success,
 *         otherwise NULL
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out)
{
	unspent_tx_out_t *add = NULL;

	if (!block_hash || !tx_id || !out)
		return (NULL);
	add = calloc(1, sizeof(unspent_tx_out_t));
	if (!add)
		return (NULL);
	memcpy(add->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(add->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	add->out = *out;
	return (add);
}
