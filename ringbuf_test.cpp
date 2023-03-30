
#include <iostream>
#include <string>
#include <random>
#include <functional>

#include "ringbuf.h"
#include "ringbuf.hpp"
#include "ringbuf_template.hpp"

using namespace std;

namespace {
    static const int MAX = 50;
    static const int MIN = -MAX;
    static const int ROUNDS = 5;
    static const int SIZE = 5;
}


struct RingTest {
    string Name;
    std::function<bool()> IsEmpty;
    std::function<bool()> IsFull;
    std::function<int(int)> Push;
    std::function<int(int&)> Pop;
};



void run_test(RingTest& rt) {

    // From https://stackoverflow.com/a/19728404
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(MIN,MAX); // guaranteed unbiased

    cout << "Testing: " << rt.Name << endl;


    // TEST 1
    cout << endl << "FULL/EMPTY TEST" << endl;
    for (int loop=0; loop<ROUNDS; ++loop) {
        cout << endl << "ROUND " << loop << endl;
        while (!rt.IsFull()) {
            int to_push = uni(rng);
            cout << "Pushing " << to_push << endl;
            if (0 != rt.Push(to_push)) {
                cout <<  "Couldn't push anymore!" << endl;
                break;
            }
        }
        while (!rt.IsEmpty()) {
            int popped;
            if (0 != rt.Pop(popped)) {
                cout << "Couldn't pop anymore!" << endl;
                break;
            }
            cout << "Popped " << popped << endl;
        }
    }

    // TEST 2
    cout << endl << "ROLLOVER TEST" << endl;
    for (int loop=0; loop<ROUNDS; ++loop) {
        cout << endl << "ROUND " << loop << endl;
        for (int sub=0; sub<SIZE-1; ++sub) {
            int to_push = uni(rng);
            cout << "Pushing " << to_push << endl;
            int derp = rt.Push(to_push);
            if (0 != derp/*rt.Push(to_push)*/) {
                cout <<  "Couldn't push anymore!" << endl;
                break;
            }
        }
        while (!rt.IsEmpty()) {
            int popped;
            if (0 != rt.Pop(popped)) {
                cout << "Couldn't pop anymore!" << endl;
                break;
            }
            cout << "Popped " << popped << endl;
        }
    }
}



int main() {

    // Invoke one of each type
    RINGBUF_INIT(ringieC, int, SIZE);
    RingBufCpp ringieCpp(sizeof(int), SIZE);
    RingBufTemplate<int, SIZE> ringieT;
    
    // Fill out our test table using fancy C++ lambda functions
    // They can even wrap C functions!
    RingTest rt_array[] = { 
        {   .Name="Standard C",
            .IsEmpty=[&] () {return ringbuf_is_empty(&ringieC);},
            .IsFull=[&] () {return ringbuf_is_full(&ringieC);},
            .Push=[&] (int to_push) {return RINGBUF_PUSH(&ringieC, to_push);},
            .Pop=[&] (int& popped) {return RINGBUF_POP(&ringieC, popped);},
        },
        {   .Name="C++",
            .IsEmpty=[&] () {return ringieCpp.IsEmpty();},
            .IsFull=[&] () {return ringieCpp.IsFull();},
            .Push=[&] (int to_push) {return ringieCpp.Push(to_push);},
            .Pop=[&] (int& popped) {return ringieCpp.Pop(popped);},
        },
        {   .Name="C++ Template",
            .IsEmpty=[&] () {return ringieT.IsEmpty();},
            .IsFull=[&] () {return ringieT.IsFull();},
            .Push=[&] (int to_push) {return ringieT.Push(to_push);},
            .Pop=[&] (int& popped) {return ringieT.Pop(popped);},
        },
    };


    for (auto &rt : rt_array) {
        run_test(rt);
        cout << endl << endl;
    }

    cout << "DONE!" << endl;

    return 0; 
}