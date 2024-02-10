#include "crypto.h"

#include <openssl/evp.h>

#include <cstdio>
#include <iomanip>
#include <sstream>

namespace Lua {
namespace crypto {
static void digest(std::stringstream& output, unsigned char* hash,
                   size_t hashLength) {
	output << std::hex << std::setfill('0');

	for (int i = 0; i < hashLength; i++) {
		output << std::setw(2) << +hash[i];
	}
}

std::string md5(std::string_view input) {
	unsigned char hash[EVP_MAX_MD_SIZE];
	EVP_MD_CTX* context = EVP_MD_CTX_create();
	EVP_MD* md = EVP_MD_fetch(NULL, "MD5", NULL);

	EVP_DigestInit_ex(context, md, NULL);
	EVP_DigestUpdate(context, input.data(), input.length());
	EVP_DigestFinal_ex(context, hash, NULL);

	EVP_MD_free(md);
	EVP_MD_CTX_free(context);

	std::stringstream output;
	digest(output, hash, 16);
	return output.str();
}

std::string sha256(std::string_view input) {
	unsigned char hash[EVP_MAX_MD_SIZE];
	EVP_MD_CTX* context = EVP_MD_CTX_create();
	EVP_MD* md = EVP_MD_fetch(NULL, "SHA256", NULL);

	EVP_DigestInit_ex(context, md, NULL);
	EVP_DigestUpdate(context, input.data(), input.length());
	EVP_DigestFinal_ex(context, hash, NULL);

	EVP_MD_free(md);
	EVP_MD_CTX_free(context);

	std::stringstream output;
	digest(output, hash, 32);
	return output.str();
}
}  // namespace crypto
}  // namespace Lua
