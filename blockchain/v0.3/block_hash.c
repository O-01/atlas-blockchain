#include "blockchain.h"

int add_tx(llist_node_t tx, unsigned int iter, void *mem);

/**
 * block_hash - computes hash of Block
 * @block: points to Block to be hashed
 * @hash_buf: destination for resulting hash
 * Return: pointer to hash_buf
 */
uint8_t *block_hash(
	block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	blk_mem_t mem = {0};

	if (!block || !hash_buf)
		return (NULL);
	mem.base_len = BM_BASE_LEN(block);
	mem.buff_len = mem.base_len + TX_ID_TOT(block->transactions);
	mem.buff = calloc(1, mem.buff_len);
	if (block->transactions && llist_size(block->transactions))
		llist_for_each(block->transactions, add_tx, &mem);
	SHA256(mem.buff, mem.buff_len, hash_buf);
	FREE_0(mem.buff);
	return (hash_buf);
}

/**
 * add_tx - adds transaction data to block memory buffer for hashing
 * @tx: transaction data node
 * @iter: index of transaction within transaction list
 * @mem: memory structure containing buffer for insertion of transaction ID
 * Return: always 0
 */
int add_tx(llist_node_t tx, unsigned int iter, void *mem)
{
	memcpy(&BM(mem)->buff[BM(mem)->base_len + TX_IDX(iter)],
		TX(tx)->id, SHA256_DIGEST_LENGTH);
	return (0);
}
