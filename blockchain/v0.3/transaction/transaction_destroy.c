#include "blockchain.h"

/**
 * transaction_destroy - deallocates transaction structure
 * @transaction: points to transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;
	if (transaction->inputs)
		llist_destroy(
			transaction->inputs, (llist_size(transaction->inputs) > 0), NULL);
	if (transaction->outputs)
		llist_destroy(
			transaction->outputs, (llist_size(transaction->outputs) > 0), NULL);
	FREE_0(transaction);
}
