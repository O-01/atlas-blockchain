#include "blockchain.h"

static uint32_t last_adj_idx(blockchain_t const *blockchain);

/**
 * blockchain_difficulty - computes difficulty to assign to potential next
 *                         Block in Blockchain
 * @blockchain: points to Blockchain to analyze
 * Return: difficulty to be assigned to a potential next Block in Blockchain
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *block = NULL, *last_adj = NULL;

	if (!blockchain)
		return (0);
	block = llist_get_tail(blockchain->chain);
	if (!block->info.index ||
		block->info.index < DIFFICULTY_ADJUSTMENT_INTERVAL ||
		NO_INTERVAL_ADJUSTMENT(block->info.index))
		return (block->info.difficulty);
	last_adj = llist_get_node_at(blockchain->chain, last_adj_idx(blockchain));
	if (ACTUAL(block, last_adj) < (EXPECTED >> 1))
		return (block->info.difficulty + 1);
	else if (ACTUAL(block, last_adj) > (EXPECTED << 1))
		return (block->info. difficulty - 1);
	return (block->info.difficulty);
}

/**
 * last_adj_idx - retrieves index of last block with adjusted difficulty
 * @blockchain: points to Blockchain to search in
 * Return: index of last block with adjusted difficulty
 */
static uint32_t last_adj_idx(blockchain_t const *blockchain)
{
	uint32_t size = 0;

	if (!blockchain)
		return (-1);
	size = llist_size(blockchain->chain);
	return (LAST_ADJUSTED(size));
}
