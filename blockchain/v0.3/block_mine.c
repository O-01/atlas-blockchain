#include "blockchain.h"

/**
 * block_mine - mines Block in order to insert it into Blockchain
 * @block: points to Block to be mined
 * MUST find hash for block that matches its difficulty
 */
void block_mine(block_t *block)
{
	if (!block)
		return;
	for (block_hash(block, block->hash);
		!hash_matches_difficulty(block->hash, block->info.difficulty);
		++block->info.nonce, block_hash(block, block->hash))
		;
}
