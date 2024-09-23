#include "blockchain.h"

/**
 * blockchain_destroy - deletes existing Blockchain and all contained Blocks
 * @blockchain: points to Blockchain structure to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	llist_destroy(blockchain->chain, 1, NULL);
	FREE_0(blockchain);
}
