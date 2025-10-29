#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <string>
#include <algorithm>
#include <set>
#include <queue>
#include <numeric>
#include <map>
#include <math.h>
#include <deque>

#define l long
#define ll long long
#define LLMAX 9223372036854775807LL
// #define DEBUG 
// #define STRIDE_TEST
#define BITSET_TEST
// #define MAIN

using namespace std;

#define ull unsigned long long
#define ull_size (sizeof(ull)*8) 
class rtbs {
protected:
    ull* bits;
    ull size;
    string ull_bin(ull n){
        string ret;
        if (n == 0) {return "0";}
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

    void s(ull pos) {
        ull idx = pos/ull_size;
        ull set = (ull_size-1-(pos%ull_size));
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

class bitset: public rtbs {
public:
    bitset():rtbs(0) {}
    bitset(ull n):rtbs(n) {} 
    int get(ull start, ull end) {
        ull ret = 0; ull offset = 0;
        for (auto i = start; i<end; ++i)
            ret ^= (isS(i)<<offset++);
        return ret; 
    }

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
    using rtbs::s; 
    void s(std::vector<ull> pts) {
        for (auto i : pts) s(i);
    }
};

struct CoorSystem {
    std::vector<ll> dims;
    std::vector<ll> board;
    ll colors;
    ll size;
    // ll -> ull might be problematic rip.
    ll coordinate_getter(std::vector<ll>);
    ll get(std::vector<ll>);
    void print_board();
    void print_board(ll);
    ll solver();
    bitset visited;
    bool visit(std::vector<ll>);
    bool is_visited(std::vector<ll>);
};

/* helpers */
ll CoorSystem::coordinate_getter(std::vector<ll> coordinates) {
    if (coordinates.empty() || coordinates.size() != dims.size()) throw std::invalid_argument("Coordinates size must match with dimension size.");
    ll stride = 1;
    ll idx = 0;

    for (int i = coordinates.size() - 1; i >= 0; i--) {
        if (coordinates[i] >= dims[i]) 
        throw std::invalid_argument(
            "Index " + std::to_string(i) 
            + " was out of range. Requested coordinate was: "
            + std::to_string(coordinates[i])
            + "; while dimension was: "
            + std::to_string(dims[i]));
        idx += stride * coordinates[i]; // smallest delta first, delta grows as we get closer to the first index. why? because the first index chooses from the biggest "chunk" (cuts the most space out)
        stride *= dims[i];
    }
    if (idx > board.size()) throw std::invalid_argument("Index out of range.");

    return idx;
}

ll CoorSystem::get(std::vector<ll> coordinates) {
    return board[coordinate_getter(coordinates)];
}

bool CoorSystem::is_visited(std::vector<ll> coordinates) {
    return visited.isS(coordinate_getter(coordinates));
}

bool CoorSystem::visit(std::vector<ll> coordinates) {
    ll idx = coordinate_getter(coordinates);
    visited.s(idx);
    return visited.isS(idx);
}
// @todo format better in the future
void print_board(std::vector<ll> board, ll bufferSize) {
    // @todo - save to txt file for use in javascript / Three.js visualizations / useful for web (integrate this C++ code onto a web app)
    for (int i = 0; i < board.size(); ++i) {
        std::cout << board[i];
        if (i == board.size() - 1) std::cout << std::endl;
        else if ((i + 1) % bufferSize == 0 ) std::cout << ", " << std::endl;
        else std::cout << ", ";   
    }
}

void print_board(std::vector<ll> board) {
    print_board(board, 10);
}

void CoorSystem::print_board(ll bufferSize) {
    ::print_board(board, bufferSize);
}

void CoorSystem::print_board() {
    ::print_board(board, 10);
}

void debug_prints(CoorSystem System) {
    #ifdef DEBUG
    std::cout << "byte size of system: " << sizeof(System) << std::endl;
    std::cout << "byte size of board vec: " << sizeof(System.board) << std::endl;
    std::cout << "byte size of dims vec: " << sizeof(System.dims) << std::endl; //! 8 - 64 bit addresses on windows, 8 for long, 8 for long (int 4)
    if (System.board.size() >= 1) std::cout << "byte size of first board el: " << sizeof(System.board[0]) << std::endl;
    if (System.board.size() >= 1) std::cout << "byte size of first dims el: " << sizeof(System.dims[0]) << std::endl;
    std::cout << "num elements in board: " << System.board.size() << std::endl;
    #endif
}

/* core */
std::vector<ll> gen_rand_board(ll colors, ll size) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, (unsigned int) colors); // distribution in range [1, #colors]
    
    std::vector<ll> ret;
    while (--size >= 0) {
        ret.push_back((ll)dist6(rng));
    }

    #ifdef DEBUG
    std::cout << "printing board from gen_rand_board bc. debug mode enabled." << std::endl;
    print_board(ret);
    #endif

    return ret;
}

std::vector<ll> gen_increasing_board(ll colors, ll size) {
    std::vector<ll> ret;
    for (ll i = 0; i < size; ++i) {
        ret.push_back(i);
    }
    return ret;
}

CoorSystem init_board(CoorSystem System, std::vector<ll> dims, std::tuple<bool, ll, std::vector<ll>> ifRandElseState) { 
    /* initialize n x n x dimension board (typically flood is 1 dimensional n x n grid) 
    
        standard 1d array = [_, _, _] // x * i
        2d array = 
            [ 
                [_, _, _],
                [_, _, _],
                [_, _, _]
            ] // x * i + y * j
        3d array =
            [
                [
                    [_, _, _],
                    [_, _, _],
                    [_, _, _]
                ],
                [
                    [_, _, _],
                    [_, _, _],
                    [_, _, _]
                ],
                [
                    [_, _, _],
                    [_, _, _],
                    [_, _, _]
                ]     
            ] // x * i + y * j + z * k

        x, y, z, ... can be init'd as a vector of dimensions (therefore, dims)
    */
    System.dims = dims;
    ll size = 1;
    bool initd = false;
    for (int i = 0; i < System.dims.size(); ++i) {
        if (System.dims[i] != 0) {
            initd = true;
            size *= System.dims[i];
        }
        else {
            initd = false;
            break;
        }
    }
    if (!initd) {
        throw std::invalid_argument("Must be non-zero dimension.");
    }    
    System.size=size;
    System.visited=bitset(size);
    // accessing undefined memory led to segfault bruh.
    auto [isRandomInit, colors, initState] = ifRandElseState;
    System.colors = colors;
    if (isRandomInit) {
        #ifdef STRIDE_TEST
        System.board = gen_increasing_board(colors, size);
        return System;
        #endif
        System.board = gen_rand_board(colors, size);
    }
    else {
        if (initState.empty() || initState.size() != size) {
            throw std::invalid_argument("Initial state must match dimensional size.");
        }
        if (*std::max_element(initState.begin(), initState.end()) > colors) {
            throw std::invalid_argument("Can't have colors outside of the total scope of colors.");
        }
        System.board = initState;
    }
    return System;
}

/* solver */
ll CoorSystem::solver() {
    /* by default the goal is to get from 0,0,...,0 to n,n,...,n where n = dim.size()*/
    ll colors = colors;

    // assume 2d setup
    ll rows = dims[0];
    ll cols = dims[1];
    ll dimSize = dims.size();
    bitset directions(dimSize);
    /*
        1010
        1100
        0110

        1000
        0100
    */
    for (auto d : dims) 
    
    bitset curr(dimSize); // 0 by default

    std::priority_queue<std::tuple<ll, bitset>> pq;
    
    // pq.push(std::make_tuple(LLMAX, curr));

    while (!pq.empty()) {
        
        
        /*
            simple pq strategy:
            1. pop top point
            2. convert all similarly numbered neighboring points to the current point.
            2. dfs from that space to unvisited nodes
                a. create a list of possible candidates
                b. rank candidates by count
        */


    }

    /*

        ideas: 
        - union find
        - topological sort
        - flood fill algorithm
        - A* (weighted graph)
        - Dijkstra's Algorithm
        - simulation

        approach:
        - figure out a way to organize the data currently
        - apply algorithmic approach to the data
        - create a graphical visual setup to data 

        start = i(0, 0)
        end =   i(3, 3)


        0 1 2 3
        1 2 3 4
        2 3 4 5
        3 4 5 6
    */

    return 0;
}

int main() {
    #ifdef DEBUG
    CoorSystem System;
    CoorSystem SystemInit;

    std::cout << "inside debug mode." << std::endl; 
    System = init_board(
        System,                                                            
        std::vector {3LL, 3LL, 3LL, 3LL, 3LL},                             
        std::tuple<bool, ll, std::vector<ll>> {true, 10, std::vector<ll>(0)}
    );
    ll x = 17, y = 3;
    std::vector<ll> vec(x * y);
    std::iota(vec.begin(), vec.end(), 1);
    SystemInit = init_board(
        SystemInit, 
        std::vector {17LL, 3LL},
        std::tuple<bool, ll, std::vector<ll>> 
            {   false, 
                x * y, 
                vec
            }
    );

    debug_prints(System);
    System.print_board();

    debug_prints(SystemInit);
    SystemInit.print_board(17);

    #endif

    #ifdef STRIDE_TEST
    CoorSystem System;
    CoorSystem SystemInit;

    std::cout << "inside stride test mode." << std::endl; 
    System = init_board(
        System,                                                            
        std::vector {3LL, 3LL, 3LL, 3LL, 3LL},                             
        std::tuple<bool, ll, std::vector<ll>> {true, 6, std::vector<ll>(0)}
    );

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int a = 0; a < 3; ++a) {
                    for (int b = 0; b < 3; ++b) {
                        std::cout << "inside loop: " << System.get({i, j, k, a, b}) << std::endl;
                    }
                }
            }
        }
    }
    System.print_board(3);
    // lolz praying these 5-d coordinate system is correct. if you can visualize this, you might be cracked - email me at contact[at]hzx[dot]ai
    std::cout << "custom test: " << System.get({0, 0, 0, 0, 0}) << std::endl;
    std::cout << "custom test: " << System.get({0, 0, 1, 0, 0}) << std::endl;
    std::cout << "custom test: " << System.get({0, 1, 1, 0, 0}) << std::endl;
    std::cout << "custom test: " << System.get({2, 2, 1, 0, 0}) << std::endl; 
    std::cout << "custom test: " << System.get({2, 2, 1, 0, 0}) << std::endl; 


    #endif

    #ifdef BITSET_TEST
    CoorSystem System;
    System = init_board(
        System,
        std::vector<ll> {14, 14},
        std::tuple<bool, ll, std::vector<ll>> {true, 6, std::vector<ll>{}}
    );
    System.print_board(14);
    bitset bits(3);
    for (int i = 0; i <= 3; i++) {
        bits.s(i);
        std::cout<<bits.isS(i)<<std::endl;
        bits.p();
        bits.z(i);
        std::cout<<bits.isS(i)<<std::endl;
        bits.p();
        std::cout<<std::endl;
    }
    bits.s(std::vector<ull>{0, 1, 3});
    // 1101
    // 1011 
    // 1101
    bits.s(99);
    cout<<bits.get(95, 100)<<endl; // lsb is the leftmost bit, msb is the rightmost
    
    
    System.visited.p();
    
    
    #endif

    #ifdef MAIN
    CoorSystem System;
    System = init_board(
        System,
        std::vector<ll> {14, 14},
        std::tuple<bool, ll, std::vector<ll>> {true, 6, std::vector<ll>{}}
    );
    System.print_board(14);

    auto ans = System.solver();
    std::cout << ans << std::endl;
    #endif
    return 0;
};