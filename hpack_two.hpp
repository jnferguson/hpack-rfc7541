// DO NOT USE - work in progress, partial decoder written

#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <utility>
#include <vector>
#include <array>
#include <deque>
#include <map>
#include <limits>
#include <string>

namespace HPACK
{
	typedef std::pair< const std::string, const std::string > header_t;
	typedef std::vector< bool > bits_t;
	
	static const std::array< header_t, 62 > predefined_headers = {
		{
			header_t("INVALIDINDEX", "INVALIDINDEX"), header_t(":authority", ""), header_t(":method", "GET"),
			header_t(":method", "POST"), header_t(":path", "/"), header_t(":path", "/index.html"),
			header_t(":scheme", "http"), header_t(":scheme", "https"), header_t(":status", "200"),
			header_t(":status", "204"), header_t(":status", "206"), header_t(":status", "304"),
			header_t(":status", "400"), header_t(":status", "404"), header_t(":status", "500"),
			header_t("accept-charset", ""), header_t("accept-encoding", "gzip, deflate"), header_t("accept-language", ""),
			header_t("accept-ranges", ""), header_t("accept", ""), header_t("access-control-allow-origin", ""),
			header_t("age", ""), header_t("allow", ""), header_t("authorization", ""),
			header_t("cache-control", ""), 	header_t("content-disposition", ""), header_t("content-encoding", ""),
			header_t("content-language", ""), header_t("content-length", ""), header_t("content-location", ""),
			header_t("content-range", ""), header_t("content-type", ""), header_t("cookie", ""),
			header_t("date", ""), header_t("etag", ""), header_t("expect", ""),
			header_t("expires", ""), header_t("from", ""), header_t("host", ""),
			header_t("if-match", ""), header_t("if-modified-since", ""), header_t("if-none-match", ""),
			header_t("if-range", ""), header_t("if-unmodified-since", ""), header_t("last-modified", ""),
			header_t("link", ""), header_t("location", ""), header_t("max-forwards", ""),
			header_t("proxy-authenticate", ""), header_t("proxy-authorization", ""), header_t("range", ""),
			header_t("referer", ""), header_t("refresh", ""), header_t("retry-after", ""),
			header_t("server", ""), header_t("set-cookie", ""), header_t("strict-transport-security", ""),
			header_t("transfer-encoding", ""), header_t("user-agent", ""), header_t("vary", ""),
			header_t("via", ""), header_t("www-authenticate", "")
		}
	};

	// 256 chars plus end of string
	static const std::array< bits_t, 257 > huffman_table = {
		{
			{ 1,1,1,1,1,1,1,1,1,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1 },
			{ 0,1,0,1,0,0 },{ 1,1,1,1,1,1,1,0,0,0 },{ 1,1,1,1,1,1,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,0,0,1 },
			{ 0,1,0,1,0,1 },{ 1,1,1,1,1,0,0,0 },{ 1,1,1,1,1,1,1,1,0,1,0 },{ 1,1,1,1,1,1,1,0,1,0 },{ 1,1,1,1,1,1,1,0,1,1 },
			{ 1,1,1,1,1,0,0,1 },{ 1,1,1,1,1,1,1,1,0,1,1 },{ 1,1,1,1,1,0,1,0 },{ 0,1,0,1,1,0 },{ 0,1,0,1,1,1 },{ 0,1,1,0,0,0 },
			{ 0,0,0,0,0 },{ 0,0,0,0,1 },{ 0,0,0,1,0 },{ 0,1,1,0,0,1 },{ 0,1,1,0,1,0 },{ 0,1,1,0,1,1 },{ 0,1,1,1,0,0 },
			{ 0,1,1,1,0,1 },{ 0,1,1,1,1,0 },{ 0,1,1,1,1,1 },{ 1,0,1,1,1,0 },{ 1,1,1,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },
			{ 1,0,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,0,1,0 },{ 1,0,0,0,0,1 },
			{ 1,0,1,1,1,0,1 },{ 1,0,1,1,1,1,0 },{ 1,0,1,1,1,1,1 },{ 1,1,0,0,0,0,0 },{ 1,1,0,0,0,0,1 },{ 1,1,0,0,0,1,0 },
			{ 1,1,0,0,0,1,1 },{ 1,1,0,0,1,0,0 },{ 1,1,0,0,1,0,1 },{ 1,1,0,0,1,1,0 },{ 1,1,0,0,1,1,1 },{ 1,1,0,1,0,0,0 },
			{ 1,1,0,1,0,0,1 },{ 1,1,0,1,0,1,0 },{ 1,1,0,1,0,1,1 },{ 1,1,0,1,1,0,0 },{ 1,1,0,1,1,0,1 },{ 1,1,0,1,1,1,0 },
			{ 1,1,0,1,1,1,1 },{ 1,1,1,0,0,0,0 },{ 1,1,1,0,0,0,1 },{ 1,1,1,0,0,1,0 },{ 1,1,1,1,1,1,0 },{ 1,1,1,0,0,1,1 },
			{ 1,1,1,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,0,0 },{ 1,0,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },{ 0,0,0,1,1 },{ 1,0,0,0,1,1 },
			{ 0,0,1,0,0 },{ 1,0,0,1,0,0 },{ 0,0,1,0,1 },{ 1,0,0,1,0,1 },{ 1,0,0,1,1,0 },{ 1,0,0,1,1,1 },{ 0,0,1,1,0 },
			{ 1,1,1,0,1,0,0 },{ 1,1,1,0,1,0,1 },{ 1,0,1,0,0,0 },{ 1,0,1,0,0,1 },{ 1,0,1,0,1,0 },{ 0,0,1,1,1 },{ 1,0,1,0,1,1 },
			{ 1,1,1,0,1,1,0 },{ 1,0,1,1,0,0 },{ 0,1,0,0,0 },{ 0,1,0,0,1 },{ 1,0,1,1,0,1 },{ 1,1,1,0,1,1,1 },{ 1,1,1,1,0,0,0 },
			{ 1,1,1,1,0,0,1 },{ 1,1,1,1,0,1,0 },{ 1,1,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
		}
	};

	class huffman_node_t
	{
		private:
		protected:
			huffman_node_t*	m_left;
			huffman_node_t* m_right;
			int16_t		m_code;
		public:
			huffman_node_t(huffman_node_t* l = nullptr, huffman_node_t* r = nullptr, int16_t c = -1) : m_left(l), m_right(r), m_code(c) { }
			virtual ~huffman_node_t(void) { m_left = nullptr; m_right = nullptr; m_code = 0; return; }
			int16_t code(void) const { return m_code; }
			void code(int16_t c) { m_code = c; return; }
			huffman_node_t* left(void) { return m_left; }
			void left(huffman_node_t* l) { m_left = l; return; }
			huffman_node_t* right(void) { return m_right; }
			void right(huffman_node_t* r) { m_right = r; return; }
	};

	class huffman_tree_t
	{
		private:
		protected:
			huffman_node_t* m_root;

			void 
			delete_node( huffman_node_t* n )
			{
				if ( nullptr != n->right() )
					delete_node(n->right());
				if ( nullptr != n->left() )
					delete_node(n->left());

				if ( nullptr == n->left() && nullptr == n->right() ) {
					delete n;
					n = nullptr;
				}

				return;
			}

		public:
			huffman_tree_t(void) : m_root(new huffman_node_t)
			{
				for ( uint64_t idx = 0; idx < huffman_table.size(); idx++ ) {
					const bits_t&		bits = huffman_table.at(idx);
					huffman_node_t*		current = m_root;

					for ( const auto& bit : bits ) {
						if ( true == bit ) {
							if ( nullptr == current->right() )
								current->right(new huffman_node_t);

							current = current->right();
						} else {
							if ( nullptr == current->left() )
								current->left(new huffman_node_t);

							current = current->left();
						}
					}

					current->code(static_cast< int16_t >( idx ));
				}
			}

			virtual ~huffman_tree_t(void) 
			{ 
				delete_node(m_root); 
				return; 
			}

			std::string
			decode(const std::string& src)
			{
				std::string			dst("");
				huffman_node_t*		current(m_root);

				for ( int64_t idx = 0; idx < src.length(); idx++ ) {
					for ( int8_t j = 7; j >= 0; j-- ) {
						if ( ( src[ idx ] & ( 1 << j ) ) > 0 ) {
							if ( nullptr == current->right() )
								throw std::runtime_error("");
							current = current->right();
						} else {
							if ( nullptr == current->left() )
								throw std::runtime_error("");

							current = current->left();
						}

						if ( current->code() >= 0 ) {

							if ( 257 == current->code() )
								throw std::invalid_argument("");

							dst += (uint8_t)current->code();
							current = m_root;
						}
					}
				}

				return dst;
			}
	};

	class ringtable_t
	{
		private:
		protected:
			uint64_t				m_max;
			std::deque< header_t >	m_queue;

		public:
			// 4096 is the default table size per the HTTPv2 RFC
			ringtable_t(void) : m_max(4096) { }
			ringtable_t(uint64_t m) : m_max(m) { }
			virtual ~ringtable_t(void) { }

			void
			max(uint64_t m)
			{
				m_max = m;

				// the RFC dictates that we do this here,
				// so we do.
				while ( length() >= m_max ) {
					m_queue.pop_back();
				}

				return;
			}

			inline uint64_t
			max(void) const
			{
				return m_max;
			}

			inline uint64_t
			entries_count(void) const
			{
				return m_queue.size();
			}

			inline uint64_t
			length(void) const
			{
				uint64_t size(0);
	
				for ( auto& h : m_queue ) {
					uint64_t nl(h.first.length());
					uint64_t vl(h.second.length());
					uint64_t tl(0);

					// In practice it should basically never occur
					// that either of these exceptions are thrown and
					// its probably safe to remove the checks in most instances
					if ( vl > std::numeric_limits< uint64_t >::max() ||
						nl > std::numeric_limits< uint64_t >::max() - vl )
						throw std::runtime_error("ringtable_t::length() Additive integer overflow encountered");

					tl = nl + vl;

					if ( tl > std::numeric_limits< uint64_t >::max() - size )
						throw std::runtime_error("ringtable_t::length() Additive integer overflow encountered");

					size += tl;

				}

				return size;
			}

			void
			add(const header_t&  h)
			{
				uint64_t size(h.first.length() + h.second.length());

				// In practice it should be basically implausible to trip these exceptions because
				// you would need 2^(sizeof(uint64_t)*8) bytes of memory to be in use, which in itself
				// will likely fail long before then. In other words its probably safe to remove
				// these checks for the forseeable future, but I left them in because technically I
				// should check even if its an absurd condition.
				if ( h.first.length() > std::numeric_limits< uint64_t >::max() - h.second.length() )
					throw std::runtime_error("ringtable_t::add(): Additive integer overflow encountered.");

				// Again the RFC dictates when we resize the queue.
				while ( length() >= m_max ) {
					m_queue.pop_back();
				}

				m_queue.push_front(h);
				return;
			}

			void
			add(const std::string& n, const std::string& v)
			{
				header_t h(n, v);

				add(h);
				return;
			}

			void
			add(const char* n, const char* v)
			{
				std::string name(n), value(v);

				if ( nullptr == n || nullptr == v )
					throw std::runtime_error("");

				add(name, value);
				return;
			}

			const header_t&
			at(uint64_t idx)
			{
				if ( idx > m_queue.size() ) {
					// It's not clear these checks even entirely make sense
					// the concern was that someone passes in an out-of-bounds
					// index, but thats because the static and dynamic
					// tables have their indices flattened into one, so we 
					// attempt to address that by subtracting the static 
					// headers index size if we are out of bounds, which
					// might yield a totally bogus index due to implementation
					// bug asking for an invalid index that just happens to line
					// up.
					if ( idx < predefined_headers.size() )
						throw std::invalid_argument("ringtable_t::at(): Invalid/out-of-bounds index specified");

					idx -= predefined_headers.size();

					if ( idx > m_queue.size() )
						throw std::invalid_argument("ringtable_t::at(): Invalid/out-of-bounds index specified");

				}

				return m_queue.at(static_cast< std::size_t >( idx ));
			}

			bool
			find(const header_t& h, int64_t& index) const
			{
				index = -1;

				for ( std::size_t idx = 0; idx < m_queue.size(); idx++ ) {
					if ( !h.first.compare(m_queue.at(idx).first) && !h.second.compare(m_queue.at(idx).second) ) {
						index = predefined_headers.size() + idx;
						return true;
					} else if ( !h.first.compare(m_queue.at(idx).first) ) {
						index = predefined_headers.size() + idx;
						return false;
					}
				}

				return false;
			}

			const header_t& 
			get_header(const int64_t index) const
			{
				if ( index < predefined_headers.size() ) {
					return predefined_headers.at(index);
				} else if ( index > predefined_headers.size() && index < predefined_headers.size() + m_queue.size() )
					return m_queue.at(index - predefined_headers.size());

				throw std::runtime_error("");
			}

	};
	
	class huffman_encoder_t
	{
		private:
			uint8_t m_byte;
			uint8_t m_count;

		protected:
			inline bool 
			write_bit(uint8_t bit)
			{
				m_byte |= bit;
				m_count--;

				if (0 == m_count ) {
					m_count = 8;
					return true;
				} else
					m_byte <<= 1;

				return false;
			}

		public:
			huffman_encoder_t(void) : m_byte(0), m_count(8) { }
			virtual ~huffman_encoder_t(void) { }

			std::vector< uint8_t >
			encode(std::vector< uint8_t >& src)
			{
				std::vector< uint8_t > ret(0);

				for ( auto& byte : src ) {
					bits_t bits = huffman_table.at(byte);

					for ( auto& bit : bits ) {
						if ( true == write_bit(bit) ) {
							ret.push_back(m_byte);
							m_byte = 0;
							m_count = 8;
						}

					}
				}

				// Apparently the remainder unused bits 
				// are to be set to 1, some sources refer
				// to this as the EOS bit, but the code
				// for EOS is like 30-bits of 1's so its
				// clearly not the EOS code.
				if ( 8 != m_count && 0 != m_count) {
					m_byte = ( m_byte << ( m_count - 1 ) );
					m_byte |= ( 0xFF >> ( 8 - m_count ) );
					ret.push_back(m_byte);
					m_byte = 0;
					m_count = 8;
				}

				return ret;
			}

			std::vector< uint8_t >
			encode(const std::string& src) 
			{
				std::vector< uint8_t > s(src.begin(), src.end());
				return encode(s);
			}

			std::vector< uint8_t >
			encode(const char* ptr)
			{
				std::string str(ptr);

				if ( nullptr == ptr )
					throw std::invalid_argument("");

				return encode(str);
			}
	};

	/*
	\x82\x86\x84
	\x41\x8b\x0b\xe2\x5c\x2e\x3c\xb8\x5e\x69\x70\x89\xef
	\x58\x86\xa8\xeb\x10\x64\x9c\xbf
	\x40\x88\x25\xa8\x49\xe9\x5b\xa9\x7d\x7f \x89\x25\xa8\x49\xe9\x5b\xb8\xe8\xb4\xbf
	*/

	class decoder_t
	{
		private:
		protected:
		std::map< std::string, std::string > m_headers;
			//std::vector< header_t >	m_headers;
			ringtable_t				m_dynamic;
			huffman_tree_t			m_huffman;

			typedef std::vector< uint8_t >::iterator dec_vec_itr_t;
			void
			decode_integer(dec_vec_itr_t& beg, dec_vec_itr_t& end, uint32_t& dst, uint8_t N)
			{
				const uint16_t two_N = static_cast< uint16_t >( std::pow(2, N) - 1 );
				dec_vec_itr_t&  current(beg);

				if ( current == end )
					throw std::invalid_argument("");

				dst = ( *current & two_N );

				if ( dst == two_N ) {
					uint64_t M = 0;

					for ( ; current < end; current++ ) {
						dst += ( ( *current & 0x7F ) << M );
						M += 7;

						if ( !( *current & 0x80 ) )
							break;
					}
				}

				beg = current+1;
				return;
			}

			uint64_t
			decode_integer(std::vector< uint8_t >& src, uint64_t& index, uint32_t& dst, uint8_t N)
			{
				const uint16_t two_N = static_cast< uint16_t >( std::pow(2, N) - 1 );
				uint64_t idx = index;

				if ( 0 == src.size() || idx >= src.size() )
					throw std::invalid_argument("");

				dst = ( src.at(idx++) & two_N );

				if ( dst == two_N ) {
					uint64_t M = 0;

					for ( ; idx < src.size(); idx++ ) {
						dst += ( ( src.at(idx) & 0x7F ) << M );
						M += 7;

						if ( !src.at(idx) & 0x80 )
							break;
					}
				}

				index = idx;
				return idx;
			}

			std::string
			parse_string(dec_vec_itr_t& itr, dec_vec_itr_t& end)
			{
				std::string		dst("");
				uint64_t		len(*itr & 0x7F);
				bool			huff(( *itr & 0x80 ) == 0x80 ? true : false);
				dec_vec_itr_t	cur(itr);

				if ( itr == end )
					throw std::invalid_argument("");

				for ( ++cur; cur != end; cur++ ) {
					dst += *cur;

					if ( cur - itr == len )
						break;
				}

				itr += len + 1;

				if ( true == huff )
					dst = m_huffman.decode(dst);

				return dst;
			}

			int64_t
			parse_string(std::string &dst, std::vector< uint8_t >& buf, uint64_t& idx) // const uint8_t* buf)
			{
				uint32_t str_len = 0;
				int64_t len = decode_integer(buf, idx, str_len, 7); // ( str_len, buf, 7 );
				if ( ( buf.at(idx) & 0x80 ) > 0 ) {
					dst = m_huffman.decode(std::string(buf.at(idx), str_len));// ( ( buf + len, str_len );
				} else {
					for ( int i = 0; i < str_len; i++ ) {
						dst += (char)( buf.at(idx + i)); // *( buf + ( len + i ) ) );
					}
				}
				return len + str_len;
			}

		public:
			decoder_t(int64_t max = 4096) : m_dynamic(max) { }
			virtual ~decoder_t(void) { }
			bool 
			decode(std::vector< uint8_t >& data)
			{
				uint32_t nlen(0);
				bool	 indexed(false);


				if ( !data.size() ) {
					throw std::invalid_argument("");
				}

				for ( auto& itr = data.begin(); itr != data.end(); /* itr++ */ ) {
					if ( 0x20 == ( *itr * 0xE0 ) ) { // 6.3 Dynamic Table update
						uint32_t size(0);

						decode_integer(itr, data.end(), size, 5);

						if ( size > m_dynamic.max() ) {
							// decoding error
							throw std::runtime_error("");
						}

						m_dynamic.max(size);
					} else if ( ( *itr & 0x80 ) ) { // 6.1 Indexed Header Field Representation
						uint32_t index(0);

						decode_integer(itr, data.end(), index, 7);

						if ( 0 == index ) {
							// decoding error
							throw std::runtime_error("");
						}

						m_headers[ m_dynamic.get_header(index).first ] = m_dynamic.get_header(index).second;
					} else {
						uint32_t index(0);
						std::string n(""); // , v("");

						if ( 0x40 == ( *itr & 0xC0 ) )
							decode_integer(itr, data.end(), index, 6);
						else
							decode_integer(itr, data.end(), index, 4);

						if ( 0 != index ) {
							header_t h = m_dynamic.get_header(index);
							n = h.first;
						} else {
							n = parse_string(itr, data.end());
						}

						//v = parse_string(itr, data.end());
						m_headers[ n ] = parse_string(itr, data.end());
					}
				}
			}
	};



#define INDEXED_BIT_PATTERN 0x80
#define LITERAL_INDEXED_BIT_PATTERN 0x40
#define LITERAL_WITHOUT_INDEXING_BIT_PATTERN 0x00
#define LITERAL_NEVER_INDEXED_BIT_PATTERN 0x10
#define HUFFMAN_ENCODED 0x80

	/*! \Class The HPACK encoder class. 
     *  \Brief A wrapper class that ties together the ringtable_t dynamic table implementation
	 * with the prior static table such that one can simply add(name, value) into an
	 * internally managed buffer (a std::vector< uint8_t >) which can be retrieved at
	 * the end of operations. Huffman encoding and dynamic table references are handled 
	 * automatically; 
	 * 
	 * \Warning Never Indexed code paths untested.
	 */
	class encoder_t
	{
		private:
		protected:
			std::vector< uint8_t >	m_buf;
			ringtable_t				m_dynamic;
			huffman_encoder_t		m_huffman;

			void
			huff_encode(const std::string& str)
			{
				std::vector< uint8_t >	huffbuff(0);
				std::size_t				len(0);


				huffbuff = m_huffman.encode(str);	

				if ( 128 > huffbuff.size() )
					m_buf.push_back(static_cast< uint8_t >( HUFFMAN_ENCODED | huffbuff.size() ));
				else {
					std::vector< uint8_t > tmp;
					encode_integer(tmp, huffbuff.size(), 7);
					m_buf.insert(m_buf.end(), tmp.begin(), tmp.end());
				}
				
				m_buf.insert(m_buf.end(), huffbuff.begin(), huffbuff.end());				
				return;
			}

			bool
			find(const header_t& h, int64_t& index)
			{
				int64_t saved_index(-1);
				index = -1;

				for ( uint64_t idx = 1; idx < predefined_headers.size(); idx++ ) {
					if ( !h.first.compare(predefined_headers.at(static_cast<std::size_t>( idx )).first) &&
						!h.second.compare(predefined_headers.at(static_cast<std::size_t>( idx )).second) ) {
						index = idx;
						return true;
					} else if ( !h.first.compare(predefined_headers.at(static_cast<std::size_t>( idx )).first) ) {
						index = idx;
					}
				}

				saved_index = index;

				if ( true == m_dynamic.find(h, index) )
					return true;
				else if ( -1 != index )
					return false;

				if ( -1 != saved_index )
					index = saved_index;

				return false;
			}

			uint64_t
			encode_integer(std::vector< uint8_t >& dst, uint32_t I, uint8_t N)
			{
				const uint16_t two_N = static_cast< uint16_t >( std::pow(2, N) - 1 );

				if ( I < two_N ) {
					dst.push_back(static_cast< uint8_t >( I ));
					return 1;
				} else {
					I -= two_N;
					dst.push_back(static_cast< uint8_t >( two_N ));

					while ( I >= 128 ) {
						dst.push_back(static_cast< uint8_t >( ( I & 0x7F ) | 0x80 ));
						I >>= 7;
					}

					dst.push_back(static_cast< uint8_t >( I ));
					return dst.size();
				}

				// unreached
				throw std::runtime_error("Impossible code path in encode_integer()");
			}

		public:
			/*!
			\fn encoder_t(uint64_t max = 4096)
			\Brief Constructs the encoder

			\param max the maximum size of the dynamic table; unbounded and allowed to exceed RFC sizes
			*/
			encoder_t(uint64_t max = 4096) : m_dynamic(max) { }
			virtual ~encoder_t(void) { }

			/*!
			\fn void max_table_size(uint64_t max)
			\Brief Resizes the dynamic table

			\param max the maximum size of the dynamic table; unbounded and allowed to exceed RFC sizes
			*/
			void 
			max_table_size(uint64_t max) 
			{ 
				m_dynamic.max(max); 
				return; 
			}

			/*!
			\fn inline uint64_t max_table_size(void) const
			\Brief Retrieve the size of dynamic table

			\return max the maximum size of the dynamic table; unbounded and allowed to exceed RFC sizes
			*/
			inline uint64_t 
			max_table_size(void) const 
			{ 
				return m_dynamic.max(); 
			}

			/*!
			\fn void add(const std::string& n, const std::string& v, bool huffman = true, bool never_indexed = false)
			\Brief Add a header name-value pair to the header list
			
			\param n the name-value pair name member
			\param v the name-value pair value member
			\param huffman a boolean value that indicates whether to huffman encode any related string literals
			\param never_indexed whether to set the never indexed flag for the name-value pair
			*/
			void
			add(const std::string& n, const std::string& v, bool huffman = true, bool never_indexed = false)
			{
				header_t h(n, v);
				add(h, huffman, never_indexed);
			}

			/*!
			\fn void add(const char* n, const char* v, bool huffman = true, bool never_indexed = false)
			\Brief Add a header name-value pair to the header list
			\Throws std::invalid_argument() when n or v is null

			\param n the name-value pair name member
			\param v the name-value pair value member
			\param huffman a boolean value that indicates whether to huffman encode any related string literals
			\param never_indexed whether to set the never indexed flag for the name-value pair
			*/
			void
			add(const char* n, const char* v, bool huffman = true, bool never_indexed = false)
			{
				header_t h(n, v);

				if ( nullptr == n || nullptr == v )
					throw std::invalid_argument("HPACK::encoder_t::add(): Invalid nullptr parameter.");

				add(h, huffman, never_indexed);
			}

			/*!
			\fn void add(const header_t& h, bool huffman = true, bool never_indexed = false)
			\Brief Add a header name-value pair to the header block

			\param h the name-value header_t pair to be added to the header block
			\param huffman a boolean value that indicates whether to huffman encode any related string literals
			\param never_indexed whether to set the never indexed flag for the name-value pair
			*/
			void
			add(const header_t& h, bool huffman = true, bool never_indexed = false)
			{
				int64_t					index(0);
				std::vector< uint8_t >	buf(0), huffbuff(0);

				if ( false == never_indexed && true == find(h, index) ) {
					encode_integer(buf, index, 7);

					buf.front() |= INDEXED_BIT_PATTERN;
					m_buf.insert(m_buf.end(), buf.begin(), buf.end());
					buf.clear();
				} else if ( false == never_indexed && -1 != index ) {
					m_dynamic.add(h.first, h.second);

					if ( false == never_indexed ) {
						encode_integer(buf, index, 6);
						buf.front() |= LITERAL_INDEXED_BIT_PATTERN;
						m_buf.insert(m_buf.end(), buf.begin(), buf.end());
						buf.clear();
					} else {
						encode_integer(buf, index, 4);
						buf.front() |= LITERAL_NEVER_INDEXED_BIT_PATTERN;
						m_buf.insert(m_buf.end(), buf.begin(), buf.end());
						buf.clear();
					}

					if ( true == huffman )
						huff_encode(h.second);
					else {
						m_buf.push_back(static_cast<uint8_t>( h.second.length() ));
						m_buf.insert(m_buf.end(), h.second.begin(), h.second.end());
					}
				} else {
					if ( false == never_indexed ) {
						m_dynamic.add(h.first, h.second);
						m_buf.push_back(LITERAL_INDEXED_BIT_PATTERN);
					} else
						m_buf.push_back(LITERAL_NEVER_INDEXED_BIT_PATTERN);

					if ( true == huffman && false == never_indexed )
						huff_encode(h.first);
					else {
						m_buf.push_back(static_cast<uint8_t>( h.first.length() ));
						m_buf.insert(m_buf.end(), h.first.begin(), h.first.end());
					}
					if ( true == huffman && false == never_indexed )
						huff_encode(h.second);
					else {
						m_buf.push_back(static_cast< uint8_t >( h.second.length() ));
						m_buf.insert(m_buf.end(), h.second.begin(), h.second.end());
					}	
				}

				return;
			}



			std::vector< uint8_t >& data(void) { return m_buf; }
		};
}
