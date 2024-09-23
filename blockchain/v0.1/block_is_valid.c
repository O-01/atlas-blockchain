#include "blockchain.h"

static int is_genesis(block_t const *block);
void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * block_is_valid - verifies that Block is valid
 * @block: points to Block to check
 * @prev_block: points to previous Block in Blockchain, NULL if block is first
 *              Block of chain
 * Return: 1 if valid, otherwise 0
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block || (!prev_block && block->info.index) ||
		(prev_block && block->info.index != prev_block->info.index + 1))
		return (1);
	if (!block->info.index)
		return (is_genesis(block));
	block_hash(prev_block, prev_hash);
	block_hash(block, current_hash);
	return (memcmp(prev_block->hash, prev_hash, SHA256_DIGEST_LENGTH) ||
		memcmp(block->hash, current_hash, SHA256_DIGEST_LENGTH));
}

/**
 * is_genesis - checks if current block is genesis block
 * @block: pointer to Block to verify against genesis data
 * Return: 1 if matches genesis data, otherwise 0
 */
static int is_genesis(block_t const *block)
{
	block_t genesis = {
		{0, 0, 1537578000, 0, {0}},
		{"Holberton School", 16},
		HOLBERTON_SCHOOL_SHA256};
	return (memcmp(block, &genesis, 1116));
}
