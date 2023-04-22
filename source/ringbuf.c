#include <string.h> // memset
#include <stdio.h>
#include <stdint.h> // uintptr_t

#include "ringbuf.h"



void ringbuf_clear(RingBufC* ring) {
    memset(ring->data, 0, ring->item_size * ring->data_size);
    ring->head = ring->data;
    ring->tail = ring->data;  
}


bool ringbuf_is_empty(RingBufC* ring) {
    return ring->head == ring->tail;
}


bool ringbuf_is_full(RingBufC* ring) {
    return ((char*)ring->head == ((char*)ring->tail)-ring->item_size) || 
       (ring->tail == ring->data && ring->head == ring->end);
}


int ringbuf_push(RingBufC* ring, const void* to_push) {
    if (ringbuf_is_full(ring)) {
        return -1;
    }
    memcpy(ring->head, to_push, ring->item_size);
    ring->head = (void*)((char*)ring->head + ring->item_size);
    if (ring->head > ring->end) { // check for wrap
        ring->head = ring->data;
    }
    return 0;
}


int ringbuf_pop(RingBufC* ring, void* popped) {
    if (ringbuf_is_empty(ring)) {
        return -1;
    }
    memcpy(popped, ring->tail, ring->item_size);
    ring->tail = (void*)((char*)ring->tail + ring->item_size);
    if (ring->tail > ring->end) { // check for wrap
        ring->tail = ring->data;
    }
    return 0;
}


size_t ringbuf_capacity(RingBufC* ring)
{
    return ring->data_size;
}


size_t ringbuf_space_used(RingBufC* ring)
{
    if (ring->head > ring->tail) {
        return ((char*)(ring->head) - (char*)(ring->tail)) / ring->item_size;
    }
    return ((uintptr_t)(ring->end) - ((char*)(ring->tail)-(char*)(ring->head)-1)) / ring->item_size;
}


size_t ringbuf_space_remaining(RingBufC* ring)
{
    return ringbuf_capacity(ring) - ringbuf_space_used(ring);
}
