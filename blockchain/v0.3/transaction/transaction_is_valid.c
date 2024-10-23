#include "transaction.h"

static int verify_input(tx_in_t *input, uint32_t iter, tx_proc_t *dat);
static int input_match(unspent_tx_out_t *uto, tx_in_t *input);
static int get_out_total(tx_out_t *output, uint32_t iter, uint32_t *out_total);

/**
 * transaction_is_valid - checks whether transaction is valid
 * @transaction: points to transaction to verify
 * @all_unspent: list of all unspent transaction outputs to date
 * Return: 1 upon valid transaction, otherwise 0
 */
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t sample[SHA256_DIGEST_LENGTH] = {0};
	tx_proc_t dat = {0};
	uint32_t out_total = 0;

	if (!transaction || !all_unspent)
		return (0);
	transaction_hash(transaction, sample);
	memcpy(dat.tx_id, transaction->id, SHA256_DIGEST_LENGTH);
	dat.utos = all_unspent;
	if (memcmp(transaction->id, sample, SHA256_DIGEST_LENGTH) ||
		llist_for_each(transaction->inputs, (node_func_t)&verify_input, &dat))
		return (0);
	llist_for_each(transaction->outputs, (node_func_t)&get_out_total, &out_total);
	if (dat.bal != out_total)
		return (0);
	return (1);
}

/**
 * verify_input - verifies input against existing unspent transaction output
 * @input: transaction input node to be checked
 * @iter: unused iterator
 * @dat: transaction verification data containing transaction id and unspent
 *       transaction output against which transaction input verified
 * Return: 0 upon successful verification, otherwise 1
 */
static int verify_input(tx_in_t *input, uint32_t iter, tx_proc_t *dat)
{
	unspent_tx_out_t *found = NULL;
	EC_KEY *key = NULL;

	(void)iter;
	found = llist_find_node(dat->utos, (node_ident_t)&input_match, input);
	if (!found)
		return (1);
	key = ec_from_pub(found->out.pub);
	if (!key)
		return (1);
	if (!ec_verify(key, dat->tx_id, SHA256_DIGEST_LENGTH, &input->sig))
		return (EC_KEY_free(key), 1);
	dat->bal += found->out.amount;
	return (EC_KEY_free(key), 0);
}

/**
 * input_match - finds unspent transaction output that matches input
 * @uto: unspent transaction output node
 * @input: transaction input against which unspent transaction output verified
 * Return: 1 if match found, otherwise 0
 */
static int input_match(unspent_tx_out_t *uto, tx_in_t *input)
{
	if (!memcmp(uto->block_hash, input->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(uto->tx_id, input->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(uto->out.hash, input->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * get_out_total - retrieves total amount from transaction outputs
 * @output: transaction output to to extract amount from
 * @iter: unused iterator
 * @out_total: total amount from all transaction outputs
 * Return: always 0
 */
static int get_out_total(tx_out_t *output, uint32_t iter, uint32_t *out_total)
{
	(void)iter;
	*out_total += output->amount;
	return (0);
}
