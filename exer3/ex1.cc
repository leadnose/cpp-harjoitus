#include <vector>
#include <iostream>

using std::swap;
using std::cout;
using std::vector;
using std::ostream;
using std::endl;

template <typename T>
vector<T> reverse(const vector<T> & invec)
{
    vector<T> ans;
    ans.reserve(invec.size());

    for (auto it = invec.rbegin(); it != invec.rend(); ++it) {
        ans.push_back(*it);
    }

    return ans;
}

template <typename T>
void in_place_reverse(vector<T> & invec)
{
    for (size_t lo = 0, hi = invec.size()-1; lo < hi; swap(invec.at(lo++), invec.at(hi--)));
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T> & v)
{
    os << '[';
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << *it << ' ';
    }
    return os << ']';
}

int main()
{
    vector<int> iv = {1, 2, 3, 4, 5};
    cout << iv << endl;
    auto iv2 = reverse(iv);
    cout << "iv: " << iv << endl;
    cout << "iv2: " << iv2 << endl;

    in_place_reverse(iv);
    cout << "iv: " << iv << endl;
    cout << "iv2: " << iv2 << endl;
    
}
