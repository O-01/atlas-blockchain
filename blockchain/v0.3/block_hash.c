#include "blockchain.h"

static int add_tx(transaction_t *tx, uint32_t iter, uint8_t *buff);

/**
 * block_hash - computes hash of Block
 * @block: points to Block to be hashed
 * @hash_buf: destination for resulting hash
 * Return: pointer to hash_buf
 */
uint8_t *block_hash(
	block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buff = NULL;
	uint64_t tx_count = 0, base_len = 0, expect_len = 0;

	if (!block || !hash_buf)
		return (NULL);
	if (block->transactions)
		tx_count = llist_size(block->transactions);
	expect_len = (base_len = BASE_LEN(block)) + TX_ID_TOT(tx_count);
	buff = calloc(1, expect_len);
	if (!buff)
		return (NULL);
	memcpy(buff, block, base_len);
	if (block->transactions && llist_size(block->transactions))
		llist_for_each(block->transactions, (node_func_t)&add_tx, &buff[base_len]);
	SHA256(buff, expect_len, hash_buf);
	FREE_0(buff);
	return (hash_buf);
}

/**
 * add_tx - adds transaction data to buffer for hashing
 * @tx: transaction data node
 * @iter: index of transaction within transaction list
 * @buff: buffer for insertion of transaction ID
 * Return: always 0
 */
static int add_tx(transaction_t *tx, uint32_t iter, uint8_t *buff)
{
	memcpy(&buff[TX_IDX(iter)], tx->id, SHA256_DIGEST_LENGTH);
	return (0);
}
