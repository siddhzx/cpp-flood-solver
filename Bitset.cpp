#include <iostream>
#include <string>
#include <math.h>
#include <deque>

using namespace std;
#define ll long long
#define ull unsigned long long
#define ull_size (sizeof(ull)*8) // ORDER OF OPERATIONS HERE IS REQUIRED WTF
class rtbs {
private:
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
        // @important - by default set all to 1 (~0 unsigned = 1^ set)
        for(ull i = 0; i<size;++i)bits[i]= (0); 
    }

    ~rtbs() { delete[] bits; }

    void p() {
        cout<<string(ull_size, '*')<<endl;
        for (ull i = 0; i < size; ++i){
            ull n = bits[i];
            cout<<i<<"; "<<n<<"; "<<ull_bin(n)<<endl;
        }
        cout<<string(ull_size, '*')<<endl;
    }

    /*
        0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
        0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
    */
    void s(ull pos) {
        ull idx = pos/ull_size;
        ull set = (ull_size-1-pos);
        cout<<"test: "<<ull_size<<";"<<idx<<";"<<set<<endl;
        bits[idx] |= (1ull<<set);
    }

    bool isS(ull pos) {
        ull idx = pos/ull_size;
        ull set = (ull_size-1-pos);
        return bits[idx] & (1ull<<set);
    }
};

int main() {
    rtbs b(2);
    b.p();
    cout<<endl;
    b.s(0);
    cout<<"is set "<<b.isS(0)<<endl;
    b.s(1);
    b.s(5);
    b.s(7);
    b.s(9);
    b.s(64);
    b.s(63);
    b.p();
}