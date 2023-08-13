Forked from google/brotli and adapted to run on the ESP32.

## Usage
To use, simply call the below functions:
```cpp 
ps::string brotli::compress(ps::string& msg);
ps::string brotli::decompress(ps::string& msg);
```cpp

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
```cpp