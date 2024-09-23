#include "blockchain.h"

/**
 * blockchain_deserialize - deserializes Blockchain from file
 * @path: contains path to file to load Blockchain from
 * Return: pointer to deserialized Blockchain upon success, otherwise NULL
 *         Error cases:
 *                      Unable to open/read file
 *                      Incorrect magic number
 *                      Version mismatch
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	blockchain_t *blockchain = NULL;
	FILE *stream = NULL;
	uint8_t buff[1116] = {0};

	if (!path)
		return (NULL);
	stream = fopen(path, "r");
	if (!stream)
		return (NULL);
	if (!fread(buff, 7, 1, stream) || memcmp(buff, HBLK VERS, 7))
		return (fclose(stream), NULL);
	/* ; */
}
