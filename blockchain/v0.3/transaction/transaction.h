#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <llist.h>

#include "hblk_crypto.h"

#define COINBASE_AMOUNT 50

#define IN_HASH_BLK_SZ (SHA256_DIGEST_LENGTH * 3)
#define IN_SZ(x) ((x) * IN_HASH_BLK_SZ)
#define OUT_SZ(x) ((x) * SHA256_DIGEST_LENGTH)

#define OUT_HASH_ADDR(x) ((x) + sizeof(uint32_t) + EC_PUB_LEN)
#define UTO_OUT_ADDR(x) ((x) + (SHA256_DIGEST_LENGTH * 2))
#define UTO_OUT_PUB_ADDR(x) (UTO_OUT_ADDR((x)) + sizeof(uint32_t))
#define UTO_OUT_HASH_ADDR(x) (OUT_HASH_ADDR(UTO_OUT_ADDR((x))))

#define TX(x) (((transaction_t *)(x)))
#define INPUT(x) (((tx_in_t *)(x)))
#define OUT(x) (((tx_out_t *)(x)))
#define UTO(x) (((unspent_tx_out_t *)(x)))

#define DAT(x) (((tx_proc_t *)(x)))

/**
 * struct transaction_s - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 *           Prevents further alteration of the transaction.
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
 * @hash:   Hash of @amount and @pub. Serves as output ID
 */
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct tx_in_s - Transaction input
 *
 * Description: A transaction input always refers to a previous
 * transaction output. The only exception is for a Coinbase transaction, that
 * adds new coins to ciruclation.
 *
 * @block_hash:  Hash of the Block containing the transaction @tx_id
 * @tx_id:       ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
 * @sig:         Signature. Prevents anyone from altering the content of the
 *               transaction. The transaction input is signed by the receiver
 *               of the referenced transaction output, using their private key
 */
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 *
 * Description: This structure helps identify transaction outputs that were not
 * used in any transaction input yet, making them "available".
 *
 * @block_hash: Hash of the Block containing the transaction @tx_id
 * @tx_id:      ID of the transaction containing @out
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t;

/**
 * struct tx_process_s - Transaction processing
 *
 * Description: This structure aids in the process of locating transaction
 *              inputs and outputs based on sender's private key
 *
 * @sender_pub:  Sender's public key
 * @bal:         Sender's remaining balance
 * @sender:      Sender's private key
 * @utos:        List of all unspent transaction outputs associated with
 *               sender's private key
 * @tx_id:       transaction ID
 */
typedef struct tx_process_s
{
	uint8_t         sender_pub[EC_PUB_LEN];
	uint32_t        bal;
	EC_KEY const    *sender;
	llist_t         *utos;
	uint8_t         tx_id[SHA256_DIGEST_LENGTH];
} tx_proc_t;

/**
 * struct tx_uto_update_s - Unspent transaction output list update
 *
 * Description: This structure aids in the process of updating unspent
 *              transaction outputs
 *
 * @block_hash:  Hash of validated Block that contains transaction list
 *               transactions
 * @all_utos:    List of unspent transaction outputs
 */
typedef struct tx_uto_update_s
{
	uint8_t         block_hash[SHA256_DIGEST_LENGTH];
	uint8_t         tx_id[SHA256_DIGEST_LENGTH];
	llist_t         *all_utos;
} uto_up_t;

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out);
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent);
transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent);
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent);
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);
void transaction_destroy(transaction_t *transaction);
llist_t *update_unspent(
	llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent);

#endif
