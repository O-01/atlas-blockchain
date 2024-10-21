#include "blockchain.h"

int get_sender_utos(llist_node_t node, unsigned int iter, void *dat);
int make_input(llist_node_t node, unsigned int iter, void *inputs);
int sign_input(llist_node_t node, unsigned int iter , void *dat);
void *free_proc(tx_proc_t *dat);

/**
 * transaction_create - creates transaction
 * @sender: contains private key of transaction sender
 * @receiver: contains public key of transaction receiver
 * @amount: amount to send
 * @all_unspent: list of all unspent outputs to date
 * Return: pointer to created transaction upon success, otherwise NULL
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *add = NULL;
	uint8_t rec_pub[EC_PUB_LEN] = {0};
	tx_proc_t *dat = NULL;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);
	dat = calloc(1, sizeof(tx_proc_t));
	if (!dat)
		return (NULL);
	add = calloc(1, sizeof(transaction_t));
	if (!add)
		return (FREE_0(dat), NULL);
	ec_to_pub(sender, dat->sender_pub), ec_to_pub(receiver, rec_pub);
	dat->bal = 0;
	dat->sender = sender;
	dat->sender_utos = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, get_sender_utos, dat);
	if (dat->bal < amount)
		return (free_proc(dat), NULL);
	add->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(dat->sender_utos, make_input, add->inputs);
	add->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(add->outputs, tx_out_create(amount, rec_pub), ADD_NODE_REAR);
	dat->bal -= amount;
	if (dat->bal > 0)
		llist_add_node(add->outputs, tx_out_create(dat->bal, dat->sender_pub),
			ADD_NODE_REAR);
	transaction_hash(add, add->id);
	memcpy(dat->tx_id, add->id, SHA256_DIGEST_LENGTH);
	llist_for_each(add->inputs, sign_input, dat);
	return (free_proc(dat), add);
}

/**
 * get_sender_utos - retrieves all unspent transaction outputs that match
 *                   sender's public key
 * @node: unspent transaction output node
 * @iter: unused iterator
 * @dat: transaction processing data struct
 * Return: always 0
 */
int get_sender_utos(llist_node_t node, unsigned int iter, void *dat)
{
	(void)iter;
	if (!memcmp(UTO_OUT_PUB_ADDR((uint8_t *)node), (uint8_t *)dat, EC_PUB_LEN))
	{
		llist_add_node(((tx_proc_t *)dat)->sender_utos, node, ADD_NODE_REAR);
		((tx_proc_t *)dat)->bal += ((unspent_tx_out_t *)node)->out.amount;
	}
	return (0);
}

/**
 * make_input - creates transaction input for node and inserts into inputs list
 * @node: unspent transaction output from which to create transaction input
 * @iter: unused iterator
 * @inputs: list of transaction inputs
 * Return: always 0
 */
int make_input(llist_node_t node, unsigned int iter, void *inputs)
{
	(void)iter;
	llist_add_node((llist_t *)inputs, tx_in_create((unspent_tx_out_t *)node),
		ADD_NODE_REAR);
	return (0);
}

/**
 * sign_input - calls tx_in_sign to sign transaction input
 * @node: transaction input to be signed
 * @iter: unused iterator
 * @dat: transaction processing data
 * Return: always 0
 */
int sign_input(llist_node_t node, unsigned int iter, void *dat)
{
	(void)iter;
	tx_in_sign((tx_in_t *)node, ((tx_proc_t *)dat)->tx_id,
		((tx_proc_t *)dat)->sender, ((tx_proc_t *)dat)->sender_utos);
	return (0);
}

/**
 * free_proc - frees allocated memory related to transaction processing data
 * @dat: transaction processing data
 * Return: always NULL
 */
void *free_proc(tx_proc_t *dat)
{
	if (!dat)
		return (NULL);
	if (dat->sender_utos)
		llist_destroy(dat->sender_utos, 0, NULL);
	FREE_0(dat);
	return (NULL);
}
