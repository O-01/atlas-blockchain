#include "blockchain.h"

int input_to_buffer(llist_node_t node, unsigned int iter, void *buff);
int output_to_buffer(llist_node_t node, unsigned int iter, void *buff);

/**
 * transaction_hash - computes ID (hash) of transaction
 * @transaction: points to transaction to compute hash of
 * @hash_buf: buff in which to store computed hash
 * Return: pointer to hash_buf
 */
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buff = NULL;
	uint32_t ins = 0, outs = 0, buff_len = 0;

	if (!transaction || !hash_buf)
		return (NULL);
	ins = llist_size(transaction->inputs);
	outs = llist_size(transaction->outputs);
	buff_len = IN_SZ(ins) + OUT_SZ(outs);
	buff = calloc(1, buff_len);
	if (!buff)
		return (NULL);
	llist_for_each(transaction->inputs, input_to_buffer, buff);
	llist_for_each(transaction->outputs, output_to_buffer, &buff[IN_SZ(ins)]);
	SHA256(buff, buff_len, hash_buf);
	FREE_0(buff);
	return (hash_buf);
}

/**
 * input_to_buffer - transfers transaction input block_hash, tx_id, and
 *                tx_out_hash to buff
 * @node: transaction input llist node
 * @iter: index within transaction input llist
 * @buff: buff to be written with transaction input data
 * Return: 0 upon success, otherwise 1
 */
int input_to_buffer(llist_node_t node, unsigned int iter, void *buff)
{
	if (memcpy(&((uint8_t *)buff)[IN_SZ(iter)], node, IN_HASH_BLK_SZ))
		return (0);
	return (1);
}

/**
 * output_to_buffer - transfers transaction output hash to buff
 * @node: transaction output llist node
 * @iter: index within transaction output llist
 * @buff: buff to be written with transaction output hash
 * Return: 0 upon success, otherwise 1
 */
int output_to_buffer(llist_node_t node, unsigned int iter, void *buff)
{
	if (memcpy(&((u_int8_t *)buff)[OUT_SZ(iter)],
		OUT_HASH_ADDR((uint8_t *)node), SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}
