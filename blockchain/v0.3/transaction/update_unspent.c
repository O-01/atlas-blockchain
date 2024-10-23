#include "blockchain.h"

static int tx_for_each(transaction_t *tx, uint32_t iter, uto_up_t *update);
static int input_for_each(tx_in_t *in, uint32_t iter, uto_up_t *update);
static int this_uto(unspent_tx_out_t *uto, tx_in_t *in);
static int output_append(tx_out_t *out, uint32_t iter, uto_up_t *update);

/**
 * update_unspent - updates list of all unspent transaction outputs based on
 *                  given list of processed transactions
 * @transactions: list of validated transactions
 * @block_hash: hash of validated Block that contains transaction list
 *              transactions
 * @all_unspent: current list of unspent transaction outputs
 * Return: new list of unspent transaction outputs
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	uto_up_t update = {0};

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);
	memcpy(update.block_hash, block_hash, SHA256_DIGEST_LENGTH);
	update.all_utos = all_unspent;
	llist_for_each(transactions, (node_func_t)&tx_for_each, &update);
	return (all_unspent);
}

/**
 * tx_for_each - conducts unspent transaction output list update based on data
 *               contained in each transaction node
 * @tx: transaction list node
 * @iter: unused iterator
 * @update: unspent transaction output list update processing data
 * Return: always 0
 */
static int tx_for_each(transaction_t *tx, uint32_t iter, uto_up_t *update)
{
	(void)iter;
	memcpy(update->tx_id, tx->id, SHA256_DIGEST_LENGTH);
	llist_for_each(tx->inputs, (node_func_t)&input_for_each, update);
	llist_for_each(tx->outputs, (node_func_t)&output_append, update);
	return (0);
}

/**
 * input_for_each - removes unspent transaction output if it matches
 *                  transaction input
 * @in: transaction input node
 * @iter: unused iterator
 * @update: unspent transaction output list update processing data
 * Return: always 0
 */
static int input_for_each(tx_in_t *in, uint32_t iter, uto_up_t *update)
{
	(void)iter;
	llist_remove_node(update->all_utos, (node_ident_t)&this_uto, in, 1, NULL);
	return (0);
}

/**
 * this_uto - finds unspent transaction output that matches input
 * @uto: unspent transaction output node
 * @in: transaction input against which unspent transaction output verified
 * Return: 1 if match found, otherwise 0
 */
static int this_uto(unspent_tx_out_t *uto, tx_in_t *in)
{
	if (!memcmp(uto->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(uto->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(uto->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * output_append - appends all transaction outputs as unspent transaction
 *                 outputs to unspent transaction output list
 * @out: transaction output node
 * @iter: unused iterator
 * @update: unspent transaction output list update processing data
 * Return: always 0
 */
static int output_append(tx_out_t *out, uint32_t iter, uto_up_t *update)
{
	(void)iter;
	llist_add_node(update->all_utos,
		unspent_tx_out_create(update->block_hash, update->tx_id, out),
		ADD_NODE_REAR);
	return (0);
}
