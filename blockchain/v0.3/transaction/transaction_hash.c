#include "blockchain.h"

int input_to_buffer(llist_node_t node, unsigned int iter, void *buffer);
int output_to_buffer(llist_node_t node, unsigned int iter, void *buffer);

/**
 * transaction_hash - computes ID (hash) of transaction
 * @transaction: points to transaction to compute hash of
 * @hash_buf: buffer in which to store computed hash
 * Return: pointer to hash_buf
 */
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buffer = NULL, *input_buffer = NULL, *output_buffer = NULL;
	uint32_t input_count = 0, output_count = 0, buffer_len = 0;

	if (!transaction || !hash_buf)
		return (NULL);
	input_count = llist_size(transaction->inputs);
	output_count = llist_size(transaction->outputs);
	buffer_len = IN_SZ(input_count) + OUT_SZ(output_count);
	input_buffer = calloc(1, IN_SZ(input_count));
	if (!input_buffer)
		return (NULL);
	output_buffer = calloc(1, OUT_SZ(output_count));
	if (!output_buffer)
		return (FREE_0(input_buffer), NULL);
	llist_for_each(transaction->inputs, input_to_buffer, input_buffer);
	llist_for_each(transaction->outputs, output_to_buffer, output_buffer);
	buffer = calloc(1, buffer_len);
	if (!buffer)
		return (FREE_0(output_buffer), FREE_0(input_buffer), NULL);
	memcpy(buffer, input_buffer, IN_SZ(input_count));
	memcpy(&buffer[IN_SZ(input_count)], output_buffer, OUT_SZ(output_count));
	SHA256(buffer, buffer_len, hash_buf);
	FREE_0(output_buffer), FREE_0(input_buffer), FREE_0(buffer);
	return (hash_buf);
}

/**
 * input_to_buffer - transfers transaction input block_hash, tx_id, and
 *                tx_out_hash to buffer
 * @node: transaction input llist node
 * @iter: index within transaction input llist
 * @buffer: buffer to be written with transaction input data
 * Return: 0 upon success, otherwise 1
 */
int input_to_buffer(llist_node_t node, unsigned int iter, void *buffer)
{
	if (memcpy(&((uint8_t *)buffer)[IN_SZ(iter)], node, TX_IN_HASH_BLK_SZ))
		return (0);
	return (1);
}

/**
 * output_to_buffer - transfers transaction output hash to buffer
 * @node: transaction output llist node
 * @iter: index within transaction output llist
 * @buffer: buffer to be written with transaction output hash
 * Return: 0 upon success, otherwise 1
 */
int output_to_buffer(llist_node_t node, unsigned int iter, void *buffer)
{
	if (memcpy(&((u_int8_t *)buffer)[OUT_SZ(iter)],
		OUT_HASH_ADDR((uint8_t *)node), SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}
