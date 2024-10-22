#include "blockchain.h"

/**
 * coinbase_is_valid - checks whether coinbase transaction is valid
 * @coinbase: points to coinbase transaction to verify
 * @block_index: index of Block coinbase transaction will belong to
 * Return: 1 if coinbase is valid, otherwise 0
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t sample[SHA256_DIGEST_LENGTH], zero_32[SHA256_DIGEST_LENGTH] = {0};
	uint32_t ins = 0, outs = 0;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;
	sig_t zero_sig = {0};

	if (!coinbase)
		return (0);
	transaction_hash(coinbase, sample);
	if (memcmp(sample, coinbase->id, SHA256_DIGEST_LENGTH))
		return (0);
	ins = llist_size(coinbase->inputs), outs = llist_size(coinbase->outputs);
	if (ins != outs || ins != 1 || outs != 1)
		return (0);
	in = llist_get_head(coinbase->inputs);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(uint32_t)) ||
		memcmp(in->block_hash, zero_32, SHA256_DIGEST_LENGTH) ||
		memcmp(in->tx_id, zero_32, SHA256_DIGEST_LENGTH) ||
		memcmp(&in->sig, &zero_sig, sizeof(sig_t)))
		return (0);
	out = llist_get_head(coinbase->outputs);
	return (out->amount == COINBASE_AMOUNT);
}
