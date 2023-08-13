#include "esp_brotli.h"

#include "esp_log.h"
#include "esp_timer.h"

#include "ps_base64/ps_base64.h"
#include "brotli/include/brotli/encode.h"
#include "brotli/include/brotli/decode.h"

namespace brotli {
/**
 * @brief Decompresses a string using the Brotli Algrotithm.
 * Dynamically allocates a buffer of BROTLI_DECODER_BUFFER_SIZE, uses the Google Brotli Decoder to decode the message
 * into the output buffer. Converts the output buffer to a ps::string and shrinks it to fit.
 * 
 * @note Returns an empty string on decompression failure
 * .
 * @param message Brotli Compressed string to decompress.
 * @return std::string - Decompressed Data
*/
ps::string decompress(ps::string& message)
{
    size_t decoded_size = BROTLI_DECODER_BUFFER_SIZE;

    #ifdef DEBUG_COMPRESSION
    ESP_LOGD("DECOMPR", "Decoding base64...");
    uint64_t start_time = esp_timer_get_time();
    uint64_t tot_time = 0;
    #endif

    ps::string inbuf = base64_decode(message);

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("DECOMPR", "base64 Decoded. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("DECOMPR", "Allocating Memory...");
    start_time = esp_timer_get_time();
    #endif

    auto outbuf = (uint8_t*) heap_caps_calloc(1, sizeof(uint8_t) * decoded_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("DECOMPR", "Memory Allocated. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("DECOMPR", "Decompressing...");
    start_time = esp_timer_get_time();
    #endif

    BrotliDecoderResult status = BrotliDecoderDecompress(inbuf.size(), (const uint8_t *)inbuf.c_str(), &decoded_size, outbuf); // Decompress message with Brotli Decompression

    if (status != BROTLI_DECODER_RESULT_SUCCESS) // Check that decoder was successful.
    {
        ESP_LOGE("DECOMPR", "Decompression Failed.");

        heap_caps_free(outbuf);

        return ps::string("");
    }

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("DECOMPR", "Decompression Completed. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("DECOMPR", "Converting to string and cleaning up...");
    start_time = esp_timer_get_time();
    #endif

    ps::string ret((char *)outbuf ); // Create a string containing the decompressed data.

    ret.shrink_to_fit();

    heap_caps_free(outbuf);

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("DECOMPR", "Done. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("DECOMPR", "\n==== Decompression Details ====\n- Decompression Time: %uus\n- Decompression Ratio: %f\n- Original Message: \'%s\'\n- Decompression Result: \'%s\'\n===============================", tot_time, (float)message.size() / (float)ret.size(), message.c_str(), ret.c_str());
    #endif

    return ret;
}

/**
 * @brief Compresses a string using the Brotli Algorithm.
 * Dynamically allocates a temporary buffer to store the compressed string. Uses the 
 * Google Brotli Encoder to compress the inout string. Copys the output of the encoder to a 
 * new string and shrinks it to fit.
 * 
 * @note Returns an empty string on compression failure.
 * 
 * @param message std::string to be compressed.
 * @return std::string - Compressed string data.
*/
ps::string compress(ps::string& message)
{
    #ifdef DEBUG_COMPRESSION
    ESP_LOGD("COMPR", "Allocating Memory...");
    uint64_t start_time = esp_timer_get_time();
    uint64_t tot_time = 0;
    #endif

    size_t data_size = message.size();
    size_t outbytes = data_size + 150;
    auto outbuf = (uint8_t*) heap_caps_calloc(1, sizeof(uint8_t) * outbytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("COMPR", "Memory Allocated. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("COMPR", "Starting Compression...");
    start_time = esp_timer_get_time();
    #endif

    int status = BrotliEncoderCompress(BROTLI_DEFAULT_QUALITY, BROTLI_DEFAULT_WINDOW, BROTLI_MODE_TEXT, data_size, (const uint8_t *)message.c_str(), &outbytes, outbuf); // Compress message with Brotli Compression

    if (status != BROTLI_TRUE) // Check that encoder was successful.
    {
        ESP_LOGE("COMPR", "Compression Failed.");
        heap_caps_free(outbuf);
        return ps::string("");
    }

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("COMPR", "Compression completed. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("COMPR", "Encoding in base64...");
    start_time = esp_timer_get_time();
    #endif

    ps::string ret = base64_encode((unsigned char *)outbuf, outbytes, false); // Create a string containing the compressed data.
    heap_caps_free(outbuf);

    #ifdef DEBUG_COMPRESSION
    tot_time += esp_timer_get_time() - start_time;
    ESP_LOGD("COMPR", "Encoding completed. [Took %uus]", esp_timer_get_time() - start_time);
    ESP_LOGD("COMPR", "\n==== Compression Details ====\n- Compression Time: %uus\n- Compression Ratio: %f\n- Original Message: \'%s\'\n- Compression Result: \'%s\'\n=============================", tot_time, (float)message.size() / (float)ret.size(), message.c_str(), ret.c_str());
    #endif

    return ret;
}

}
