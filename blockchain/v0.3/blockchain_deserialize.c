#include "blockchain.h"

/**
 * blockchain_deserialize - deserializes Blockchain from file
 * @path: contains path to file to load Blockchain from
 * Return: pointer to deserialized Blockchain upon success, otherwise NULL
 *         Error cases:
 *                      Unable to open/read file
 *                      Incorrect magic number
 *                      Version mismatch
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;
	FILE *stream = NULL;
	uint8_t f_head[12];
	uint32_t iter = 0, count = 0;

	if (!path)
		return (NULL);
	stream = fopen(path, "r");
	if (!stream)
		return (NULL);
	if (!fread(f_head, 8, 1, stream) || memcmp(f_head, HBLK VERS, 7))
		return (fclose(stream), NULL);
	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
		return (fclose(stream), NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
		return (FREE_0(blockchain), fclose(stream), NULL);
	for (fread(&count, sizeof(uint32_t), 1, stream); iter < count; ++iter)
	{
		block = calloc(1, sizeof(block_t));
		if (!block)
			return (blockchain_destroy(blockchain), fclose(stream), NULL);
		fread(&block->info, sizeof(block_info_t), 1, stream);
		fread(&block->data.len, sizeof(uint32_t), 1, stream);
		fread(block->data.buffer, block->data.len, 1, stream);
		fread(block->hash, SHA256_DIGEST_LENGTH, 1, stream);
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	fclose(stream);
	return (blockchain);
}
