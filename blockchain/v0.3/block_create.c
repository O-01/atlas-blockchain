#include "blockchain.h"

/**
 * block_create - creates Block structure and initializes it
 * @prev: pointer to previous Block in Blockchain
 * @data: points to memory area to duplicate in Block’s data
 * @data_len: stores number of bytes to duplicate in data - if data_len is
 *            bigger than BLOCKCHAIN_DATA_MAX, then only BLOCKCHAIN_DATA_MAX
 *            bytes must be duplicated
 * Return: pointer to allocated Block
 */
block_t *block_create(
	block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *add = NULL;
	block_info_t inf = {
		prev ? prev->info.index + 1 : 0, 0, time(NULL), 0, {0}};
	block_data_t dat = {{0}, MIN(data_len, BLOCKCHAIN_DATA_MAX)};

	add = calloc(1, sizeof(block_t));
	if (!add)
		return (NULL);
	if (prev)
		memcpy(inf.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	add->info = inf;
	if (data)
		memcpy(dat.buffer, data, MIN(data_len, BLOCKCHAIN_DATA_MAX));
	add->data = dat;
	memset(add->hash, '\x0', SHA256_DIGEST_LENGTH);
	add->transactions = llist_create(MT_SUPPORT_FALSE);
	return (add);
}
