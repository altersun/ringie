#include <stdbool.h>
#include <assert.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RingBufC
{
    // Data and tracking
    void* data;
    void* head;
    void* tail;
    void* end;

    // Other bookkeeping
    size_t item_size;
    size_t data_size;
} RingBufC;


void ringbuf_clear(RingBufC* ring);

bool ringbuf_is_empty(RingBufC* ring);

bool ringbuf_is_full(RingBufC* ring);

int ringbuf_push(RingBufC* ring, const void* to_push);

int ringbuf_pop(RingBufC* ring, void* popped);

size_t ringbuf_capacity(RingBufC* ring);

size_t ringbuf_space_used(RingBufC* ring);

size_t ringbuf_space_remaining(RingBufC* ring);

#ifdef __cplusplus
}
#endif

// C Ring buffer "Constructor" macro
// Uses the C concatenate macro (##) to create a named array...
// ...which serves as the data store for the buffer. Static allocation!
// Modified from https://embedjournal.com/implementing-circular-buffer-embedded-c/
#define RINGBUF_INIT(name,type,capacity)                \
    uint8_t name##_data_space[capacity*sizeof(type)];         \
    RingBufC name = {                      \
        .data = name##_data_space,      \
        .head = name##_data_space,        \
        .tail = name##_data_space,        \
        .end = name##_data_space + (capacity*sizeof(type)), \
        .item_size = sizeof(type), \
        .data_size = capacity, \
    }; \
    ringbuf_clear(&name)

// Macro for pushing without casting or referencing
#define RINGBUF_PUSH(ring, to_push) \
    ringbuf_push(ring, (void*)(&to_push))

// Macro for popping without casting or referencing
#define RINGBUF_POP(ring, popped) \
    ringbuf_pop(ring, (void*)(&popped))
