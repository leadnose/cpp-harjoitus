#include <algorithm>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <cstdio>
#include "string.hh"

namespace jpr {

    InvalidStringException::InvalidStringException(const char *msg)
        : m_msg(msg) // use only statically allocated strings in exceptions to keep things simple
    {
    }
        
    const char *InvalidStringException::what() const throw()
    {
        return m_msg;
    }

    void String::check() throw (InvalidStringException)
    {
        if (m_bufsize > 0) {
        
            if (0 == m_buf) {
                throw InvalidStringException("m_bufsize > 0 but m_buf == nullptr");
            }
        
            if (m_used > m_bufsize) {
                throw InvalidStringException("m_used > m_bufsize");
            }
        
        } else {
            if (0 != m_buf && m_bufsize == 0) {
                throw InvalidStringException("m_buf != nullptr but m_bufsize == 0");
            }
        }
    }

    void String::dump_state(std::ostream &os)
    {
        os << "this = " << (void*) this << ", m_buf = " << (void*)m_buf << ", m_bufsize = " << m_bufsize << ", m_used = " << m_used << std::endl;
    }
    
    // default constructor
    String::String()
        : m_bufsize(0), m_used(0), m_buf(0)
    {
        check();
    }

    // copy constructor
    String::String(const String &other)
        : m_bufsize(other.m_used),
          m_used(other.m_used)
    {
        m_buf = new char[m_bufsize];

        for (size_t i = 0; i < m_bufsize; ++i) {
            m_buf[i] = other.m_buf[i];
        }

        check();
    }

    String & String::operator= (const String & other)
    {
        if (this != &other) {
            char * new_buf = new char[other.m_bufsize];
            std::copy(other.m_buf, other.m_buf + other.m_bufsize, new_buf);

            delete [] m_buf;

            m_buf = new_buf;
            m_bufsize = other.m_bufsize;
            m_used = other.m_used;
        }

        return *this;
    }

    bool String::operator==(const String& other) const
    {
        if (other.m_used != m_used) {
            return false;
        } else {
            for (size_t i = 0; i < m_used; ++i) {
                if (m_buf[i] != other.m_buf[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    bool String::operator!=(const String& other) const
    {
        return !(*this == other);
    }

    // constructor from C-string, which must be null-terminated and may be freed after use
    String::String(const char *cstr)
    {
        if (0 != cstr) {
            m_used = strlen(cstr);
            m_bufsize = m_used;
            
            m_buf = new char[m_bufsize];
            
            for (size_t i = 0; i < m_bufsize; ++i) {
                m_buf[i] = cstr[i];
            }
        } else {
            m_used = 0, m_bufsize = 0;
            m_buf = 0;
        }

        check();
    }

    // destructor
    String::~String()
    {
        delete[] m_buf;
    }

    char & String::operator[](size_t index) const
    {
        if (index >= m_used) { // don't need to check for index < 0 because size_t is always >= 0
            throw std::out_of_range("index too large");
        } else {
            return m_buf[index];
        }
    }

    void String::push_back(char c)
    {

        if (m_bufsize == 0) {
            m_bufsize++;
            m_buf = new char[m_bufsize];
            m_buf[m_used] = c;
            m_used++;
            return;
        } else if (m_used == m_bufsize) {
            const int MULTIPLIER = 2;
            char *new_buf = new char[m_bufsize * MULTIPLIER];
            std::copy(m_buf, m_buf+m_bufsize, new_buf);
            delete [] m_buf;
            m_buf = new_buf;
            m_buf[m_used] = c;
            m_bufsize = m_bufsize * MULTIPLIER;
            m_used++;
            return;
        } else {
            m_buf[m_used] = c;
            m_used++;
            return;
        }
    }

    void String::pop_back()
    {
        if (m_used == 0) {
            throw std::logic_error("tried to pop_back() on an empty string");
        } else {
            m_used--;
            // TODO: shrink the buffer
        }
    }

    void String::swap(String & other)
    {
        std::swap(m_bufsize, other.m_bufsize);
        std::swap(m_used,    other.m_used);
        std::swap(m_buf,     other.m_buf);
    }
        
    std::ostream & operator<<(std::ostream & os, const String & s)
    {
        for (size_t i = 0; i < s.m_used; i++) {
            os << s.m_buf[i];
        }
        return os;
    }

    std::istream & operator>>(std::istream & is, String & s)
    {

        s = String(); // clear the string like std::string's operator>> does
        
        for (;;) {
            int ch = is.get();
            if (EOF == ch) {
                // puts("EOF1");
                return is;
            } else if (isspace(ch)) {
                // puts("SPACE1");
                continue;
            } else {
                // printf("PUSH_BACK1: %c\n", ch);                
                s.push_back(ch);
                for (;;) {
                    ch = is.get();
                    if (EOF == ch) {
                        // puts("EOF2");
                        return is;
                    } else if (isspace(ch)) {
                        // puts("SPACE2");
                        return is;
                    } else {
                        // printf("PUSH_BACK2: %c\n", ch);
                        s.push_back(ch);
                    }
                }
            }
        }
    }

    // iterator implementation
    
    String::iterator::iterator(String & string, size_t index)
        : m_string(string), m_index(index)
    {}
    
    String::iterator String::iterator::operator++() throw (std::logic_error)
    {
        if (m_index >= m_string.m_used) {
            throw std::logic_error("iterator out of bounds");
        } else {
            m_index++;
            return *this;
        }
    }

    String::iterator String::iterator::operator++(int) throw (std::logic_error)
    {
        String::iterator ans = *this;
        ++(*this);
        return ans;
    }
    
    char & String::iterator::operator*() throw (std::logic_error)
    {
        if (m_index >= m_string.m_used) {
            throw std::logic_error("iterator out of bounds");
        } else {
            return m_string[m_index];
        }
    }

    char * String::iterator::operator->() throw (std::logic_error)
    {
        if (m_index >= m_string.m_used) {
            throw std::logic_error("iterator out of bounds");
        } else {
            return &m_string[m_index];
        }
    }

    bool String::iterator::operator!=(const String::iterator& other)
    {
        // iterators are equal if the addressess of the pointed-to objects are the same
        if (&m_string == &other.m_string) {
            return m_index != other.m_index;
        } else {
            return true;
        }
    }


    String::iterator String::begin()
    {
        return String::iterator(*this, 0);
    }
    
    String::iterator String::end()
    {
        return String::iterator(*this, this->m_used);
    }



}
