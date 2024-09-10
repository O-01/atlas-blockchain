#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * ec_save - saves existing EC key pair on disk
 * @key: points to EC key pair to be saved on disk
 * @folder: path to folder in which to save keys
 * Return: 1 upon success, 0 upon failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	struct stat info = {0};
	char key_path[PATH_MAX] = {0}, key_pub_path[PATH_MAX] = {0};
	FILE *stream = NULL;

	if (!key || !folder)
		return (0);
	if (lstat(folder, &info) == -1 || !S_ISDIR(info.st_mode))
		mkdir(folder, 0755);
	memcpy(key_path, folder, strlen(folder));
	memcpy(&key_path[strlen(folder)], "/key.pem", 9);
	memcpy(key_pub_path, folder, strlen(folder));
	memcpy(&key_pub_path[strlen(folder)], "/key_pub.pem", 12);
	stream = fopen(key_pub_path, "w+");
	PEM_write_EC_PUBKEY(stream, key);
	fclose(stream);
	stream = fopen(key_path, "w+");
	PEM_write_ECPrivateKey(stream, key, NULL, NULL, 0, NULL, NULL);
	fclose(stream);
	return (1);
}
