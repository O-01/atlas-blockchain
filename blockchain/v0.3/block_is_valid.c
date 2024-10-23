#include "blockchain.h"

static int is_genesis(block_t const *block);
int tx_valid(llist_node_t tx, unsigned int iter, void *all_utos);
void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * block_is_valid - verifies that Block is valid
 * @block: points to Block to check
 * @prev_block: points to previous Block in Blockchain, NULL if block is first
 *              Block of chain
 * @all_unspent: points to list of all unspent transaction outputs to date
 * Return: 0 if valid, otherwise non-zero value
 */
int block_is_valid(
	block_t const *block, block_t const *prev_block, llist_t *all_unspent)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};
	int valid_cb = 0;

	(void)all_unspent;
	if (!block || (!prev_block && block->info.index) ||
		(prev_block && block->info.index != prev_block->info.index + 1) ||
		!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	if (!block->info.index)
		return (is_genesis(block));
	block_hash(prev_block, prev_hash), block_hash(block, current_hash);
	valid_cb = coinbase_is_valid(
		llist_get_head(block->transactions), block->info.index);
	return (!(!memcmp(prev_block->hash, prev_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(block->info.prev_hash, prev_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(block->hash, current_hash, SHA256_DIGEST_LENGTH) &&
		!llist_for_each(block->transactions, tx_valid, all_unspent) &&
		valid_cb));
}

/**
 * is_genesis - checks if current block is genesis block
 * @block: pointer to Block to verify against genesis data
 * Return: 0 if matches genesis data, otherwise non-zero value
 */
static int is_genesis(block_t const *block)
{
	block_t genesis = {
		{0, 0, 1537578000, 0, {0}},
		{"Holberton School", 16}, 0,
		HOLBERTON_SCHOOL_SHA256};
	return (memcmp(block, &genesis, 1116));
}

/**
 * tx_valid - verifies that transaction is valid
 * @tx: transaction to be verified
 * @iter: index of transaction within list
 * @all_utos: pointer to list of all unspent transaction outputs to date
 * Return: 0 upon success, otherwise 1
 */
int tx_valid(llist_node_t tx, unsigned int iter, void *all_utos)
{
	if (iter && !transaction_is_valid(TX(tx), (llist_t *)all_utos))
		return (1);
	return (0);
}
