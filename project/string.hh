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
        void check() const throw (InvalidStringException);
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
        // size()
        size_t size() const;
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

        // Various inserts (from http://www.cplusplus.com/reference/string/string/insert/ )
        // Inserts a copy of the entire string object str at character position pos1.
        String& insert ( size_t pos1, const String& str );
        
        // Inserts a copy of a substring of str at character position pos1. The substring is the portion of str that begins at the character position pos2 and takes up to n characters (it takes less than n if the end of str is reached before).
        String& insert ( size_t pos1, const String& str, size_t pos2, size_t n );
        
        // Inserts at the character position pos1, a copy of the string formed by the first n characters in the array of characters pointed by s.
        String& insert ( size_t pos1, const char * s, size_t n );
        
        // Inserts at character position pos1, a copy of the string formed by the null-terminated character sequence (C string) pointed by s. The length of this character sequence is determined by the first ocurrence of a null character (as determined by traits.length(s)).
        String& insert ( size_t pos1, const char * s );
        
        // Inserts a string formed by a repetition of character c, n times, at the character position pos1.
        String& insert ( size_t pos1, size_t n, char c );
        
        // Inserts a copy of character c at the position referred by iterator p and returns an iterator referring to this position where it has been inserted.
        // iterator insert ( iterator p, char c );
        
        // Inserts a string formed by the repetition of character c, n times, at the position referred by iterator p.
        // void insert ( iterator p, size_t n, char c );
        
        // Inserts at the internal position referred by p the content made up of the characters that go from the element referred by iterator first to the element right before the one referred by iterator last. 
        // template<class InputIterator> void insert (iterator p, InputIterator first, InputIterator last);
        

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
            char & operator*() const throw (std::logic_error);
            char * operator->() const throw (std::logic_error);
            bool operator!=(const jpr::String::iterator& other) const;
            bool operator==(const jpr::String::iterator& other) const;

        private:
            String & m_string;
            size_t m_index;
        }; // class String::iterator

        class const_iterator
        {
            friend class String; // so that String may use the ctor
            
        private:
            // this can be called from String::begin(), ::end() etc. but arbitrary indexes can't be used
            const_iterator(const String & string, size_t index); // TODO: make it so that the type of index is signed, and that it's maximum positive value is >= maxvalue of size_t?
        public:
            // all the operations will throw a std::logic_error if they detect
            // that you're doing something that doesn't make sense
            const_iterator operator++() throw (std::logic_error);
            const_iterator operator++(int) throw (std::logic_error);
            const char & operator*() const throw (std::logic_error);
            const char * operator->() const throw (std::logic_error);
            bool operator!=(const jpr::String::const_iterator& other) const;
            bool operator==(const jpr::String::const_iterator& other) const;

        private:
            const String & m_string;
            size_t m_index;
        }; // class String::const_iterator


        // acquiring iterators
        iterator begin();
        iterator end();
        // const_iterators
        const_iterator begin() const;
        const_iterator end() const;


    private:
        size_t m_bufsize, m_used;
        char* m_buf;
        
    }; // class String
    
} // namespace jpr
