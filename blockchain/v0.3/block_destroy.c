#include "blockchain.h"

/**
 * block_destroy - deletes existing Block
 * @block: points to Block to delete
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	llist_destroy(block->transactions, 1, (node_dtor_t)&transaction_destroy);
	FREE_0(block);
}
