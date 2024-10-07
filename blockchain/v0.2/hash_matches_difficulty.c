#include "blockchain.h"

/**
 * hash_matches_difficulty - checks whether given hash matches given difficulty
 * @hash: hash to check
 * @difficulty: minimum difficulty hash should match
 * Return: 1 if difficulty is respected, otherwise 0
 */
int hash_matches_difficulty(
	uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty)
{
	uint32_t count = 0, mark = 7;

	if (!hash)
		return (0);
	for (; count < difficulty + 1; ++count, mark = (mark - 1) & 7)
		if ((hash[count / 8] & (1 << mark)))
			break;
	return (count == difficulty);
}
