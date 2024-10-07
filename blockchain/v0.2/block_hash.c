#include "blockchain.h"

/**
 * block_hash - computes hash of Block
 * @block: points to Block to be hashed
 * @hash_buf: destination for resulting hash
 * Return: pointer to hash_buf
 */
uint8_t *block_hash(
	block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block || !hash_buf)
		return (NULL);
	SHA256((uint8_t *)block, sizeof(block_info_t) + block->data.len, hash_buf);
	return (hash_buf);
}
