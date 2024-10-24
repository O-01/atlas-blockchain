#include "blockchain.h"

static int write_block(block_t *block, uint32_t index, FILE *stream);
static int write_tx(transaction_t *tx, uint32_t index, FILE *stream);
static int write_in(tx_in_t *in, uint32_t index, FILE *stream);
static int write_out(tx_out_t *out, uint32_t index, FILE *stream);
static int write_uto(unspent_tx_out_t *uto, uint32_t index, FILE *stream);

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
	uint8_t f_head[16] = {0};
	uint32_t block_count = 0, uto_count = 0;

	if (!blockchain || !path)
		return (-1);
	stream = fopen(path, "w");
	block_count = llist_size(blockchain->chain);
	uto_count = llist_size(blockchain->unspent);
	memcpy(f_head, HBLK VERS, 7);
	memcpy(&f_head[7], ENDIAN, 1);
	memcpy(&f_head[8], &block_count, sizeof(uint32_t));
	memcpy(&f_head[12], &uto_count, sizeof(uint32_t));
	fwrite(f_head, 16, 1, stream);
	llist_for_each(blockchain->chain, (node_func_t)&write_block, stream);
	llist_for_each(blockchain->unspent, (node_func_t)&write_uto, stream);
	fclose(stream);
	return (0);
}

/**
 * write_block - writes block information to `FILE *` stream
 * @block: pointer to current block node
 * @index: index of current node (unused)
 * @stream: pointer to file stream passed through llist_for_each
 * Return: 0 upon success, otherwise -1 upon NULL block
 */
static int write_block(block_t *block, uint32_t index, FILE *stream)
{
	uint8_t buf[1120] = {0};
	uint32_t len = 0, total = 0, txs = 0;

	if (!block)
		return (-1);
	(void)index;
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(block);
	memcpy(&buf[len], block, sizeof(block_info_t));
	total = block->data.len, txs = llist_size(block->transactions);
	memcpy(&buf[len += sizeof(block_info_t)], &total, sizeof(uint32_t));
	memcpy(&buf[len += sizeof(uint32_t)], block->data.buffer, total);
	memcpy(&buf[len += total], block->hash, SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], &txs, sizeof(uint32_t));
	fwrite(buf, len, 1, stream);
	llist_for_each(block->transactions, (node_func_t)&write_tx, stream);
	return (0);
}

/**
 * write_tx - writes transaction information to `FILE *` stream
 * @tx: pointer to current transaction node
 * @index: index of current node (unused)
 * @stream: pointer to file stream passed through llist_for_each
 * Return: 0 upon success, otherwise -1 upon NULL transaction
 */
static int write_tx(transaction_t *tx, uint32_t index, FILE *stream)
{
	uint8_t buf[40] = {0};
	uint32_t len = 0, ins = 0, outs = 0;

	if (!tx)
		return (-1);
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(tx);
	memcpy(&buf[len], tx->id, SHA256_DIGEST_LENGTH);
	ins = llist_size(tx->inputs), outs = llist_size(tx->outputs);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], &ins, sizeof(uint32_t));
	memcpy(&buf[len += sizeof(uint32_t)], &outs, sizeof(uint32_t));
	fwrite(buf, len, 1, stream);
	llist_for_each(tx->inputs, (node_func_t)&write_in, stream);
	llist_for_each(tx->outputs, (node_func_t)&write_out, stream);
	return (0);
}


/**
 * write_in - writes transaction input information to `FILE *` stream
 * @in: pointer to current transaction input node
 * @index: index of current node (unused)
 * @stream: pointer to file stream passed through llist_for_each
 * Return: 0 upon success, otherwise -1 upon NULL transaction input
 */
static int write_in(tx_in_t *in, uint32_t index, FILE *stream)
{
	uint8_t buf[169] = {0};
	uint32_t len = 0;

	if (!in)
		return (-1);
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(in);
	memcpy(&buf[len], in->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], in->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], in->tx_out_hash,
		SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], in->sig.sig, SIG_MAX_LEN);
	memcpy(&buf[len += SIG_MAX_LEN], &in->sig.len, sizeof(uint8_t));
	fwrite(buf, len, 1, stream);
	return (0);
}

/**
 * write_out - writes transaction output information to `FILE *` stream
 * @out: pointer to current transaction out node
 * @index: index of current node (unused)
 * @stream: pointer to file stream passed through llist_for_each
 * Return: 0 upon success, otherwise -1 upon NULL transaction output
 */
static int write_out(tx_out_t *out, uint32_t index, FILE *stream)
{
	uint8_t buf[101] = {0};
	uint32_t len = 0;

	if (!out)
		return (-1);
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(out);
	memcpy(&buf[len], &out->amount, sizeof(uint32_t));
	memcpy(&buf[len += sizeof(uint32_t)], out->pub, EC_PUB_LEN);
	memcpy(&buf[len += EC_PUB_LEN], out->hash, SHA256_DIGEST_LENGTH);
	fwrite(buf, len, 1, stream);
	return (0);
}

/**
 * write_uto - writes unspent transaction output information to `FILE *` stream
 * @uto: pointer to current unspent transaction output node
 * @index: index of current node (unused)
 * @stream: pointer to file stream passed through llist_for_each
 * Return: 0 upon success, otherwise -1 upon NULL unspent transaction output
 */
static int write_uto(unspent_tx_out_t *uto, uint32_t index, FILE *stream)
{
	uint8_t buf[165] = {0};
	uint32_t len = 0;

	if (!uto)
		return (-1);
	if (IS_BIG_ENDIAN)
		SWAPENDIAN(uto);
	memcpy(&buf[len], uto->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], uto->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&buf[len += SHA256_DIGEST_LENGTH], &uto->out.amount, sizeof(uint32_t));
	memcpy(&buf[len += sizeof(uint32_t)], uto->out.pub, EC_PUB_LEN);
	memcpy(&buf[len += EC_PUB_LEN], uto->out.hash, SHA256_DIGEST_LENGTH);
	fwrite(buf, len, 1, stream);
	return (0);
}
