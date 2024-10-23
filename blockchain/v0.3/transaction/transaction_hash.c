#include "blockchain.h"

static int in_buf(tx_in_t *in, uint32_t iter, uint8_t *buf);
static int out_buf(tx_out_t *out, uint32_t iter, uint8_t *buf);

/**
 * transaction_hash - computes ID (hash) of transaction
 * @transaction: points to transaction to compute hash of
 * @hash_buf: buff in which to store computed hash
 * Return: pointer to hash_buf
 */
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buf = NULL;
	uint32_t ins = 0, outs = 0, buf_len = 0;

	if (!transaction || !hash_buf)
		return (NULL);
	ins = llist_size(transaction->inputs);
	outs = llist_size(transaction->outputs);
	buf_len = IN_SZ(ins) + OUT_SZ(outs);
	buf = calloc(1, buf_len);
	if (!buf)
		return (NULL);
	llist_for_each(transaction->inputs, (node_func_t)&in_buf, buf);
	llist_for_each(transaction->outputs, (node_func_t)&out_buf, &buf[IN_SZ(ins)]);
	SHA256(buf, buf_len, hash_buf);
	FREE_0(buf);
	return (hash_buf);
}

/**
 * in_buf - transfers transaction input block_hash, tx_id, and
 *                tx_out_hash to buff
 * @in: transaction input llist node
 * @iter: index within transaction input llist
 * @buf: buff to be written with transaction input data
 * Return: 0 upon success, otherwise 1
 */
static int in_buf(tx_in_t *in, uint32_t iter, uint8_t *buf)
{
	if (memcpy(&buf[IN_SZ(iter)], in, IN_HASH_BLK_SZ))
		return (0);
	return (1);
}

/**
 * out_buf - transfers transaction output hash to buff
 * @out: transaction output llist node
 * @iter: index within transaction output llist
 * @buf: buff to be written with transaction output hash
 * Return: 0 upon success, otherwise 1
 */
static int out_buf(tx_out_t *out, uint32_t iter, uint8_t *buf)
{
	if (memcpy(&buf[OUT_SZ(iter)], out->hash, SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}
