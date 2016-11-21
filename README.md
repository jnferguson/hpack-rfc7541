# hpack
header-only c++/STL-ish implementation of a HTTPv2 HPACK/RFC7541 encoder/decoder. 

# Encoder 

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
	encoder.add(HPACK::header_t(":authority", "www.example.com"));
	encoder.add(HPACK::header_t("custom-key", "custom-value"), false);
d = encoder.data();
```

# Decoder

```
	std::string x("\x82\x86\x84\x41\x8b\x0b\xe2\x5c\x2e\x3c\xb8\x5e\x69\x70\x89\xef\x58\x86\xa8\xeb\x10\x64\x9c\xbf\x40\x88\x25\xa8\x49\xe9\x5b\xa9\x7d\x7f\x89\x25\xa8\x49\xe9\x5b\xb8\xe8\xb4\xbf");
	HPACK::decoder_t dec;

	if (true == dec.decode(x)) {
		for (auto& hdr : dec.headers())
			std::cout << hdr.first << ": " << hdr.second << std::endl;
	} else 
		// protocol error
```
