#include "hblk_crypto.h"

/**
 * ec_load - loads EC key pair from disk
 * @folder: path to folder from which to load keys
 * Return: pointer to created EC key pair upon success, otherwise NULL
 */
EC_KEY *ec_load(char const *folder)
{
	struct stat info = {0};
	EC_KEY *key = NULL;
	char key_path[PATH_MAX] = {0}, key_pub_path[PATH_MAX] = {0};
	FILE *stream = NULL;

	if (!folder || lstat(folder, &info) == -1 || !S_ISDIR(info.st_mode))
		return (NULL);
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	memcpy(key_path, folder, strlen(folder));
	memcpy(&key_path[strlen(folder)], "/key.pem", 9);
	memcpy(key_pub_path, folder, strlen(folder));
	memcpy(&key_pub_path[strlen(folder)], "/key_pub.pem", 12);
	stream = fopen(key_pub_path, "r");
	if (!stream)
		return (EC_KEY_free(key), fclose(stream), NULL);
	PEM_read_EC_PUBKEY(stream, &key, NULL, NULL);
	fclose(stream);
	stream = fopen(key_path, "r");
	if (!stream)
		return (EC_KEY_free(key), fclose(stream), NULL);
	PEM_read_ECPrivateKey(stream, &key, NULL, NULL);
	fclose(stream);
	return (key);
}
