#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

class slist
{
public:
    slist()
    {
        head = 0;
    }

    // copy-ctor
    slist(slist & sl)
    {
        node *dest = new node(sl.head->elem);
        head = dest;

        for (slist::iterator src = ++sl.begin(); src != sl.end(); ++src) {
            dest->next = new node(*src);
            dest = dest->next;
        }

    }

    // assignment
    slist & operator=(slist & sl)
    {
        if (&sl != this) {
            // first, delete ALL the nodes in our list
            for (node *np = head; np != 0; np = np->next) {
                delete np;
            }

            node *dest = new node(sl.head->elem);
            head = dest;
            
            for (slist::iterator src = ++sl.begin(); src != sl.end(); ++src) {
                dest->next = new node(*src);
                dest = dest->next;
            }

        }

        return *this;
    }

    ~slist()
    {
        node *cur = head;

        while (cur != 0) {
            node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    void push_front(const string & s)
    {
        if (head == 0) {
            head = new node(s);
        } else {
            node *old_head = head;
            head = new node(s, old_head);
        }
    }

    void pop_front() throw (logic_error)
    {
        if (head == 0) {
            throw logic_error("list was empty");
        } else {
            node *old_head = head;
            head = old_head->next;
        }
    }

    string & front() const throw (logic_error)
    {
        if (head == 0) {
            throw logic_error("list was empty");
        } else {
            return head->elem;
        }
    }

private:
    struct node
    {
        node(const string & s)
            : elem(s), next(0)
        {}

        node(const string & s, node * next)
            : elem(s), next(next)
        {}

        string elem;
        node *next;
    } *head;

public:
    class iterator
    {
        friend class slist;
    private:
        iterator(node *n)
            : cur_node(n)
        {}
        
    public:
        iterator operator++() throw (logic_error)
        {
            if (cur_node == 0) {
                throw logic_error("cur_node == 0");
            } else {
                cur_node = cur_node->next;
                return *this;
            }
        }

        iterator operator++(int) throw (logic_error)
        {
            iterator ans = *this;
            ++(*this);
            return ans;
        }

        string & operator*() throw (logic_error)
        {
            if (cur_node == 0) {
                throw logic_error("cur_node == 0");
            } else {
                return cur_node->elem;
            }
        }

        string * operator->() throw (logic_error)
        {
            if (cur_node == 0) {
                throw logic_error("cur_node == 0");
            } else {
                return &cur_node->elem;
            }
        }

        bool operator!=(const iterator& other)
        {
            return cur_node != other.cur_node;
        }


        node *cur_node;

    };

    iterator begin()
    {
        return iterator(head);
    }

    iterator end()
    {
        return iterator(0);
    }


};

int main()
{
    slist l;

    l.push_front("a");
    l.push_front("b");
    l.push_front("c");

    cout << "list 1:" << &l << endl;

    for (slist::iterator it = l.begin(); it != l.end(); it++) {
        cout << *it << " ";
    }
    

    slist l2(l);

    cout << endl << "list 2:" << &l2 << endl;

    for (slist::iterator it = l2.begin(); it != l2.end(); it++) {
        cout << *it << " ";
    }

    slist l3;

    l3.push_front("x");
    l3.push_front("y");
    l3.push_front("z");
    
    cout << endl << "list 3:" << &l3 << endl;

    for (slist::iterator it = l3.begin(); it != l3.end(); it++) {
        cout << *it << " ";
    }

    l3 = l2;

    cout << endl << "list 3:" << &l3 << endl;

    for (slist::iterator it = l3.begin(); it != l3.end(); it++) {
        cout << *it << " ";
    }


}
