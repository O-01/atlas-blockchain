#include "blockchain.h"

void tx_destroy(void *tx);

/**
 * block_destroy - deletes existing Block
 * @block: points to Block to delete
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	if (block->transactions)
		llist_destroy(block->transactions, 1, tx_destroy);
	FREE_0(block);
}

/**
 * tx_destroy - deallocates tx structure
 * @tx: points to tx to delete
 */
void tx_destroy(void *tx)
{
	if (!tx)
		return;
	if (TX(tx)->inputs)
		llist_destroy(TX(tx)->inputs, (llist_size(TX(tx)->inputs) > 0), NULL);
	if (TX(tx)->outputs)
		llist_destroy(TX(tx)->outputs, (llist_size(TX(tx)->outputs) > 0), NULL);
	FREE_0(tx);
}
