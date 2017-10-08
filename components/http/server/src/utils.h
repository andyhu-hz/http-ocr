#pragma once

#include <string>
#include "reply.hpp"
#include "request.hpp"

namespace timax
{
	bool iequal(const char* src, size_t src_len, const char* dest, size_t dest_len);
	std::string http_date(time_t t);
	char *http_date(char *res, time_t t);


	template<typename T>
	void integral_to_hex_str(T n, char *s)
	{
		static_assert(std::is_integral<T>::value, "Param n must be integral!");
		const char hex_lookup[] = "0123456789abcdef";
		int len = 0;
		if (n == 0)
		{
			len = 1;
		}
		for (auto tmp = n; tmp != 0; tmp >>= 4)
		{
			++len;
		}

		s[len] = '\0';

		for (--len; len >= 0; n >>= 4, --len)
		{
			s[len] = hex_lookup[n & 0xf];
		}
	}

    reply reply_static_file(std::string const& static_path, request const& req);

	inline int htoi(int c1, int c2)
	{
		int value;
		int c;

		c = c1;
		if (isupper(c))
			c = tolower(c);
		value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

		c = c2;
		if (isupper(c))
			c = tolower(c);
		value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

		return (value);
	}

	size_t base64_encode(char *_dst, const void *_src, size_t len, int url_encoded);

	bool is_valid_utf8(unsigned char *s, size_t length);

}




