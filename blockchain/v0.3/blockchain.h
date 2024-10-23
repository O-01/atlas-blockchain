#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <llist.h>
#include <openssl/sha.h>

#include "provided/endianness.h"
#include "transaction.h"

#define FREE_0(x) (free((x)), (x) = NULL)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define HOLBERTON_SCHOOL_SHA256 \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

#define HBLK "\x48\x42\x4c\x4b"
#define VERS "\x30\x2e\x33" /* 0.3 */

#define GET_ENDIAN (_get_endianness())
#define ENDIAN (GET_ENDIAN == 1 ? "\x01" : "\x02")
#define IS_LITTLE_ENDIAN (GET_ENDIAN == 1)
#define IS_BIG_ENDIAN (GET_ENDIAN == 2)

#define BLOCKCHAIN_DATA_MAX 1024

#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5

#define NO_INTERVAL_ADJUSTMENT(x) ((x) % DIFFICULTY_ADJUSTMENT_INTERVAL)
#define LAST_ADJ(x) (llist_size((x)) - DIFFICULTY_ADJUSTMENT_INTERVAL)

#define EXPECTED(x, y) (\
	((x)->info.index - (y)->info.index) * BLOCK_GENERATION_INTERVAL)
#define ACTUAL(x, y) ((x)->info.timestamp - (y)->info.timestamp)

#define BASE_LEN(x) (sizeof(block_info_t) + (x)->data.len)
#define TX_ID_TOT(x) ((x) * SHA256_DIGEST_LENGTH)
#define TX_IDX(x) TX_ID_TOT((x))

/**
 * struct block_info_s - Block info structure
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	/*
	 * The order of the elements in this structure should remain the same.
	 * It was designed so every element of this structure is aligned and
	 * doesn't require padding from the compiler.
	 * Therefore, it is possible to use the structure as an array of char,
	 * on any architecture.
	 */
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
	/*
	 * @buffer must stay first, so we can directly use the structure as
	 * an array of char
	 */
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_s - Block structure
 * @info: Block info
 * @data: Block data
 * @transactions: List of transactions
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info; /* This must stay first */
	block_data_t    data; /* This must stay second */
	llist_t     *transactions;
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 * @chain: Linked list of pointers to block_t
 * @unspent: Linked list of unspent transaction outputs
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;

/* v0.1 - Data structures */
blockchain_t *blockchain_create(void);
block_t *block_create(
	block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(
	block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
/* int block_is_valid(block_t const *block, block_t const *prev_block); */

/* v0.2 - Block mining */
int hash_matches_difficulty(
	uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

/* v0.3 - Transactions (Updated function prototypes) */
int block_is_valid(
	block_t const *block, block_t const *prev_block, llist_t *all_unspent);

#endif