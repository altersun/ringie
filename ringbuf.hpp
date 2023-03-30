#include <string.h> // memset


class RingBufCpp {
public:
    RingBufCpp(size_t item_size, size_t data_size);
    virtual ~RingBufCpp();

    void Clear();
    bool IsEmpty();
    bool IsFull();

    int Push(auto to_push) {
        if (IsFull()) {
           return -1;
        }
        memcpy(m_head, static_cast<void*>(&to_push), m_item_size);
        m_head = static_cast<void*>(static_cast<char*>(m_head) + m_item_size);
        if (m_head > m_end) { // check for wrap
           m_head = m_data;
        }
       return 0;
    }

    int Pop(auto& popped) {
        if (IsEmpty()) {
           return -1;
        }
        memcpy(static_cast<void*>(&popped), m_tail, m_item_size);
        m_tail = static_cast<void*>(static_cast<char*>(m_tail) + m_item_size);
        if (m_tail > m_end) { // check for wrap
            m_tail = m_data;
        }
        return 0;
    }
    
    size_t Capacity();
    size_t SpaceUsed();
    size_t SpaceRemaining();

private:
    void* m_data;
    void* m_head;
    void* m_tail;
    void* m_end;

    size_t m_item_size;
    size_t m_data_size;



/*

Empty:
     h
     v     
    ---------------
    | | | | | | | |
    ---------------
     ^
     t

Full:
     h
     v     
    ---------------
    | | | | | | | |
    ---------------
       ^
       t

    or

                 h
                 v     
    ---------------
    | | | | | | | |
    ---------------
     ^
     t
*/

};