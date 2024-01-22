#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include "testlib.h"

using pii = std::pair<int, int>;
using i64_ll = long long;
using i128_ll = __int128_t;

double eps = 1e-12;

template <typename... Args>
inline void Quit(Args... params) {
    ((std::cout << params << ' '), ...);
    std::cout << "\n";
    exit(1);
}

inline pii Sort(pii a) {
    return a.first > a.second ? std::make_pair(a.second, a.first) : a;
}

inline i64_ll qpow(i64_ll a, i64_ll b, i64_ll mod) {
    assert(b >= 0);
    i64_ll ans = 1;
    while (b) {
        if (b & 1)
            ans = 1ll * ans * a % mod;
        a = 1ll * a * a % mod;
        b >>= 1;
    }
    return ans;
}

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

class _random {
   public:
    template <typename _Tp>
    inline _Tp choice(std::vector<_Tp> array, int l = 1, int r = -1) {
        if (!~r)
            r = array.size();
        return array.at(rnd.next(l, r));
    }
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

#define warn printf

class Tree {
   public:
    int n;
    std::vector<int> fa, leaves;
    bool weighted = false;
using _Self = Tree;
    /**
     *  @brief  Initiate Tree object with size `size`.
     *  @param  size The count of the nodes that will be generated.
     *  @return The graph itself.
     *  @throw  out_of_range if @a size is an invalid node count, e.g. -1.
     */
    inline void init(int size) {
        fa.clear(), leaves.clear();
        if (n < 1)
            throw format("Invalid 'n' has been passed in `init`: %d", size);
        n = size;
        fa.resize(size + 1);
    }
    /**
     *  @brief  Generate a tree with an expected height of $O(\sqrt n)$
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
     *  @brief  Generate a tree with an expected height of O(\log n)
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
     *  @brief  Generate a tree with an expected max_deg of O(n)
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
                fa.at(i) = _rnd.choice(nodes);
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
     *  @brief  Output the generated tree to stdout. NOTE that n will not be
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
     *  @brief  Get the leave nodes of the current tree.
     *  @param  no params.
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

template <typename _Tp>
class Array {
   public:
    using _Sequence = std::vector<_Tp>;
    using _Self = Array<_Tp>;
    int n;
    _Sequence array;
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
     *  @param  no params.
     *  @return The reference of the element.
     *  @throw  It throws what the _Sequence throws.
     */
    inline auto begin() { return array.begin(); }
    /**
     *  @brief  return the reference of the last element in this array.
     *  @param  no params.
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
        array.clear();
        n = size, array.resize(n + 1);
    }
    /**
     *  @brief  Output the current array.
     *  @param  no params.
     *  @return The array itself.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void print(char sep = ' ') {
        for (int i = 1; i <= n; i++)
            std::cout << array.at(i) << sep;
    }
    /**
     *  @brief  Get the sum of the elements.
     *  @param  no params.
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
     *  @param  no params.
     *  @return no return.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void sort() { std::sort(array.begin(), array.end()); }
    /**
     *  @brief  Shuffle the current array.
     *  @param  no params.
     *  @return no return.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void shuffle() { _rnd.shuffle(array); }
    /**
     *  @brief  Reverse the current array.
     *  @param  no params.
     *  @return no return.
     *  @throw  It throws what the _Sequence throws.
     */
    inline void reverse() { std::reverse(array.begin(), array.end()); }
    /**
     *  @brief  Turn this array into the Diffrence array of it.
     *  @param  no params.
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
     *  @param  no params.
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
            array.at(i) = GenerateFunction(i + begin);
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
};

class Graph {
   public:
    using _Self = Graph;
    int n, m;
    bool directed;
    std::set<pii> edges;

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
    std::string str;
    template <typename... Args>
    inline std::string gen(const char* pattern, Args... t) {
        return str = rnd.next(format(pattern, t...));
    }
    inline std::string lowerletter(int size) { return gen("[a-z]{%d}", size); }
    inline std::string latinletter(int size) {
        return gen("[a-zA-Z]{%d}", size);
    }
    inline std::string latin_and_number(int size) {
        return gen("[a-zA-Z0-9]{%d}", size);
    }
    inline std::string numbers_only(int size, bool leading_zero = false) {
        if (leading_zero)
            gen("[0-9]{%d}", size);
        else
            gen("[1-9][0-9]{%d}", size - 1);
        return str;
    }
    inline std::string repeat(int size) {
        std::string res;
        for (int i = 1; i <= size; i++)
            res += str;
        return str = res;
    }
    inline std::string gen_multi(std::string (*func)(int),
                                 int (*size)(),
                                 int times,
                                 std::string sep = " ") {
        std::string res;
        for (int i = 1; i <= times; i++)
            res += func(size()), i != times ? res += sep : sep;
        return str = res;
    }
    inline std::string random_insert(int size, char rep) {
        Array<int> array;
        array.ascending_array(size, 0, str.length() - 1);
        for (int i : array)
            str[i] = rep;
        return str;
    }
};

template <typename PointType>
struct Point {
    PointType x, y;
    bool operator==(const Point& rhs) const {
        return (x - rhs.x) <= 15 * eps && (y - rhs.y) <= 15 * eps;
    }
};

template <typename PointType>
class Geometry {
   public:
    using _Tp = Point<PointType>;
    int n;
    std::set<_Tp> points;
    inline void init() { points.clear(); }
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
    inline void make_raw_convex_shell(int size) {
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
};
