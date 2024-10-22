#include "transaction.h"

int verify_input(llist_node_t input, unsigned int iter, void *utos);
int input_match(llist_node_t uto, void *input);
int get_out_total(llist_node_t output, unsigned int iter, void *out_total);

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
		llist_for_each(transaction->inputs, verify_input, &dat))
		return (0);
	llist_for_each(transaction->outputs, get_out_total, &out_total);
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
int verify_input(llist_node_t input, unsigned int iter, void *dat)
{
	unspent_tx_out_t *found = NULL;
	EC_KEY *key = NULL;

	(void)iter;
	found = llist_find_node(DAT(dat)->utos, input_match, input);
	if (!found)
		return (1);
	DAT(dat)->bal += found->out.amount;
	key = ec_from_pub(UTO(found)->out.pub);
	if (!key)
		return (1);
	if (!ec_verify(key, DAT(dat)->tx_id, SHA256_DIGEST_LENGTH, &INPUT(input)->sig))
		return (EC_KEY_free(key), 1);
	return (EC_KEY_free(key), 0);
}

/**
 * input_match - finds unspent transaction output that matches input
 * @uto: unspent transaction output node
 * @input: transaction input against which unspent transaction output verified
 * Return: 1 if match found, otherwise 0
 */
int input_match(llist_node_t uto, void *input)
{
	if (!memcmp(UTO(uto)->block_hash, INPUT(input)->block_hash,
			SHA256_DIGEST_LENGTH) &&
		!memcmp(UTO(uto)->tx_id, INPUT(input)->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(UTO(uto)->out.hash, INPUT(input)->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * get_out_total - retrieves total amount from transaction outputs
 * @output: transaction output to to extract amount from
 * @iter: unused iterator
 * @out_total: total amount from all transaction outputs
 */
int get_out_total(llist_node_t output, unsigned int iter, void *out_total)
{
	(void)iter;
	*((uint32_t *)out_total) += OUT(output)->amount;
	return (0);
}
