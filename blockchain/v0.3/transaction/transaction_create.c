#include "blockchain.h"

int get_sender_utos(llist_node_t node, unsigned int iter, void *proc);
int make_input(llist_node_t node,
	unsigned int iter __attribute__((unused)), void *inputs);
int sign_input(llist_node_t node,
	unsigned int iter __attribute__((unused)), void *proc);
void *free_proc(tx_proc_t *proc);

/**
 * transaction_create - creates transaction
 * @sender: contains private key of transaction sender
 * @receiver: contains public key of transaction receiver
 * @amount: amount to send
 * @all_unspent: list of all unspent outputs to date
 * Return: pointer to created transaction upon success, otherwise NULL
 */
transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent)
{
	transaction_t *add = NULL;
	uint8_t receiver_pub[EC_PUB_LEN] = {0};
	tx_proc_t *proc = NULL;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);
	proc = calloc(1, sizeof(tx_proc_t));
	if (!proc)
		return (NULL);
	add = calloc(1, sizeof(transaction_t));
	if (!add)
		return (FREE_0(proc), NULL);
	ec_to_pub(sender, proc->sender_pub), ec_to_pub(receiver, receiver_pub);
	proc->bal = 0;
	proc->sender = sender;
	proc->sender_utos = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, get_sender_utos, proc);
	if (proc->bal < amount)
		return (free_proc(proc), NULL);
	add->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(proc->sender_utos, make_input, add->inputs);
	add->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(
		add->outputs, tx_out_create(amount, receiver_pub), ADD_NODE_REAR);
	proc->bal -= amount;
	if (proc->bal > 0)
		llist_add_node(add->outputs,
			tx_out_create(proc->bal, proc->sender_pub), ADD_NODE_REAR);
	transaction_hash(add, add->id);
	memcpy(proc->tx_id, add->id, SHA256_DIGEST_LENGTH);
	llist_for_each(add->inputs, sign_input, proc);
	return (free_proc(proc), add);
}

/**
 * get_sender_utos - retrieves all unspent transaction outputs that match
 *                   sender's public key
 * @node: unspent transaction output node
 * @iter: unused iterator
 * @proc: transaction processing data struct
 * Return: always 0
 */
int get_sender_utos(
	llist_node_t node, unsigned int iter __attribute__((unused)), void *proc)
{
	if (!memcmp(UTO_OUT_PUB_ADDR((uint8_t *)node), (uint8_t *)proc, EC_PUB_LEN))
	{
		llist_add_node(((tx_proc_t *)proc)->sender_utos, node, ADD_NODE_REAR);
		((tx_proc_t *)proc)->bal += ((unspent_tx_out_t *)node)->out.amount;
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
int make_input(
	llist_node_t node, unsigned int iter __attribute__((unused)), void *inputs)
{
	llist_add_node((llist_t *)inputs,
		tx_in_create((unspent_tx_out_t *)node), ADD_NODE_REAR);
	return (0);
}

/**
 * sign_input - calls tx_in_sign to sign transaction input
 * @node: transaction input to be signed
 * @iter: unused iterator
 * @proc: transaction processing data
 * Return: always 0
 */
int sign_input(llist_node_t node,
	unsigned int iter __attribute__((unused)), void *proc)
{
	tx_in_sign((tx_in_t *)node, ((tx_proc_t *)proc)->tx_id,
		((tx_proc_t *)proc)->sender, ((tx_proc_t *)proc)->sender_utos);
	return (0);
}

/**
 * free_proc - frees allocated memory related to transaction processing data
 * @proc: transaction processing data
 * Return: always NULL
 */
void *free_proc(tx_proc_t *proc)
{
	if (!proc)
		return (NULL);
	if (proc->sender_utos)
		llist_destroy(proc->sender_utos, 0, NULL);
	FREE_0(proc);
	return (NULL);
}
