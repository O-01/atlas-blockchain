#include "blockchain.h"

/**
 * block_destroy - deletes existing Block
 * @block: points to Block to delete
 */
void block_destroy(block_t *block) { FREE_0(block); }
