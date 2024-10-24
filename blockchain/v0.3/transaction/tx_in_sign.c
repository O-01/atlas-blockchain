#include "transaction.h"

static int get_uto(unspent_tx_out_t *uto, uint8_t *buff);

/**
 * tx_in_sign - signs transaction input, given transaction id it is from
 * @in: points to transaction input structure to sign
 * @tx_id: contains ID (hash) of transaction that transaction input to sign is
 *         stored in
 * @sender: contains private key of receiver of coins contained in transaction
 *          output referenced by transaction input
 * @all_unspent: list of all unspent transaction outputs to date
 * Return: pointer to result signature structure upon success, otherwise NULL
 */
sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN] = {0};
	unspent_tx_out_t *uto = NULL;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);
	uto = llist_find_node(all_unspent, (node_ident_t)&get_uto, in->tx_out_hash);
	if (!uto)
		return (NULL);
	ec_to_pub(sender, pub);
	if (memcmp(pub, uto->out.pub, EC_PUB_LEN))
		return (NULL);
	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);
	return (&in->sig);
}

/**
 * get_uto - callback for locating unspent transaction output with matching
 *           output hash
 * @uto: unspent transaction output node
 * @buff: buff containing hash that must be matched
 * Return: 1 if output hash matches hash from buff, otherwise 0
 */
static int get_uto(unspent_tx_out_t *uto, uint8_t *buff)
{
	if (!memcmp(uto->out.hash, buff, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}
