#include <string.h> // memset


template <class T, unsigned int N>
class RingBufTemplate {
public:
    RingBufTemplate() : m_end(m_data+N-1) {
        Clear();
    }

    void Clear() {
        memset(m_data, 0, N);
        m_head = m_data;
        m_tail = m_data; 
    }

    bool IsEmpty() {
        return m_head == m_tail;
    }

    bool IsFull() {
        return (m_head == m_tail-1) || 
            (m_tail == m_data && m_head == m_end);
    }

    int Push(T to_push) {
        if (IsFull()) {
            return -1;
        }
        *(m_head++) = to_push;
        if (m_head > m_end) { // check for wrap
            m_head = m_data;
        }
        return 0;
    }

    int Pop(T& popped) {
        if (IsEmpty()) {
            return -1;
        }
        popped = *(m_tail++);
        if (m_tail > m_end) { // check for wrap
            m_tail = m_data;
        }
        return 0;
    }

    unsigned int Capacity() {return N;}

    unsigned int SpaceUsed() {
        if (m_head > m_tail) {
            return m_head - m_tail;
        }
        return m_end - (m_tail-m_head-1);
    }

    unsigned int SpaceRemaining() {
        return Capacity() - SpaceUsed(); 
    }

private:
    T m_data[N];
    T* m_head;
    T* m_tail;
    const T* m_end;




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