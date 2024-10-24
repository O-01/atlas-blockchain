#include "blockchain.h"

static uint8_t load_blocks(blockchain_t *bc, uint32_t count, FILE *stream);
static uint8_t load_tx(block_t *block, uint32_t count, FILE *stream);
static uint8_t load_in(transaction_t *tx, uint32_t count, FILE *stream);
static uint8_t load_out(transaction_t *tx, uint32_t count, FILE *stream);
static uint8_t load_utos(blockchain_t *bc, uint32_t count, FILE *stream);

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
	FILE *stream = NULL;
	uint8_t f_head[16];
	uint32_t block_count = 0, uto_count = 0;

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
		return (blockchain_destroy(blockchain), fclose(stream), NULL);
	fread(&block_count, sizeof(uint32_t), 1, stream);
	fread(&uto_count, sizeof(uint32_t), 1, stream);
	if (load_blocks(blockchain, block_count, stream))
		return (blockchain_destroy(blockchain), fclose(stream), NULL);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
		return (blockchain_destroy(blockchain), fclose(stream), NULL);
	if (load_utos(blockchain, uto_count, stream))
		return (blockchain_destroy(blockchain), fclose(stream), NULL);
	fclose(stream);
	return (blockchain);
}

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * load_blocks - deserialize blocks from file
 * @bc: blockchain data structure containing list of blocks to load into
 * @count: number of expected blocks to be loaded
 * @stream: file stream from which to read data
 * Return: 0 upon success, otherwise 1
 */
static uint8_t load_blocks(blockchain_t *bc, uint32_t count, FILE *stream)
{
	uint32_t iter = 0;
	int32_t tx_count = 0;
	block_t *block = NULL;

	if (!bc || !count || !stream)
		return (1);
	for (; iter < count; ++iter)
	{
		block = calloc(1, sizeof(block_t));
		if (!block)
			return (1);
		fread(&block->info, sizeof(block_info_t), 1, stream);
		fread(&block->data.len, sizeof(uint32_t), 1, stream);
		fread(block->data.buffer, block->data.len, 1, stream);
		fread(block->hash, SHA256_DIGEST_LENGTH, 1, stream);
		fread(&tx_count, sizeof(int32_t), 1, stream);
		if (tx_count >= 0)
			block->transactions = llist_create(MT_SUPPORT_FALSE),
			load_tx(block, tx_count, stream);
		llist_add_node(bc->chain, block, ADD_NODE_REAR);
	}
	return (0);
}

/**
 * load_tx - deserialize transactions from file
 * @block: block data structure containing list of transactions to load into
 * @count: number of expected transactions to be loaded
 * @stream: file stream from which to read data
 * Return: 0 upon success, otherwise 1
 */
static uint8_t load_tx(block_t *block, uint32_t count, FILE *stream)
{
	uint32_t iter = 0, in_count = 0, out_count = 0;
	transaction_t *tx = NULL;

	if (!block || !count || !stream)
		return (1);
	for (; iter < count; ++iter)
	{
		tx = calloc(1, sizeof(transaction_t));
		if (!tx)
			return (1);
		fread(tx->id, SHA256_DIGEST_LENGTH, 1, stream);
		fread(&in_count, sizeof(uint32_t), 1, stream);
		fread(&out_count, sizeof(uint32_t), 1, stream);
		tx->inputs = llist_create(MT_SUPPORT_FALSE);
		tx->outputs = llist_create(MT_SUPPORT_FALSE);
		if (load_in(tx, in_count, stream))
			return (1);
		if (load_out(tx, out_count, stream))
			return (1);
		llist_add_node(block->transactions, tx, ADD_NODE_REAR);
	}
	return (0);
}

/**
 * load_in - deserialize transaction inputs from file
 * @tx: transaction data structure containing list of transaction inputs to
 *      load into
 * @count: number of expected transaction inputs to be loaded
 * @stream: file stream from which to read data
 * Return: 0 upon success, otherwise 1
 */
static uint8_t load_in(transaction_t *tx, uint32_t count, FILE *stream)
{
	uint32_t iter = 0;
	tx_in_t *in = NULL;

	if (!tx || !count || !stream)
		return (1);
	for (; iter < count; ++iter)
	{
		in = calloc(1, sizeof(tx_in_t));
		if (!in)
			return (1);
		fread(in->block_hash, SHA256_DIGEST_LENGTH, 1, stream);
		fread(in->tx_id, SHA256_DIGEST_LENGTH, 1, stream);
		fread(in->tx_out_hash, SHA256_DIGEST_LENGTH, 1, stream);
		fread(in->sig.sig, SIG_MAX_LEN, 1, stream);
		fread(&in->sig.len, sizeof(uint8_t), 1, stream);
		llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	}
	return (0);
}

/**
 * load_out - deserialize transaction outputs from file
 * @tx: transaction data structure containing list of transaction outputs to
 *      load into
 * @count: number of expected transaction outputs to be loaded
 * @stream: file stream from which to read data
 * Return: 0 upon success, otherwise 1
 */
static uint8_t load_out(transaction_t *tx, uint32_t count, FILE *stream)
{
	uint32_t iter = 0;
	tx_out_t *out = NULL;

	if (!tx || !count || !stream)
		return (1);
	for (; iter < count; ++iter)
	{
		out = calloc(1, sizeof(tx_out_t));
		if (!out)
			return (1);
		fread(&out->amount, sizeof(uint32_t), 1, stream);
		fread(out->pub, EC_PUB_LEN, 1, stream);
		fread(out->hash, SHA256_DIGEST_LENGTH, 1, stream);
		llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	}
	return (0);
}

/**
 * load_utos - deserialize unspent transaction outputs from file
 * @bc: blockchain data structure containing list of unspent transaction
 *      outputs to load into
 * @count: number of expected unspent transaction outputs to be loaded
 * @stream: file stream from which to read data
 * Return: 0 upon success, otherwise 1
 */
static uint8_t load_utos(blockchain_t *bc, uint32_t count, FILE *stream)
{
	uint32_t iter = 0;
	unspent_tx_out_t *uto = NULL;

	if (!bc || !count || !stream)
		return (1);
	for (; iter < count; ++iter)
	{
		uto = calloc(1, sizeof(unspent_tx_out_t));
		if (!uto)
			return (1);
		fread(uto->block_hash, SHA256_DIGEST_LENGTH, 1, stream);
		fread(uto->tx_id, SHA256_DIGEST_LENGTH, 1, stream);
		fread(&uto->out.amount, sizeof(uint32_t), 1, stream);
		fread(uto->out.pub, EC_PUB_LEN, 1, stream);
		fread(uto->out.hash, SHA256_DIGEST_LENGTH, 1, stream);
		llist_add_node(bc->unspent, uto, ADD_NODE_REAR);
	}
	return (0);
}
