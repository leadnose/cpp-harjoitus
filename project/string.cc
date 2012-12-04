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

    // a helper class to check the invariants during member functions
    // it is intended to be used by constructing an instance at the beginning (or near the beginning, after parameter checks) of
    // every member function of String, and it will check that the invariants are held both before and after the call
    class StringChecker
    {
    public:
        // the constructor takes a reference to jpr::String and simply calls its .check() method
        StringChecker(const String & s, const char *filename, int line)
            : m_str(s), m_filename(filename), m_line(line)
        {
            try {
                m_str.check();
                std::cerr << "ENTRY OK" << std::endl;
            } catch (const InvalidStringException & e) {
                std::cerr << "ERROR: invariant failed on entry in file " << m_filename << ":" << m_line << std::endl;
            }
        }
        
        // the destructor checks the invariant again, but instead of letting the possible exception
        // be thrown through (you can't throw from destructor), a message is written to std::cerr, prefixed with "ERROR:"
        // and terminate() is called
        ~StringChecker() throw()
        {
            try {
                m_str.check(); // may throw
                std::cerr << "EXIT OK" << std::endl;
            } catch (const InvalidStringException & e) {
                std::cerr << "ERROR: invariant failed on exit in file " << m_filename << ":" << m_line << std::endl;
                std::terminate();
            } catch (...) { // catch all other exceptions too
                std::cerr << "ERROR: unknown exception in ~StringChecker()" << std::endl;
                std::terminate();
            }
        }

    private:
        const String & m_str;
        const char *m_filename;
        int m_line;
        
    };

// helper macro to make using StringChecker more convenient
#ifdef DEBUG        
#define CHECKED(x) StringChecker checker (x, __FILE__, __LINE__)
#endif

#ifndef DEBUG
#define CHECKED(x) ;        
#endif

    void String::check() const throw (InvalidStringException) 
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

    size_t String::size() const
    {
        CHECKED(*this);
        return m_used;
    }

    String & String::operator= (const String & other)
    {
        CHECKED(*this);
        if (this != &other && other.m_buf) {
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
        CHECKED(*this);
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
        CHECKED(*this);
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
        CHECKED(*this);
        if (index >= m_used) { // don't need to check for index < 0 because size_t is always >= 0
            throw std::out_of_range("index too large");
        } else {
            return m_buf[index];
        }
    }

    void String::push_back(char c)
    {
        CHECKED(*this);
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
        CHECKED(*this);
        if (m_used == 0) {
            throw std::logic_error("tried to pop_back() on an empty string");
        } else {
            m_used--;
            // TODO: shrink the buffer
        }
    }

    void String::swap(String & other)
    {
        CHECKED(*this);
        std::swap(m_bufsize, other.m_bufsize);
        std::swap(m_used,    other.m_used);
        std::swap(m_buf,     other.m_buf);
    }
        
    std::ostream & operator<<(std::ostream & os, const String & s)
    {
        CHECKED(s);
        for (size_t i = 0; i < s.m_used; i++) {
            os << s.m_buf[i];
        }
        return os;
    }

    std::istream & operator>>(std::istream & is, String & s)
    {
        CHECKED(s);

        s = String(); // clear the string like std::string's operator>> does

        char ch;

        while (is >> ch) {
            if (isspace(ch)) {
                continue;
            } else {
                while (is >> ch) {
                    if (isspace(ch)) {
                        break;
                    } else {
                        s.push_back(ch);
                    }
                }
                return is;
            }           
        }
        return is;
    }

    // iterator implementation
    
    String::iterator::iterator(String * string, size_t index)
        : m_string(string), m_index(index)
    {}

    String::iterator& String::iterator::operator=(const String::iterator & other)
    {
        if (this == &other)
            return *this;
        
        m_string = other.m_string;
        m_index = other.m_index;

        return *this;
    }
    
    String::iterator String::iterator::operator++() throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
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

    String::iterator String::iterator::operator+(size_t n) throw (std::logic_error)
    {
        String::iterator ans = *this;
        if (m_index + n >= m_string->m_used) {
            throw std::logic_error("iterator would go out of bounds");
        } else {
            ans.m_index += n;
            return ans;
        }
    }
    

    char & String::iterator::operator*() const throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
            throw std::logic_error("iterator out of bounds");
        } else {
            return (*m_string)[m_index];
        }
    }

    char * String::iterator::operator->() const throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
            throw std::logic_error("iterator out of bounds");
        } else {
            return &(*m_string)[m_index];
        }
    }

    bool String::iterator::operator!=(const String::iterator& other) const
    {
        return !((*this) == other);
    }

    bool String::iterator::operator==(const String::iterator& other) const
    {
        return (this->m_string == other.m_string) && (this->m_index == other.m_index);
    }

    // const_iterator implementation
    
    String::const_iterator::const_iterator(const String * string, size_t index)
        : m_string(string), m_index(index)
    {}
    
    String::const_iterator String::const_iterator::operator++() throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
            throw std::logic_error("const_iterator out of bounds");
        } else {
            m_index++;
            return *this;
        }
    }

    String::const_iterator String::const_iterator::operator++(int) throw (std::logic_error)
    {
        String::const_iterator ans = *this;
        ++(*this);
        return ans;
    }

    String::const_iterator String::const_iterator::operator+(size_t n) throw (std::logic_error)
    {
        String::const_iterator ans = *this;
        if (m_index + n >= m_string->m_used) {
            throw std::logic_error("iterator would go out of bounds");
        } else {
            ans.m_index += n;
            return ans;
        }
    }


    const char & String::const_iterator::operator*() const throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
            throw std::logic_error("const_iterator out of bounds");
        } else {
            return (*m_string)[m_index];
        }
    }

    const char * String::const_iterator::operator->() const throw (std::logic_error)
    {
        if (m_index >= m_string->m_used) {
            throw std::logic_error("const_iterator out of bounds");
        } else {
            return &(*m_string)[m_index];
        }
    }

    bool String::const_iterator::operator!=(const String::const_iterator& other) const
    {
        return !((*this) == other);
    }

    bool String::const_iterator::operator==(const String::const_iterator& other) const
    {
        return (this->m_string == other.m_string) && (this->m_index == other.m_index);
    }



    String::iterator String::begin()
    {
        CHECKED(*this);
        return String::iterator(this, 0);
    }
    
    String::iterator String::end()
    {
        CHECKED(*this);
        return String::iterator(this, this->m_used);
    }

    String::const_iterator String::begin() const
    {
        CHECKED(*this);
        return String::const_iterator(this, 0);
    }
    
    String::const_iterator String::end() const
    {
        CHECKED(*this);
        return String::const_iterator(this, this->m_used);
    }

    String String::substring(size_t begin, size_t end) const
    {
        CHECKED(*this);

        String ans;

        for (size_t i = begin; i < end && i < m_used; ++i) {
            ans.push_back((*this)[i]);
        }

        return ans;
    }

    String & String::insert(size_t pos1, const String & str)
    {
        CHECKED(*this);

        String tmp;

        for (size_t i = 0; i < pos1; ++i) {
            tmp.push_back(m_buf[i]);
        }

        for (auto it = str.begin(); it != str.end(); ++it) {
            // puts("A");
            tmp.push_back(*it);
            // puts("B");
        }

        for (size_t i = pos1; i < m_used; ++i) {
            tmp.push_back(m_buf[i]);
        }

        *this = tmp;

        return *this;
    }

    String & String::insert (size_t pos1, const String & str, size_t pos2, size_t n)
    {
        CHECKED(*this);
        return insert(pos1, str.substring(pos2, pos2+n));
    }

    String & String::insert ( size_t pos1, const char * s, size_t n )
    {
        CHECKED(*this);
        return insert(pos1, String(s).substring(0, n));
    }

    String & String::insert(size_t pos1, const char * s)
    {
        CHECKED(*this);
        return insert(pos1, String(s));
    }

    String & String::insert(size_t pos1, size_t n, char c)
    {
        CHECKED(*this);
        String tmp;
        for (size_t i = 0; i < n; ++i) {
            tmp.push_back(c);
        }
        return insert(pos1, tmp);
    }

    String::iterator String::insert (String::iterator p, char c)
    {
        String tmp;
        tmp.push_back(c);
        insert(p.m_index, tmp);
        return p;
    }

    void String::insert(iterator p, size_t n, char c)
    {
        String tmp;
        for (size_t i = 0; i < n; ++i) {
            tmp.push_back(c);
        }
        insert(p.m_index, tmp);
    }
}
