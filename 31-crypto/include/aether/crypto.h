#ifndef AETHER_CRYPTO_H
#define AETHER_CRYPTO_H

#include <stddef.h>
#include <aether/types.h>

aether_status_t aether_crypto_random(void *buffer,size_t size);
int aether_crypto_equal(const void *a,const void *b,size_t size);
void aether_crypto_wipe(void *buffer,size_t size);
#endif
