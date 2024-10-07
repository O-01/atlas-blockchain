#include "blockchain.h"

/**
 * block_mine - mines Block in order to insert it into Blockchain
 * @block: points to Block to be mined
 * MUST find hash for block that matches its difficulty
 */
void block_mine(block_t *block)
{
	uint8_t *hash = NULL;

	if (!block)
		return;
	for (hash = block_hash(block, block->hash);
		!hash_matches_difficulty(hash, block->info.difficulty);
		++block->info.nonce, hash = block_hash(block, block->hash))
		;
	memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
}
