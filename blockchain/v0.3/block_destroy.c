#include "blockchain.h"

/**
 * block_destroy - deletes existing Block
 * @block: points to Block to delete
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	if (block->transactions)
		llist_destroy(block->transactions,
			(llist_size(block->transactions) > 0), transaction_destroy);
	FREE_0(block);
}
