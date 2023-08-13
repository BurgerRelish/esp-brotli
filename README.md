<p align="center">
  <img src="https://github.com/google/brotli/actions/workflows/build_test.yml/badge.svg" alt="GitHub Actions Build Status" href="https://github.com/google/brotli/actions?query=branch%3Amaster">
  <img src="https://oss-fuzz-build-logs.storage.googleapis.com/badges/brotli.svg" alt="Fuzzing Status" href="https://oss-fuzz-build-logs.storage.googleapis.com/index.html#brotli">
</p>
<p align="center"><img src="https://brotli.org/brotli.svg" alt="Brotli" width="64"></p>

### Introduction

Brotli is a generic-purpose lossless compression algorithm that compresses data
using a combination of a modern variant of the LZ77 algorithm, Huffman coding
and 2nd order context modeling, with a compression ratio comparable to the best
currently available general-purpose compression methods. It is similar in speed
with deflate but offers more dense compression.

The specification of the Brotli Compressed Data Format is defined in [RFC 7932](https://tools.ietf.org/html/rfc7932).

Brotli is open-sourced under the MIT License, see the LICENSE file.

> **Please note:** brotli is a "stream" format; it does not contain
> meta-information, like checksums or uncompresssed data length. It is possible
> to modify "raw" ranges of the compressed stream and the decoder will not
> notice that.

## Note
Forked from google/brotli and adapted to run on the ESP32. Uses PSRAM, buffer size can be changed in esp_brotli.h.

## PIO Dependencies
- burgerrelish/PSRAM Containers@^1.0.0 (github.com/BurgerRelish/ESP32-PSRAM-STL-Containers)

## Usage
To use, simply call the below functions:
```cpp 
ps::string brotli::compress(ps::string& msg);
ps::string brotli::decompress(ps::string& msg);
```

Unit test example:
```cpp
#include <Arduino.h>
#include <unity.h>

#include <esp_brotli.h>
#include <ps_stl.h>

void test_compression() {
    ps::string msg = "Hello World!!!";
    auto result = brotli::compress(msg);

    TEST_ASSERT_EQUAL_STRING("iwaASGVsbG8gV29ybGQhISED", result.c_str());
}

void test_decompression() {
    ps::string msg = "iwaASGVsbG8gV29ybGQhISED";
    auto result = brotli::decompress(msg);

    TEST_ASSERT_EQUAL_STRING("Hello World!!!", result.c_str());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_compression);
    RUN_TEST(test_decompression);
    UNITY_END();
}

void loop() {

}
```
