#include "blockchain.h"

/**
 * blockchain_create - creates and initializes add structure
 * Return: created and initialized blockchain structure upon success,
 *         otherwise NULL
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *add = NULL;
	block_t *block = NULL;
	block_info_t inf = {0, 0, 1537578000, 0, {0}};
	block_data_t dat = {"Holberton School", 16};

	add = calloc(1, sizeof(blockchain_t));
	if (!add)
		return (NULL);
	add->chain = llist_create(MT_SUPPORT_FALSE);
	if (!add->chain)
		return (FREE_0(add));
	block = calloc(1, sizeof(block_t));
	if (!block)
		return (llist_destroy(add->chain, 0, NULL), FREE_0(add));
	block->info = inf;
	block->data = dat;
	memcpy(block->hash, HOLBERTON_SCHOOL_SHA256, SHA256_DIGEST_LENGTH);
	if (llist_add_node(add->chain, block, ADD_NODE_REAR) == -1)
		return (llist_destroy(add->chain, 0, NULL), FREE_0(add));
	return (add);
}
