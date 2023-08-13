/* Copyright 2016 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

#include <stdlib.h>

#include <brotli/include/brotli/types.h>
#include <esp_heap_caps.h>

#include "platform.h"

/* Default brotli_alloc_func */
void* BrotliDefaultAllocFunc(void* opaque, size_t size) {
  BROTLI_UNUSED(opaque);
  return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}

/* Default brotli_free_func */
void BrotliDefaultFreeFunc(void* opaque, void* address) {
  BROTLI_UNUSED(opaque);
  heap_caps_free(address);
}
