# CPgen


## Quick Start

Seeing the awkward situation happened in the 2023 ICPC Asia Xiaan Regional Contest, The author wrote this.  

CPgen stands for Competitive Programming Data Generator.It is a project aimed to be a useful generator that can safely and conveniently used in Competitive Programming(OI, ICPC, etc.).  

CPgen is hoped to be a library that help problem makers to save their time on data-making.

Here is an example of a generator using CPgen:
```cpp
#include "../generator/generator.h"

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    int n = rnd.next(1, 1000), m = rnd.next(1, 1000);
    println(n, m);
    Array<int> arr; arr.basic_gen(m, 1, 10 * sqrt(n)).print();
}
```
This example generates an array with size of \f$m\f$, whose elements are integers from \f$1\f$ to \f$10\sqrt{n}\f$, and print \f$n, m\f$ and the array to the standard output.

You should note that CPgen is based on testlib. So you are required to have [testlib.h](https://github.com/MikeMirzayanov/testlib) in your working directory (or in your environment path).

To get the latest version of CPgen, download its source code via [repo](https://github.com/Piggy424008/CPgen).

## Usages

When looking through the docs of CPgen, you should note that the first place in std::vector is ignored by CPgen. That's to say, every vector used in CPgen is 1-indexed. And, `std::vector<_Tp>().size()` means `*this.size() - 1` in the implement.

Read Docs for further information.

## FAQs

- I generate exactly the same data while I run it many times. Why is that?  

It indeed testlib fault. To ensure the generator can generator exactly the same data somehow, the seed of rnd(the RNG of testlib) is calculated by the command you type when you ran it. Try to run it with different args. For example, try:
```
>>> ./foo CPgen
>>> ./foo Piggy424008
>>> ./foo cplusplus
```
Then it's expected to generate some different data.