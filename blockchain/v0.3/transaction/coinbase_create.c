#include "blockchain.h"

/**
 * coinbase_create - creates coinbase transaction
 * @receiver: contains public key of miner, recipient of coinbase coins
 * @block_index: index of Block coinbase transaction will belong to
 * Return: pointer to created transaction upon success, otherwise NULL
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx = NULL;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};

	if (!receiver)
		return (NULL);
	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
		return (NULL);
	in = calloc(1, sizeof(tx_in_t));
	if (!in)
		return (FREE_0(tx));
	memcpy(in->tx_out_hash, &block_index, sizeof(uint32_t));
	out = tx_out_create(COINBASE_AMOUNT, ec_to_pub(receiver, pub));
	if (!out)
		return (FREE_0(in), FREE_0(tx));
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	transaction_hash(tx, tx->id);
	return (tx);
}
