# hpack-encoder
header-only c++11/STL implementation of a HTTPv2 HPACK encoder. 

```
HPACK::encoder_t encoder;
std::vector< uint8_t > d;

// encoder(Name, Value, Use_Huffman_Encoding = true, Use_Never_Index = false)
encoder.add(":method", "GET", false);
encoder.add(":scheme", "http", false);
// typedef std::pair< std::string, std::string > header_t
encoder.add(HPACK::header_t(":path", "/"), false, true);
encoder.add(HPACK::header_t(":authority", "192.168.184.128"));
encoder.add(HPACK::header_t("cache-control", "no-cache"));
encoder.add(HPACK::header_t("custom-key", "custom-value"));
d = encoder.data();
```
