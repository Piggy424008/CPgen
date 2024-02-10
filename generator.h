/*! \mainpage CPgen
 *
 * ## Quick Start
 *
 * Seeing the awkward situation happened in the 2023 ICPC Asia Xiaan Regional
 * Contest, The author wrote this.
 *
 * CPgen stands for Competitive Programming Data Generator.It is a project aimed
 * to be a useful generator that can safely and conveniently used in Competitive
 * Programming(OI, ICPC, etc.).
 *
 * CPgen is hoped to be a library that help problem makers to save their time on
 * data-making.
 *
 * Here is an example of a generator using CPgen:
 * ```cpp
 * #include "../generator/generator.h"
 *
 * int main(int argc, char** argv) {
 *     registerGen(argc, argv, 1);
 *     int n = rnd.next(1, 1000), m = rnd.next(1, 1000);
 *     println(n, m);
 *     Array<int> arr; arr.basic_gen(m, 1, 10 * sqrt(n)).print();
 * }
 * ```
 * This example generates an array with size of \f$m\f$, whose elements are
 * integers from \f$1\f$ to \f$10\sqrt{n}\f$, and print \f$n, m\f$ and the array
 * to the standard output.
 *
 * You should note that CPgen is based on testlib. So you are required to have
 * [testlib.h](https://github.com/MikeMirzayanov/testlib) in your working
 * directory (or in your environment path).
 *
 * To get the latest version of CPgen, download its source code via
 * [repo](https://github.com/Piggy424008/CPgen).
 *
 * ## Usages
 *
 * When looking through the docs of CPgen, you should note that the first place
 * in std::vector is ignored by CPgen. That's to say, every vector used in CPgen
 * is 1-indexed. And, `std::vector<_Tp>().size()` means `*this.size() - 1` in
 * the implement.
 *
 * Read Docs for further information.
 *
 * ## FAQs
 *
 * - I generate exactly the same data while I run it many times. Why is that?
 *
 * It indeed testlib fault. To ensure the generator can generator exactly the
 * same data somehow, the seed of rnd(the RNG of testlib) is calculated by the
 * command you type when you ran it. Try to run it with different args. For
 * example, try:
 * ```
 * >>> ./foo CPgen
 * >>> ./foo Piggy424008
 * >>> ./foo cplusplus
 * ```
 * Then it's expected to generate some different data.
 *  */
#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include "testlib.h"

using i64_ll = long long;
using i128_ll = __int128_t;
using pii = std::pair<int, int>;
using pll = std::pair<i64_ll, i64_ll>;

double eps = 1e-12;  ///< epsilon

/**
 *  @brief The exception throwed when errors occured.
 */
struct GenException : public std::exception {
    std::string _msg;
    GenException(std::string msg) { _msg = msg; }
    GenException(const char* msg) { _msg = msg; }
    const char* what() const throw() { return _msg.data(); }
};

/**
 *  @brief  Print some infomation to standard output and quit.
 *  @param  params Any. Anything that can be printed, with any number.
 *  @return No return.
 */
template <typename... Args>
inline void Quit(Args... params) {
    ((std::cout << params << ' '), ...);
    std::cout << "\n";
    exit(1);
}

/**
 *  @brief  calculate \f$a^b\bmod mod\f$.
 *  @param  a Any `i64_ll`.
 *  @param  b Any `i64_ll`.
 *  @param  mod Any `i64_ll`.
 *  @return Return the result.
 */
inline i64_ll qpow(i64_ll a, i64_ll b, i64_ll mod) {
    assert(b >= 0);
    i128_ll ans = 1;
    while (b) {
        if (b & 1)
            ans = 1ll * ans * a % mod;
        a = 1ll * a * a % mod;
        b >>= 1;
    }
    return ans;
}

/**
 *  @brief  Check if \f$n\f$ is a prime.
 *  @param  n Any `i64_ll`.
 *  @return Return the result.
 */
inline bool is_prime(i64_ll n) {
    if (n < 3 || n % 2 == 0)
        return n == 2;
    i64_ll u = n - 1, t = 0;
    while (u % 2 == 0)
        u /= 2, ++t;
    i64_ll ud[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (i64_ll a : ud) {
        i128_ll v = qpow(a, u, n);
        if (v == 1 || v == n - 1 || v == 0)
            continue;
        for (int j = 1; j <= t; j++) {
            v = v * v % n;
            if (v == n - 1 && j != t) {
                v = 1;
                break;
            }
            if (v == 1)
                return 0;
        }
        if (v != 1)
            return 0;
    }
    return 1;
}

/**
 *  @brief  Expansion of random_t.
 */
class _random {
   public:
    /**
     *  @brief  Shuffle the array in-place, indexes from \f$l\f$ to \f$r\f$.
     *  @param array Any std::vector<_Tp>.
     *  @param l The left bound that should be shuffled. Default as \f$1\f$.|
     *  @param r The right bound that should be shuffled. Default as
     * `array.size()`.
     *  @return Return the result.
     */
    template <typename _Tp>
    inline std::vector<_Tp> shuffle(std::vector<_Tp> array,
                                    int l = 1,
                                    int r = -1) {
        if (!~r)
            r = array.size() - 1;
        for (int i = l + 1; i <= r; i++)
            std::swap(array.at(i), array.at(rnd.next(l, i - 1)));
        return array;
    }
    /**
     *  @brief get a prime \f$p\in[l, r]\f$.
     *  @param array Any std::vector<_Tp>.
     *  @param l The left bound  of the section.
     *  @param r The right bound of the section.
     *  @return The generated prime.
     *  @throw When it failed to gen a prime after \f$5\f$ times of iteration,
     * it throws an error `I suspected that there's no prime from {l} to {r}.`
     * and quit the program.
     */
    template <typename _Tp>
    inline _Tp get_prime(_Tp l, _Tp r) {
        int times = 5;
        while (times-- > 0) {
            _Tp base = rnd.next(l, r);
            while (!is_prime(base) && base <= r)
                base++;
            if (base == r)
                continue;
            return base;
        }
        Quit(format("I suspected that there's no prime from %lld to %lld.", l,
                    r));
    }
} _rnd;

/**
 *  @brief print a vector.
 *  @param vec Any std::vector<_Tp>.
 *  @param sep The seperator.
 *  @param end the end char.
 *  @return no return.
 *  @throw Throws exception when element is not print-able.
 */
template <typename T>
inline void print(std::vector<T> vec, char sep = ' ', char end = '\n') {
    for (auto&& i : vec) {
        std::cout << i << sep;
    }
    std::cout << end;
}

#define warn printf

/**
 *  @brief  Class that used to generate a tree.
 */
class Tree {
   public:
    int n;                    ///< The size of the tree.
    std::vector<int> fa;      ///< `fa[i]` is the no. of node \f$i\f$'s father.
    std::vector<int> leaves;  ///< The no. of leaves. NOTE that it would be
                              ///< empty UNLESS you call `get_leaves()` method.
    bool weighted = false;  ///< Denoting if the **edges** are weighted or not.
    using _Self = Tree;
    /**
     *  @brief  Initiate Tree object with size `size`.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline void init(int size) {
        fa.clear(), leaves.clear();
        if (size < 1)
            throw GenException(
                format("Invalid 'n' has been passed in `init`: %d", size));
        n = size;
        fa.resize(size + 1);
    }
    /**
     *  @brief  Generate a tree with an expected height of \f$O(\sqrt n)\f$
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self sqrt_height_tree(int size) {
        init(size);
        std::vector<int> p(size - 1);
        for (int i = 1; i <= size - 2; i++)
            p.at(i) = rnd.next(1, size);
        std::vector<int> d(size + 1);
        for (int i = 1; i <= size - 2; ++i)
            d.at(p.at(i))++;
        p.at(size - 1) = size;
        for (int i = 1, j = 1; i < size; ++i, ++j) {
            while (d.at(j))
                ++j;
            fa.at(j) = p.at(i);
            while (i < n && !--d.at(p.at(i)) && p.at(i) < j)
                fa.at(p.at(i)) = p.at(i + 1), ++i;
        }
        fa.at(n) = fa.at(1), fa.at(1) = 0;
        for (int i = 2; i <= size; i++)
            if (fa.at(i) == 1)
                fa.at(i) = size;
        return *this;
    }
    /**
     *  @brief  Generate a tree with an expected height of \f$O(\log n)\f$
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self log_height_tree(int size) {
        init(size);
        for (int i = 2; i <= size; i++)
            fa.at(i) = rnd.next(1, i - 1);
        return *this;
    }
    /**
     *  @brief  Generate a tree that is a chain.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self chain(int size) {
        init(size);
        for (int i = 2; i <= size; i++)
            fa.at(i) = i - 1;
        return *this;
    }
    /**
     *  @brief  Generate a tree that is a flower.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self flower(int size) {
        init(size);
        for (int i = 2; i <= size; i++)
            fa.at(i) = 1;
        return *this;
    }
    /**
     *  @brief  Generate a tree with an expected max_deg of \f$O(n)\f$.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self n_deg_tree(int size) {
        init(size);
        int flowers_count = rnd.next(1, 10);
        std::vector<int> is_flower(size + 1);
        for (int i = 1; i <= size; i++)
            is_flower.at(i) = 0;
        std::vector<int> nodes;
        for (int i = 1; i <= flowers_count; i++) {
            int node = rnd.next(1, size);
            if (is_flower.at(node) == 1) {
                i--;
                continue;
            }
            is_flower.at(node) = 1;
            nodes.push_back(node);
        }
        for (int i = 2; i <= size; i++)
            if (is_flower.at(i))
                fa.at(i) = 1;
            else
                fa.at(i) = rnd.any(nodes);
        return *this;
    }
    /**
     *  @brief  Generate a tree with chain size is about chain_percent * size
     * and flower size is about flower_percent * size.
     *  @param  size The count of the nodes that will be generated
     *  @param  chain_percent the percent of the chain size.
     *  @param  flower_percent the percent of the flower size.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1,
     * or chain_percent + flower_percent > 1.
     */
    inline _Self chain_and_flower(int size,
                                  double chain_percent = 0.3,
                                  double flower_percent = 0.3) {
        ensure(chain_percent + flower_percent <= 1);
        init(size);
        int i = 2;
        for (; i < size * chain_percent; i++)
            fa.at(i) = i - 1;
        int tmp = i - 1;
        for (; i < size * (chain_percent + flower_percent); i++)
            fa.at(i) = tmp;
        for (; i <= size; i++)
            fa.at(i) = rnd.next(1, i - 1);
        return *this;
    }
    /**
     *  @brief  Generate a tree with random shape, that is to say, randomly
     * chose from the methods above.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self random_shaped_tree(int size) {
        int idx = rnd.next(6);
        if (idx == 0)
            sqrt_height_tree(size);
        if (idx == 1)
            log_height_tree(size);
        if (idx == 2)
            chain(size);
        if (idx == 3)
            flower(size);
        if (idx == 4)
            n_deg_tree(size);
        if (idx == 5) {
            double cp = rnd.wnext(1.0, 2), fp = rnd.next(1.0 - cp);
            chain_and_flower(size, cp, fp);
        }
        return *this;
    }
    /**
     *  @brief  Output the generated edges to stdout. NOTE that n will not be
     * printed.
     *  @param  weights the weights of the edges. Input weights[i] as the weight
     * of the edge [fa[i], i].
     *  @param  shuffled if I should print it in random order.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self print(int shuffled,
                       std::vector<int> weights = std::vector<int>{}) {
        bool output_weight = true;
        if (weights.size() == 0)
            output_weight = false;
        else if (int(weights.size()) != n + 1)
            Quit("Invalid weights.size(): ", weights.size());
        std::vector<int> order(n + 1);
        std::iota(order.begin(), order.end(), 0);
        if (shuffled)
            order = _rnd.shuffle(order);
        for (int i = 2; i <= n; i++)
            if (output_weight)
                println(order.at(i), fa.at(order.at(i)), weights.at(i));
            else
                println(order.at(i), fa.at(order.at(i)));
        return *this;
    }
    /**
     *  @brief  Output the generated `fa` array to stdout. NOTE that n will not
     * be printed.
     *  @param sep The seperator.
     *  @param end the end char.
     *  @param  shuffled if I should print it in random order.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline _Self print_fa(char sep = ' ', char end = '\n') {
        for (int i = 2; i <= n; i++)
            std::cout << fa[i] << sep;
        std::cout << end;
        return *this;
    }
    /**
     *  @brief  Get the leave nodes of the current tree.
     *  @return The leaves.
     *  @throw  It throws what `std::vector<int>` throws.
     */
    inline std::vector<int> get_leaves() {
        std::vector<int> is_leave(n + 1, 1);
        for (int i = 1; i <= n; i++)
            is_leave.at(fa.at(i)) = 0;
        leaves.clear();
        for (int i = 1; i <= n; i++)
            if (is_leave.at(i))
                leaves.push_back(i);
        return leaves;
    }
};

/**
 *  @brief  Class that used to generate an array.
 */
template <typename _Tp>
class Array {
   public:
    using _Sequence = std::vector<_Tp>;
    using _Self = Array<_Tp>;
    int n;            ///< size of the array that generated.
    _Sequence array;  ///< The container of the elements.
    /**
     *  @brief  return the reference of the size-th element in this array.
     *  @param  idx the index of the element you requested.
     *  @return The reference of the element.
     *  @throw  out_of_range if idx is an invalid index.
     */
    inline _Tp& operator[](int idx) {
        ensure(idx <= n);
        return array.at(idx);
    }
    /**
     *  @brief  return the reference of the first element in this array.
     *  @return The reference of the element.
     *  @throw  It throws what the _Sequence throws.
     */
    inline auto begin() { return array.begin(); }
    /**
     *  @brief  return the reference of the last element in this array.
     *  @return The reference of the element.
     *  @throw  It throws what the _Sequence throws.
     */
    inline auto end() { return array.end(); }
    /**
     *  @brief  init the whole array with the size of `size`.
     *  @param  size how large this array should be.
     *  @return no return.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void init(int size) {
        if (size < 1)
            throw GenException(
                format("Invalid 'n' has been passed in `init`: %d", size));
        array.clear();
        n = size, array.resize(n + 1);
    }
    /**
     *  @brief  Output the current array.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void print(char sep = ' ', char end = '\n') {
        for (int i = 1; i <= n; i++)
            std::cout << array.at(i) << sep;
        std::cout << end;
    }
    /**
     *  @brief  Get the sum of the elements.
     *  @return The sum of the elements.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Tp sum() {
        _Tp current_sum = 0;
        for (auto i : array)
            current_sum += i;
        return current_sum;
    }
    /**
     *  @brief  Generate an array with size `size`, while its elements are
     * values in [wl, wr].
     *  @param  size how large this array should be.
     *  @param  wl the sub of the elements.
     *  @param  wr the sup of the elements.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self basic_gen(int size, _Tp wl, _Tp wr) {
        init(size);
        for (int i = 1; i <= size; i++)
            array.at(i) = rnd.next(wl, wr);
        return *this;
    }
    /**
     *  @brief  Sort the current array.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void sort() { std::sort(array.begin(), array.end()); }
    /**
     *  @brief  Shuffle the current array.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void shuffle() { _rnd.shuffle(array); }
    /**
     *  @brief  Reverse the current array.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void reverse() { std::reverse(array.begin(), array.end()); }
    /**
     *  @brief  Turn this array into the Diffrence array of it.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self to_diffrence() {
        for (int i = n; i >= 1; i--)
            array.at(i) -= array.at(i - 1);
        return *this;
    }
    /**
     *  @brief  Generate an array with size `size`, while its elements are 0
     * or 1.
     *  @param  size how large this array should be.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self binary_gen(int size) { return basic_gen(size, 0, 1); }
    /**
     *  @brief  Generate an array with size `size`, while its elements are not
     * decreasing.
     *  @param  size how large this array should be.
     *  @param  wl the sub of the elements.
     *  @param  wr the sup of the elements.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self ascending_array(int size, _Tp wl, _Tp wr) {
        init(size);
        basic_gen(size, wl, wr);
        (*this).sort(), (*this).shuffle();
        return *this;
    }
    /**
     *  @brief  Generate an array with size `size`, while its elements are not
     * increasing.
     *  @param  size how large this array should be.
     *  @param  wl the sub of the elements.
     *  @param  wr the sup of the elements.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self decending_array(int size, _Tp wl, _Tp wr) {
        ascending_array(size, wl, wr);
        reverse();
        return *this;
    }
    /**
     *  @brief  Generate an array with size `size`, while the sum of its
     * elements are a constant.
     *  @param  size how large this array should be.
     *  @param  sum the sum of the elements.
     *  @param  AcceptZero if the array can contain zero or not.
     *  @param  AcceptNegative if the array can contain negative values or not.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self constant_sum(int size,
                              _Tp sum,
                              bool AcceptZero = true,
                              bool AcceptNegative = true) {
        init(size);
        if (AcceptZero)
            basic_gen(size - 1, 0, sum), array.at(size) = sum;
        else
            sum > n ? basic_gen(size - 1, 0, sum - n)
                    : basic_gen(size - 1, size - n, 0),
                array.at(size) = sum - n;
        (AcceptNegative ? void(nullptr) : sort()), to_diffrence();
        if (!AcceptZero) {
            for (int i = 1; i <= size; i++)
                array.at(i)++;
        }
        return *this;
    }
    /**
     *  @brief  Perturbe the current array, keeping the sum of the elements
     * still.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self perturbe() {
        Array<_Tp> tmp_arr;
        tmp_arr.constant_sum(n, 0);
        for (int i = 1; i <= n; i++)
            array.at(i) += tmp_arr.at(i);
        return *this;
    }
    /**
     *  @brief  Generate a permutation of 1 to size.
     *  @param  size the size of the array.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self permutation(int size) {
        init(size);
        array = rnd.perm(size, 1);
        return *this;
    }
    /**
     *  @brief  Generate an array with the i-th element is f(i + begin).
     *  @param  size the size of the array.
     *  @param  GenerateFunction the GenerateFunction of the array.
     *  @param  begin the begin point of the array.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self generate_function(int size,
                                   int (*GenerateFunction)(int),
                                   int begin = 1) {
        init(size);
        for (int i = 0; i < size; i++)
            array.at(i + 1) = GenerateFunction(i + begin);
        return *this;
    }
    /**
     *  @brief  Generate an array with the i-th element is f(a_{i-1}).
     *  @param  size the size of the array.
     *  @param  GenerateFunction the GenerateFunction of the array.
     *  @param  begin the begin value of the array.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self generate_iterate_function(int size,
                                           int (*IterateFunction)(int),
                                           int begin = 1) {
        init(size);
        array.at(0) = begin;
        for (int i = 1; i < size; i++)
            array.at(i) = IterateFunction(array.at(i - 1));
        return *this;
    }
    inline _Self randomly_insert(int size, int num) {
        while (size-- > 0)
            array[rnd.next(1, n)] = num;
        return *this;
    }
};

class Graph {
   public:
    using _Self = Graph;
    int n;                ///< The count of points in the graph.
    int m;                ///< The count of edges in the graph.
    bool directed;        ///< Denoting if the **edges** are directed or not.
    std::set<pii> edges;  ///< The container of edges.

    Graph() {}
    Graph(Tree tr, bool direction = 0) {
        n = tr.n, m = n - 1;
        if (direction) {
            for (int i = 2; i <= n; i++)
                edges.insert({i, tr.fa.at(i)});
        } else {
            for (int i = 2; i <= n; i++)
                edges.insert({tr.fa.at(i), i});
        }
    }
    /**
     *  @brief  Add a graph to the current graph.
     *  @param  rhs the graph to be added.
     *  @return The graph itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline _Self add(Graph rhs) {
        int offset = n;
        n += rhs.n, m += rhs.m;
        for (pii edge : rhs.edges)
            edges.insert({offset + edge.first, offset + edge.second});
        return *this;
    }
    inline Graph operator+(Graph rhs) {
        Graph g = *this;
        return g.add(rhs);
    }
    inline _Self operator+=(Graph rhs) { return add(rhs); }
    /**
     *  @brief  init the whole graph with the size of `size`, and direct
     * `directed_graph`.
     *  @param  size how large this graph should be.
     *  @param  directed_graph is this graph directed or not.
     *  @return no return.
     *  @throw  It throws what the std::set throws.
     */
    void init(int size, bool directed_graph) {
        if (size < 1)
            throw GenException(
                format("Invalid 'n' has been passed in `init`: %d", size));
        n = size;
        edges.clear();
        directed = directed_graph;
    }
    /**
     *  @brief  To check out if the edge exists or not.
     *  @param  u, v the point number of the edge that is being checked.
     *  @return if the edge exists or not.
     *  @throw  It throws what the std::set throws.
     */
    inline bool exists(int u, int v) {
        if (edges.count({u, v}))
            return true;
        if (!directed)
            return edges.count({v, u});
    }
    /**
     *  @brief  Generate a graph completely random.
     *  @param  size how large this graph should be.
     *  @param  edges_count the count of the edges.
     *  @param  directed_graph is this graph directed or not.
     *  @return no return.
     *  @throw  It throws what the std::set throws.
     */
    inline _Self randomly_gen(int size,
                              int edges_count,
                              bool directed_graph = false) {
        m = edges_count;
        init(size, directed_graph);
        for (int i = 1; i <= edges_count; i++) {
            int u = rnd.next(1, size), v = rnd.next(1, size);
            if (!exists(u, v))
                edges.insert({u, v});
            else
                i--;
        }
        return *this;
    }
    /**
     *  @brief  Generate a DAG.
     *  @param  size how large this graph should be.
     *  @param  edges_count the count of the edges.
     *  @param  directed_graph is this graph directed or not.
     *  @return no return.
     *  @throw  It throws what the std::set throws.
     */
    inline _Self DAG(int size, int edges_count, bool ensure_connected = true) {
        m = edges_count;
        std::vector<int> a(size + 1);
        std::iota(a.begin(), a.end(), 0);
        _rnd.shuffle(a);
        if (ensure_connected) {
            ensure(edges_count >= size - 1);
            Tree tree;
            tree.random_shaped_tree(size);
            auto Sort = [](pii a) {
                return a.first > a.second ? std::make_pair(a.second, a.first)
                                          : a;
            };
            for (int i = 2; i <= size; i++)
                edges.insert(Sort({a[tree.fa.at(i)], a[i]}));
            edges_count -= (size - 1);
        }
        for (int i = 1; i <= edges_count; i++) {
            int u = rnd.next(size) + 1, v = u + rnd.next(size - u) + 1;
            if (u == v || exists(a[u], a[v]))
                i--;
            else
                edges.insert({a[u], a[v]});
        }
        return *this;
    }
    /**
     *  @brief  Generate a forest.
     *  @param  size how large this graph should be.
     *  @param  cnt the count of the trees.
     *  @return no return.
     *  @throw  It throws what the std::set throws.
     */

    inline _Self forest(int size, int cnt = -1) {
        cnt = ~cnt ? cnt : rnd.next(1, std::min(std::max(n / 1000, 10), size));
        Tree tr;
        tr.random_shaped_tree(size);
        *this = tr;
        Array<int> arr;
        arr.permutation(size).shuffle();
        for (int i = 1; i <= cnt; i++)
            edges.erase({tr.fa.at(arr[i]), arr[i]});
        return *this;
    }
    /**
     *  @brief  Generate a graph, on which spfa works so slow.
     *  @param  size how large this graph should be.
     *  @param  size how large this graph should be.
     *  @param  edges_count the count of the edges.
     *  @return no return.
     *  @throw  It throws what the std::set throws.
     */
    inline void hack_spfa(int size, int edges_count) {
        int sz = sqrt(size);
        m = edges_count;
        for (int i = 1; i <= sz; i++) {
            for (int j = 1; j <= sz; j++) {
                if (i != 1 && edges_count)
                    edges.insert({(i - 1) * sz + j, i * sz + j}), edges_count--;
                if (j != 1 && edges_count)
                    edges.insert({i * sz + j - 1, i * sz + j}), edges_count--;
            }
        }
        for (int i = 1; i <= edges_count; i++) {
            int u = rnd.next(size) + 1, v = u + rnd.next(n - u + 1) + 1;
            if (u == v || exists(u, v))
                i--;
            else
                edges.insert({u, v});
        }
    }
};

class String {
   public:
    std::string str;  ///< The container of the string.

    inline String operator+(String s) { return String{str + s.str}; }
    inline String operator+=(String s) { return *this = String{str + s.str}; }
    /**
     *  @brief  Output the generated string to stdout. NOTE that n will not be
     * printed.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline void print() { println(str); }
    /**
     *  @brief  return the reference of the size-th element in this string. NOTE
     * that this is 1-indexed.
     *  @param  idx the index of the element you requested.
     *  @return The reference of the element.
     *  @throw  out_of_range if idx is an invalid index.
     */
    inline char& operator[](int idx) { return str[idx - 1]; }
    /**
     *  @brief  Generate a string with format(pattern, ...t).
     *  @param  pattern the pattern of the format-string,
     *  @param  ...t the formated value.
     *  @return The generated string.
     */
    template <typename... Args>
    inline std::string gen(const char* pattern, Args... t) {
        return str = rnd.next(format(pattern, t...));
    }
    /**
     *  @brief  Generate a string contains lower letters only.
     *  @param  size the length of the generated string.
     *  @return The generated string.
     */
    inline std::string lower(int size) { return gen("[a-z]{%d}", size); }
    /**
     *  @brief  Generate a string contains latin letters only.
     *  @param  size the length of the generated string.
     *  @return The generated string.
     */
    inline std::string latin(int size) { return gen("[a-zA-Z]{%d}", size); }
    /**
     *  @brief  Generate a string contains latin letters and numbers only.
     *  @param  size the length of the generated string.
     *  @return The generated string.
     */
    inline std::string latin_number(int size) {
        return gen("[a-zA-Z0-9]{%d}", size);
    }
    /**
     *  @brief  Generate a string contains numbers only.
     *  @param  size the length of the generated string.
     *  @param  leading_zero if the string can start with zero or not.
     *  @return The generated string.
     */
    inline std::string numbers_only(int size, bool leading_zero = false) {
        if (leading_zero)
            gen("[0-9]{%d}", size);
        else
            gen("[1-9][0-9]{%d}", size - 1);
        return str;
    }
    /**
     *  @brief  Set the generated string size times of it.
     *  @param  size the times it should be repeated.
     *  @return The generated string.
     */
    inline std::string repeat(int size) {
        std::string res;
        for (int i = 1; i <= size; i++)
            res += str;
        return str = res;
    }
    /**
     *  @brief  Generate multi-string using the same function, and concentrate
     * them.
     *  @param  func a function which accepts a integer(the size of the
     * sub-string) and returns a std::string.
     *  @param  size a function which accepts a integer(the number of the
     * sub-string) and returns a integer, which is the size of the string.
     *  @param  times the count of the string that should be generated.
     *  @param  sep the seperator between the strings.
     *  @return The generated string.
     */
    inline std::string gen_multi(std::string (*func)(int),
                                 int (*size)(),
                                 int times,
                                 std::string sep = " ") {
        std::string res;
        for (int i = 1; i <= times; i++)
            res += func(size()), i != times ? res += sep : sep;
        return str = res;
    }
    /**
     *  @brief  Randomly replace some charactor in the generated string, using
     * `rep`.
     *  @param  size the count of char that should be replaced.
     *  @param  rep the char used to replace the origin.
     *  @return The generated string.
     */
    inline std::string random_insert(int size, char rep) {
        Array<int> array;
        array.ascending_array(size, 0, str.length() - 1);
        for (int i : array)
            str[i] = rep;
        return str;
    }
};

/**
 *  @brief  The struct that represent a point on a 2D plane.
 *  @param  PointType the value type of the Point.
 */
template <typename PointType>
struct Point {
    PointType x, y;
    bool operator==(const Point& rhs) const {
        return (x - rhs.x) <= 15 * eps && (y - rhs.y) <= 15 * eps;
    }
};

/**
 *  @brief  The fraction in C++.
 */
struct cFrac {
    i64_ll a, b;
    cFrac(i64_ll a) : a(a), b(1) {}
    cFrac(i64_ll a, i64_ll b) : a(a), b(b) {}
    cFrac(std::string str) { sscanf(str.data(), "%lld/%lld", &a, &b); }
    inline cFrac reduce() {
        i64_ll gcd = std::__gcd(a, b);
        a /= gcd, b /= gcd;
        return *this;
    }
    inline cFrac operator+(const cFrac& rhs) {
        return cFrac(a * rhs.b + b * rhs.a, b * rhs.b).reduce();
    }
    inline cFrac operator-(const cFrac& rhs) {
        return cFrac(a * rhs.b - b * rhs.a, b * rhs.b).reduce();
    }
    inline cFrac operator*(const cFrac& rhs) {
        return cFrac(a * rhs.a, b * rhs.b).reduce();
    }
    inline cFrac operator/(const cFrac& rhs) {
        return cFrac(a * rhs.b, b * rhs.a).reduce();
    }
    inline bool operator<=(const cFrac& rhs) { return a * rhs.b <= b * rhs.a; }
    inline bool operator>=(const cFrac& rhs) { return a * rhs.b >= b * rhs.a; }
    inline bool operator<(const cFrac& rhs) { return a * rhs.b < b * rhs.a; }
    inline bool operator>(const cFrac& rhs) { return a * rhs.b > b * rhs.a; }
    inline bool operator==(const cFrac& rhs) { return a * rhs.b == b * rhs.a; }
    inline bool operator!=(const cFrac& rhs) { return a * rhs.b == b * rhs.a; }
    inline std::ostream& operator<<(std::ostream& os) {
        if (a < 0)
            os << '-';
        os << a << '/' << b;
        return os;
    }
    inline std::istream& operator>>(std::istream& is) {
        std::string str;
        is >> str;
        *this = str;
        return is;
    }
    inline cFrac rand(pll b_range, double wl, double wr) {
        b = rnd.next(b_range.first, b_range.second);
        a = rnd.next(b * wl, b * wr);
        return *this;
    }
};

template <typename PointType>
class Geometry {
   public:
    using _Tp = Point<PointType>;
    int n;
    std::set<_Tp> points;
    inline void init() { points.clear(); }
    /**
     *  @brief  Randomly generate some points in the rectangle the leftbottom and rightup determine.
     *  @param  size the count of points that should be generated.
     *  @param  leftbottom, rightup the range of the points.
     */
    inline void randomize_points(int size, _Tp leftbottom, _Tp rightup) {
        init();
        for (int i = 1; i <= size; i++) {
            PointType x = rnd.next(leftbottom.x, rightup.x),
                      y = rnd.next(leftbottom.y, rightup.y);
            if (points.find({x, y}) != points.end())
                i--;
            else
                points.insert({x, y});
        }
    }
    inline void warning(int size) {
        if (size > 100) {
            if (size > 10000)
                warn(
                    "You are trying to generate a convex shell with size %d, "
                    "which is a big one that its coordinate may be over "
                    "2^{31}-1 that occurs signed-integer-overflow.",
                    size);
            else
                warn(
                    "You are trying to generate a convex shell with size %d, "
                    "which is a big one that its angle may too close to pi.",
                    size);
        }
    }
    inline void make_raw_convex_shell(int size) { warning(size); }
    inline void make_convex(int size) { warning(size); }
};
