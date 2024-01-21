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
This example generates an array with size of $m$, whose elements are integers from $1$ to $10\sqrt{n}$, and print $n, m$ and the array to the standard output.

You should note that CPgen is based on testlib. So you are required to have [testlib.h](https://github.com/MikeMirzayanov/testlib) in your working directory (or in your environment path).

To get the latest version of CPgen, download its source code via [repo](https://github.com/Piggy424008/CPgen).

## Usages

When looking through the docs of CPgen, you should note that the first place in std::vector is ignored by CPgen. That's to say, every vector used in CPgen is 1-indexed.

### Basis

Function `void Quit(Args... params)`:  
|name|description|
|--|--|
|brief|Print some infomation to standard output and quit.|
|params|Any. Anything that can be printed, with any number.|
|return|No return.|

Function `inline i64_ll qpow(i64_ll a, i64_ll b, i64_ll mod)`:  
|name|description|
|--|--|
|brief|calculate $a^b\bmod mod$.|
|$a, b, mod$|Any `i64_ll`.|
|return|Return the result.|

Function `inline bool is_prime(i64_ll n)`:  
|name|description|
|--|--|
|brief|Check if $n$ is a prime.|
|$n$|Any `i64_ll`.|
|return|Return the result.|

### class `_random`
Method `inline _Tp choice(std::vector<_Tp> array, int l = 1, int r = -1)`:  
|name|description|
|--|--|
|brief|Randomly choose an element in array, indexes from $l$ to $r$.|
|array|Any std::vector<_Tp>.|
|$l$|The left bound that should be choosed.Default as $1$.|
|$r$|The right bound that should be choosed.Default as `array.size()`.|
|return|The chosen element.|

Method `inline std::vector<_Tp> shuffle(std::vector<_Tp> array, int l = 1, int r = -1)`:  
|name|description|
|--|--|
|brief|Shuffle the array in-place, indexes from $l$ to $r$.|
|array|Any std::vector<_Tp>.|
|$l$|The left bound that should be shuffled.Default as $1$.|
|$r$|The right bound that should be shuffled.Default as `array.size()`.|
|return|The shuffled array.|

Method `inline _Tp get_prime(_Tp l, _Tp r)`:  
|name|description|
|--|--|
|brief|get a prime $p\in[l, r]$.|
|$l$|The left bound of the section.|
|$r$|The right bound of the section.|
|return|The generated prime.|
|throw|When it failed to gen a prime after $5$ times of iteration, it throws an error `I suspected that there's no prime from {l} to {r}.` and quit the program.|

## FAQs

- I generate exactly the same data while I run it many times. Why is that?  

It indeed testlib fault. To ensure the generator can generator exactly the same data somehow, the seed of rnd(the RNG of testlib) is calculated by the command you type when you ran it. Try to run it with different args. For example, try:
```
>>> ./foo CPgen
>>> ./foo Piggy424008
>>> ./foo cplusplus
```
Then it's expected to generate some different data.
