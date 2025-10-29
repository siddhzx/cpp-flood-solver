#include <iostream>
#include <string>
#include <math.h>
#include <deque>
#include <vector>

using namespace std;
#define ll long long
#define ull unsigned long long
#define ull_size (sizeof(ull)*8) // ORDER OF OPERATIONS HERE IS REQUIRED WTF
// in the future could have used templates
class rtbs {
protected:
    ull* bits;
    ull size;
    string ull_bin(ull n){
        string ret;
        if (n == 0) {return "0";}
        // @important - 64 overflows to 1 for ull
        for (int i = ull_size-1; i >= 0; i--) {
            ret += (n & (1ull << i))?'1':'0';        
            if (i%4==0&&i!=0) ret+='_';
        }
        return ret;
    }
public:
    rtbs(ull n) {
        size = n;
        bits = new ull[size];
        // @important - by default set all to 0 (but ~0 unsigned = 111...1 set)
        for(ull i = 0; i<size;++i)bits[i]= (0); 
    }

    ~rtbs() { delete[] bits; }

    void p() {
        cout<<string(ull_size, '=')<<endl;
        for (ull i = 0; i < size; ++i){
            ull n = bits[i];
            cout<<"idx: "<<i<<"; ull val: "<<n<<"; \nbinary: "<<ull_bin(n)<<endl;
        }
        cout<<string(ull_size, '=')<<endl;
    }

    /*
        0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
        0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
    */
    void s(ull pos) {
        ull idx = pos/ull_size;
        ull set = (ull_size-1-(pos%ull_size));
        // cout<<"test: "<<ull_size<<";"<<idx<<";"<<set<<endl;
        bits[idx] |= (1ull<<set);
    }

    bool isS(ull pos) {
        ull idx = pos/ull_size;
        ull set = (ull_size-1-pos);
        return bits[idx] & (1ull<<set);
    }

    void z(ull pos) {
        if (isS(pos)) {
            ull idx = pos/ull_size;
            ull set = (ull_size-1-pos);
            bits[idx] ^= (1ull<<set);
        }
    }
};
class test {
public:
    test(ull n) {
        cout<<"BASIJKDBASKJBDKJSABD: "<<n<<endl;
    }
};
class bitset: public rtbs, test {
public:
    bitset(ull n):rtbs(n), test(n) {} // base class constructor initializer, wow! multiple inheritance is v cool
    /*
    bbaa_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
    */
    int get(ull n) {
        if (n < 0 || n > (size*ull_size)) throw invalid_argument("getting an index out of bounds.");
        ull next = (n%2==0) ? n + 1 : n - 1;
        switch (isS(n) + isS(next)) {
            case 0:
                return 0;
            case 1:
                return 1;
            case 2:
                return -1;
            default:
                cout<<"cooked"<<endl;
                return -69420;
        }
    }
    using rtbs::s; // using allows us to access base class s + bitset class s
    void s(std::vector<ull> pts) {
        for (auto i : pts) s(i);
    }
};

int main() {
    /* rtbs = run-time bitset */
    bitset b(4); // gets the -1, 0, 1 positioning
    b.p();
    b.s({1ull, 2ull, 3ull});
    b.p();
    b.s(128);
    b.s(129);
    cout<<"getting: "<<b.get(128)<<endl;
    cout<<"getting: "<<b.get(1)<<endl;
    cout<<"getting: "<<b.get(2)<<endl;
    cout<<"getting: "<<b.get(3)<<endl;
    cout<<"getting: "<<b.get(4)<<endl;
}