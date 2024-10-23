#include "blockchain.h"

int add_tx(llist_node_t tx, unsigned int iter, void *buff);

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
		llist_for_each(block->transactions, add_tx, &buff[base_len]);
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
int add_tx(llist_node_t tx, unsigned int iter, void *buff)
{
	memcpy(&((uint8_t *)buff)[TX_IDX(iter)], TX(tx)->id, SHA256_DIGEST_LENGTH);
	return (0);
}
