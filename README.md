# hpack-encoder
header-only c++/STL implementation of a HTTPv2 RFC7541/HPACK encoder. 

```
HPACK::encoder_t encoder;
std::vector< uint8_t > d;

// encoder(Name, Value, Use_Huffman_Encoding = true, Use_Never_Index = false)
// typedef std::pair< std::string, std::string > header_t
	encoder.add(HPACK::header_t(":method", "GET"));
	encoder.add(HPACK::header_t(":scheme", "http"));
	encoder.add(HPACK::header_t(":path", "/"));
	encoder.add(HPACK::header_t(":authority", "www.example.com"));
	encoder.data().clear();
	encoder.add(HPACK::header_t(":method", "GET"));
	encoder.add(HPACK::header_t(":scheme", "http"));
	encoder.add(HPACK::header_t(":path", "/"));
	encoder.add(HPACK::header_t(":authority", "www.example.com"), false);
	encoder.add(HPACK::header_t("cache-control", "no-cache"), false);
	encoder.data().clear();
	encoder.add(HPACK::header_t(":method", "GET"));
	encoder.add(HPACK::header_t(":scheme", "https"));
	encoder.add(HPACK::header_t(":path", "/index.html"));
	encoder.add(HPACK::header_t(":authority", "www.example.com"), false);
	encoder.add(HPACK::header_t("custom-key", "custom-value"), false);
d = encoder.data();
```
