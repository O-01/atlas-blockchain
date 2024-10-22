#include "blockchain.h"

/**
 * blockchain_destroy - deletes existing Blockchain and all contained Blocks
 * @blockchain: points to Blockchain structure to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->chain, 1, (node_dtor_t)&block_destroy);
	llist_destroy(blockchain->unspent, 1, NULL);
	FREE_0(blockchain);
}
