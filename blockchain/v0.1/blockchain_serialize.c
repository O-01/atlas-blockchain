#include "blockchain.h"

int write_block(llist_node_t list, uint32_t index, void *arg);

/**
 * blockchain_serialize - serializes Blockchain into file
 * @blockchain: points to Blockchain to be serialized
 * @path: contains path to file to serialize Blockchain into - truncated or
 *        created as necessary
 * Return: 0 upon success, otherwise -1 upon any NULL argument
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *stream = NULL;
	uint8_t f_head[12] = {0};
	int block_count = 0;

	if (!blockchain || !path)
		return (-1);
	block_count = llist_size(blockchain->chain);
	stream = fopen(path, "w");
	memcpy(f_head, HBLK VERS, 8);
	memcpy(&f_head[7], ENDIAN, 1);
	memcpy(&f_head[8], &block_count, sizeof(int));
	fwrite(f_head, 12, 1, stream);
	llist_for_each(blockchain->chain, write_block, stream);
	fclose(stream);
	return (0);
}

/**
 * write_block - writes block information to `FILE *` stream
 * @list: pointer to current block node
 * @index: index of current node (unused)
 * @arg: pointer to arg passed through llist_for_each - expected to be `FILE *`
 * Return: 0 upon success, otherwise -1 upon NULL node or arg inputs
 */
int write_block(
	llist_node_t list, __attribute__((unused)) uint32_t index, void *arg)
{
	uint8_t buff[1116] = {0};
	uint32_t len = 0, total = 0;

	if (!list || !arg)
		return (-1);
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(list);
	memcpy(buff, list, sizeof(block_info_t));
	total = ((block_t *)list)->data.len;
	memcpy(&buff[len += sizeof(block_info_t)], &total, sizeof(uint32_t));
	memcpy(&buff[len += sizeof(uint32_t)], ((block_t *)list)->data.buffer, total);
	memcpy(&buff[len += total], ((block_t *)list)->hash, SHA256_DIGEST_LENGTH);
	fwrite(buff, len += SHA256_DIGEST_LENGTH, 1, (FILE *)arg);
	return (0);
}
