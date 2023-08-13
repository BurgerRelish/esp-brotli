/* Copyright 2016 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

/**
 * @brief Modifed by Nicholas Watt (github.com/BurgerRelish)
 */

#include <stdlib.h>

#include <brotli/types.h>

#include "platform.h"

#include "esp_heap_caps.h"

/* Default brotli_alloc_func */
void* BrotliDefaultAllocFunc(void* opaque, size_t size) {
  BROTLI_UNUSED(opaque);
#ifdef BOARD_HAS_PSRAM
  return heap_caps_malloc(size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
#else
  return malloc(size);
#endif
}

/* Default brotli_free_func */
void BrotliDefaultFreeFunc(void* opaque, void* address) {
  BROTLI_UNUSED(opaque);
#ifdef BOARD_HAS_PSRAM
  return heap_caps_free(address);
#else
  free(address);
#endif
}
