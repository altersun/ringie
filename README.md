# ringie
A few exercises involving coding a ring buffer in c, c++

## What's a ring buffer?
[Wikipedia has a good explanation](https://en.wikipedia.org/wiki/Circular_buffer)

## ...but why?
I've found asking a candidate to design and semi-code a ring buffer to be a good interview question for roles where C, C++, or both are going to be used. It lends itself to a lot of good concepts like structs/classes, accounting for corner cases (buffer rollover), and memory allocation, among others. I learn a lot about how a candidate thinks and what they know and hopefully they get to see me as a helpful representative of the interviewing organization rather than a cold quizmaster.

## But this isn't an interview?
Asking about it so many times made curious about different tricks to use when writing a ring buffer. As of this writing there are three implementations:
* C (as a struct)
* C++ (as a class)
* C++ (as a templated class)

## Common features between implementations
* Each instance of a ring buffer can set it's own entry size. Each entry for the buffer must then be the same size, though each buffer may have different entry sizes.
* Buffer space usage and rollover detection are tracked with pointers
* Buffer bytesize handled internally. Setup only requires entry size and number of entries (capacity).
* Common suite of functionality:
  * clear
  * is_empty
  * is_full
  * push
  * pop
  * capacity (the max number of entries)

## Differences between implementations
* Because of language quirks only the non-templated C++ class requires dynamic allocation of memory! And because that implentation relies on size rather than type, `calloc`/`free` is used instead of the more traditional C++ styled `new`/`delete`.
* The C implementation uses function macros for "duck-typing" in a manner of speaking
* The templated C++ version allocates space at compile time so no dynamic memory allocation is required.
* The templated C++ implementation is fully defined in the header file with no source file because that's just how templates work `¯\_(ツ)_/¯`
* The C++ implementation has Pop and Push defined in the header because they use the `auto` keyword which results in a function template under the hood and (see above) that's just how templates work `¯\_(ツ)_/¯`

## Things I discoverd while puting this together
* C functions can fit into C++ lambdas just fine so long as you link them without name mangling (it's what the `extern "C"` is for)

## How to use
Check out the code and, in the directory where you checked it out, just run
`make run`
to build everything and run the test program

## That's hardly an exhaustive test!
Yeah this is really meant to be more demonstrative and explanitive than exhaustive.
