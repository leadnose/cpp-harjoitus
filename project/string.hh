#include <exception> // std::exception
#include <cstring> // size_t
#include <iostream>

namespace jpr
{
    class InvalidStringException : public std::exception
    {
    public:
        explicit InvalidStringException(const char *msg);
        const char *what() const throw();
    private:
        const char *m_msg;
    }; // class InvalidStringException

    class String {
    public:
        // validity-check
        void check() throw (InvalidStringException);
        // for debugging
        void dump_state(std::ostream &os = std::cerr);
        // default constructor
        String();
        // copy constructor
        String(const String &other);
        // constructor from C-string, which must be null-terminated, and may be deallocated after the call if necessary
        String(const char *cstr);
        // assignment
        String & operator= (const String & other);
        // destructor
        ~String();
        // equality comparison
        bool operator==(const String& other) const;
        // non-equality
        bool operator!=(const String& other) const;
        // indexing
        char & operator[](size_t index) const;
        // input operator
        friend std::istream & operator>>(std::istream & is, String & s);        
        // output operator 
        friend std::ostream & operator<<(std::ostream & os, const String & s);
        // push_back
        void push_back(char c);
        // pop_back
        void pop_back();
        // swap
        void swap(String & other);

        class iterator
        {
            friend class String; // so that String may use the ctor
            
        private:
            // this can be called from String::begin(), ::end() etc. but arbitrary indexes can't be used
            iterator(String & string, size_t index); // TODO: make it so that the type of index is signed, and that it's maximum positive value is >= maxvalue of size_t?
        public:
            // all the operations will throw a std::logic_error if they detect
            // that you're doing something that doesn't make sense
            iterator operator++() throw (std::logic_error);
            iterator operator++(int) throw (std::logic_error);
            char & operator*() throw (std::logic_error);
            char * operator->() throw (std::logic_error);
            bool operator!=(const jpr::String::iterator& other);

        private:
            String & m_string;
            size_t m_index;
        }; // class String::iterator

        // acquiring iterators
        iterator begin();
        iterator end();

    private:
        size_t m_bufsize, m_used;
        char* m_buf;
        
    }; // class String
    
} // namespace jpr
