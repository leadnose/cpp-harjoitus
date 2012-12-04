#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>

#include "string.hh"

typedef void(*testfun)();

using jpr::String;

template <typename T>
void check_equal(const T& t1, const T& t2)
{
    if (t1 == t2) {
        return;
    } else {
        throw std::logic_error("check_equal() failed");
    }
}

template <typename T>
void check_non_equal(const T& t1, const T& t2)
{
    if (t1 != t2) {
        return;
    } else {
        throw std::logic_error("check_non_equal() failed");
    }
}

static
void default_constructor_test()
{
    String s;
    s.dump_state();
}

static
void cstring_constructor_test1()
{
    String s("asdf");
    s.dump_state();
}

static
void cstring_constructor_test2()
{
    String s(0);
    s.dump_state();
}

static
void copy_constructor_test()
{
    String s1 = "asdf";
    String s2 = s1;

    check_equal(s1, s2);
}

static
void size_test()
{
    String s0;
    check_equal(s0.size(), 0ul);

    String s4("asdf");
    check_equal(s4.size(), 4ul);
}

static
void equality_test1()
{
    String s1 = "asdf";
    String s2 = "asdf";

    check_equal(s1, s2);
}

static
void equality_test2()
{
    String s1 = "asdf";
    String s2 = "xcvb";

    check_non_equal(s1, s2);
}

static
void assignment_test()
{
    String s1 = "test";
    String s2 = "asdf";

    s2 = s1;

    s2.check();
}

static
void index_test1()
{
    String s;

    try {
        s[0];
    } catch (std::out_of_range const & e) {
        // std::cerr << e.what() << std::endl;
        return;
    }
    throw std::logic_error("indexing should have failed");
}

static
void index_test2()
{
    String s = "asdf";

    char c = s[0];

    check_equal(c, 'a');
}

static
void index_test3()
{
    String s = "asdf";

    try {
        s[1234];
    } catch (std::out_of_range const & e) {
        return;
    }
    throw std::logic_error("indexing should have failed");
}

static
void push_back_test()
{
    String s;

    for (const char *cp = "asdf"; *cp; cp++) {
        s.push_back(*cp);
    }

    check_equal('a', s[0]);
    check_equal('s', s[1]);
    check_equal('d', s[2]);
    check_equal('f', s[3]);
}

static
void pop_back_test()
{
    String s = "asdf";
    String s2 = "asdf2";

    s2.pop_back();

    check_equal(s, s2);

    s.pop_back();
    s2.pop_back();
    check_equal(s, s2);

    s.pop_back();
    s2.pop_back();
    check_equal(s, s2);

    s.pop_back();
    s2.pop_back();
    check_equal(s, s2);

    s.pop_back();
    s2.pop_back();
    check_equal(s, s2);

    try {
        s.pop_back();
    } catch (const std::logic_error & e) {
        return;
    }

    throw std::logic_error("pop_back() on empty string didn't fail");

}

static
void swap_test()
{
    String s1 = "asdf";
    String s2 = "xcvb";

    String cmp1 = "asdf";
    String cmp2 = "xcvb";

    check_equal(s1, cmp1);
    check_equal(s2, cmp2);

    s1.swap(s2);

    check_equal(s1, cmp2);
    check_equal(s2, cmp1);
}

static
void input_operator_test()
{
    std::vector<String> strings;

    String s;

    std::stringstream ss("asdf xcvb qwer hjkl");

    while (ss >> s) {
        strings.push_back(s);
    }

    for (auto it = strings.begin(); it != strings.end(); ++it) {
        std::cout << *it << std::endl;
    }

    std::vector<String> cmp = {"asdf", "xcvb", "qwer", "hjkl"};
    check_equal(strings, cmp);

}

static
void iterator_test1()
{
    const char *cstr = "qwerty";
    String s = cstr;

    String::iterator it = s.begin();
    size_t i = 0;

    while (it != s.end()) {
        check_equal(*it, cstr[i]);
        ++it;
        ++i;
    }
}

static
void iterator_test2()
{
    String s;
    String::iterator it = s.begin();

    try {
        char c = *it;
        (void) c;
    } catch (const std::logic_error & e) {
        return;
    }

    throw std::logic_error("dereferencing the iterator should've failed");
}

static
void iterator_test3()
{
    String s("a");
    String::iterator it = s.begin();

    try {
        it++;
        char c = *it;
        (void) c;
    } catch (const std::logic_error & e) {
        return;
    }

    throw std::logic_error("dereferencing the iterator should've failed");
}

static
void substring_test()
{
    String s = "alavilla mailla hallan vaara";
    const String s2 = s.substring(0, strlen("alavilla"));
    const String cmp = "alavilla";

    check_equal(s2, cmp);
}

static
void insert_test1()
{
    String str = "to be question";
    const String str2 = "the ";
    const String cmp = "to be the question";

    str.insert(6,str2);
    check_equal(str, cmp);
}

static
void insert_test2()
{
    String str = "to be question";
    String str2 = "the ";
    String str3 = "or not to be";
    String cmp = "to be not the question";
    
    str.insert(6, str2);                 // to be (the )question
    str.insert(6, str3, 3, 4);             // to be (not )the question

    check_equal(str, cmp);
}


#define TEST(f) std::make_pair(f, #f)

std::vector<std::pair<testfun, std::string>> tests =
    {TEST(default_constructor_test),
     TEST(cstring_constructor_test1),
     TEST(cstring_constructor_test2),
     TEST(copy_constructor_test),
     TEST(size_test),
     TEST(equality_test1),
     TEST(equality_test2),
     TEST(assignment_test),
     TEST(index_test1),
     TEST(index_test2),
     TEST(index_test3),
     TEST(push_back_test),
     TEST(pop_back_test),
     TEST(swap_test),
     TEST(input_operator_test),
     TEST(iterator_test1),
     TEST(iterator_test2),
     TEST(iterator_test3),
     TEST(substring_test),
     TEST(insert_test1),
     TEST(insert_test2),
    };

#undef TEST

int main()
{
    size_t successes = 0, failures = 0;

    std::vector<std::string> failed_test_names;

    for (size_t i = 0; i < tests.size(); ++i) {
        try {
            std::cout << "running test " << i+1 << " of " << tests.size() << ": " << tests.at(i).second << "... ";
            tests.at(i).first();
            std::cout << " ok" << std::endl;
            successes++;
        } catch (const std::exception &e) {
            std::cout << "test failed: " << e.what() << std::endl;
            failed_test_names.push_back(tests[i].second);
            failures++;
        }
    }

    std::cout << "Ran " << tests.size() << " tests, " << successes << " successes, " << failures << " failures." << std::endl;

    std::cout << "Failed tests were:" << std::endl;

    for (size_t i = 0; i < failed_test_names.size(); ++i) {
        std::cout << failed_test_names.at(i) << std::endl;
    }

    if (failures) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}
