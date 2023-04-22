#include <stdlib.h> // calloc
#include <stdexcept> // bad_alloc
#include <stdint.h> // uintptr_t
#include <string.h> // memset

#include "ringbuf.hpp"


RingBufCpp::RingBufCpp(size_t item_size, size_t data_size) 
{
    m_item_size = item_size;
    m_data_size = data_size;
    m_data = calloc(m_data_size, m_item_size);
    m_end = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_data) + m_data_size * m_item_size); 
    if (m_data == NULL) {
        throw std::bad_alloc();
    }
    Clear();
}


RingBufCpp::~RingBufCpp()
{
    free(m_data);
}


void RingBufCpp::Clear() 
{
    memset(m_data, 0, m_item_size*m_data_size);
    m_head = m_data;
    m_tail = m_data; 
}


bool RingBufCpp::IsEmpty() 
{
        return m_head == m_tail;
}


bool RingBufCpp::IsFull() 
{
    return static_cast<char*>(m_head) == (static_cast<char*>(m_tail)-m_item_size) || 
        (m_tail == m_data && m_head == m_end);
}

    
size_t RingBufCpp::Capacity() 
{
    return m_data_size;
}

    
size_t RingBufCpp::SpaceUsed() 
{
    if (m_head > m_tail) {
        return (static_cast<char*>(m_head) - static_cast<char*>(m_tail)) / m_item_size;  
    }
    return (reinterpret_cast<uintptr_t>(m_end) - (static_cast<char*>(m_tail)-static_cast<char*>(m_head))-1) / m_item_size;
}


size_t RingBufCpp::SpaceRemaining() 
{
    return Capacity() - SpaceUsed(); 
}

