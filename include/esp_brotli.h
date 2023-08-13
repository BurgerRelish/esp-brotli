#pragma once

#ifndef ESP_BROTLI_H
#define ESP_BROTLI_H

#include <ps_string.h>

#define BROTLI_DECODER_BUFFER_SIZE 16384

namespace brotli {

ps::string decompress(ps::string& message);
ps::string compress(ps::string& message);

}

#endif