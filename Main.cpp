#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <string>
#include <algorithm>
#include <set>
#include <queue>

#define l long
#define ll long long
#define LLMAX 9223372036854775807LL
#define DEBUG 
#define TEST

struct CoorSystem {
    std::vector<ll> dims;
    std::vector<ll> board;
    ll colors;
    ll get(std::vector<ll>);
};

ll CoorSystem::get(std::vector<ll> coordinates) {
    /* stride access pattern */
    if (coordinates.empty() || coordinates.size() > dims.size()) return board[0];
    
}

/* helpers */
// @todo format better in the future
void print_board(std::vector<ll> board) {
    // @todo - save to txt file for use in javascript / Three.js visualizations / useful for web (integrate this C++ code onto a web app)
    ll bufferSize = 15;
    for (int i = 0; i < board.size(); ++i) {
        std::cout << board[i];
        if (i == board.size() - 1) std::cout << std::endl;
        else if ((i + 1) % bufferSize == 0 ) std::cout << ", " << std::endl;
        else std::cout << ", ";   
    }
}

void print_board(CoorSystem System) {
    print_board(System.board);
}

void debug_prints(CoorSystem System) {
    #ifdef DEBUG
    std::cout << "byte size of system: " << sizeof(System) << std::endl;
    std::cout << "byte size of board vec: " << sizeof(System.board) << std::endl;
    std::cout << "byte size of dims vec: " << sizeof(System.dims) << std::endl; //! 8 - 64 bit addresses on windows, 8 for long, 8 for long (int 4)
    if (System.board.size() >= 1) std::cout << "byte size of first board el: " << sizeof(System.board[0]) << std::endl;
    if (System.board.size() >= 1) std::cout << "byte size of first dims el: " << sizeof(System.dims[0]) << std::endl;
    std::cout << "num elements in board: " << System.board.size() << std::endl;
    print_board(System);
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
    // accessing undefined memory led to segfault bruh.
    auto [isRandomInit, colors, initState] = ifRandElseState;
    System.colors = colors;
    if (isRandomInit) {
        #ifdef TEST
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
ll solver(CoorSystem System) {
    /* by default the goal is to get from 0,0,...,0 to n,n,...,n where n = dim.size()*/
    std::vector<ll> board = System.board;
    ll colors = System.colors;

    // assume 2d setup
    ll rows = System.dims[0];
    ll cols = System.dims[1];
    std::tuple<ll, ll> curr = std::tuple{0LL, 0LL};
    std::set<std::tuple<ll, ll>> visited;
    std::priority_queue<std::tuple<ll, ll, ll>> pq;
    pq.push(std::make_tuple(LLMAX, 0, 0));
    std::vector<std::tuple<int, int>> directions = {std::tuple{1, 1}, std::tuple{0, 1}, std::tuple{1, 0}}; 

    while (!pq.empty()) {
        auto [price, x, y] = pq.top();
        pq.pop();

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
    CoorSystem System;
    CoorSystem SystemInit;
    #ifdef DEBUG
    std::cout << "inside debug mode." << std::endl; 
    #endif

    System = init_board(
        System,                                                             //! default coordinate system
        std::vector {3LL, 3LL, 3LL, 3LL},                                           //! default, hard 14 x 14 array 
        std::tuple<bool, ll, std::vector<ll>> {true, 6, std::vector<ll>(0)} //! default, random init with 6 colors
    );

    SystemInit = init_board(
        SystemInit,
        std::vector {2LL, 2LL},
        std::tuple<bool, ll, std::vector<ll>> {false, 4, std::vector {0LL, 1LL, 2LL, 3LL}}
    );

    debug_prints(System);
    debug_prints(SystemInit);

    std::cout << "getting: " << System.get({0, 0, 0, 0}) << std::endl;
    std::cout << "getting: " << System.get({0, 1, 0, 0}) << std::endl;
    std::cout << "getting: " << System.get({0, 0, 1, 0}) << std::endl;
    std::cout << "getting: " << System.get({0, 0, 0, 1}) << std::endl;
    ll ans = solver(System);
    std::cout << ans << std::endl;

    return 0;
};