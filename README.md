# The Longest Common Subsequence Problem for Small Alphabets in the Word RAM Model

This repository provides a sample implementation of the longest common subsequence (LCS) algorithms described in the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM" by Rodrigo Castro. The following files are provided.

* `generator.cpp`: A C++ program that generates a pair of random strings of a certain length and alphabet size, for which their longest common subsequence is expected to be a given percentage of their length. These strings are generated following the ideas of [Rick][1] .
* `run_generator.php`: A PHP program that invokes the generator several times using distinct configuration parameters.
* `algorithm.h`: A C++ header file that implements a predicate-based doubling search, also called double binary or galloping search.
* `bit.h`: A C++ header file that implements a couple of convenience functions related to bitsets, and also implements an arbitrary $W$-bit permutation operation in $O(1)$ using AVX-512 intrinsics.
* `string_preprocessing.h`: A C++ header file that implements a collection of data structures used during the preprocessing phase of several LCS algorithms:
  - `closest_occurrence`: Implements the *closest* data structure. Given a string $A$ of length $n$, constructing the data structure takes $O(ns)$ time, where $s = |\Sigma|$. Once constructed, an $O(1)$ time query returns the index of the closest occurrence of a symbol from $A_i$ onwards.
  - `next_distinct`: Implements the *distinct* data structure. Constructing the data structure takes $O(ns)$ time. Once constructed, an $O(1)$ time query returns a list of the distinct symbols from $A_i$ onwards, together with their indices and the prefix set union of such symbols.
  - `order_distinct`: Implements the *order* data structure. Constructing the data structure takes $O(ns)$ time. Once constructed, an $O(1)$ time query returns a list of the distinct symbols from $A_i$ onwards.
  - `sparse_table`: Implements a sparse table that computes the bitset union operation. It is a weaker data structure than the one described in [Optimal Preprocessing for Answering On-Line Product Queries][2] as it only handles idempotent operations, but it is [widely known in competitive programming][3] and it is very easy to implement. Constructing the data structure takes $O(n \log n)$ time. Once constructed, an $O(1)$ time query returns a bitset representing the set union of symbols from $A$ in the queried range.
  - `sparse_tree`: Implements a "summary based" or "block hybrid" [recursive variation][4] of the previous data structure that may be constructed in $O(n \log^* n)$ while retaining $O(1)$ time queries.
* `lcs_wagner_fischer.h`: A C++ header file that implements the textbook dynamic programming algorithm due to Wagner and Fischer for computing the LCS. The `lcs_wf` function implements this algorithm.
* `lcs_chin_poon.h`: A C++ header file that provides a generic implementation of the contour based algorithm due to Chin and Poon for computing the LCS of two strings. Such implementation decouples the main contour based strategy (the `chin_poon_main` function) from the implementation of the *topmost* subroutine. An implementation of the original *topmost* subroutine from Chin and Poon is provided as `chin_poon_topmost`, and it takes $O(\min(s, \min(i, i_2) - i_1))$ time, where $i$ is the smallest index of a symbol from $A[i_1]$ to $A[i_2]$ that matches a symbol from a given range of a string $B$. The `lcs_cp` function simply calls `chin_poon_main` with `chin_poon_topmost`.
* `lcs_rcc.h`: A C++ header file that provides three implementations of the *topmost* subroutine by Rodrigo Castro and uses them with the `chin_poon_main` contour based algorithm. The `lcs_rcc0`, `lcs_rcc1` and `lcs_rcc2` functions simply call `chin_poon_main` with the corresponding *topmost* implementation.
  - `rcc0_topmost`: Implements an unpublished $O(\log s)$ algorithm for *topmost*. It is simple, but asymptotically not always better than the one from Chin and Poon, and worse than the following methods.
  - `rcc1_topmost`: Implements the first contribution from the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM". It implements *topmost* in $O(\log \min(s, \min(i, i_2) - i_1))$ time.
  - `rcc2_topmost`: Implements the second contribution from the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM". It implements *topmost* in $O(1)$ time when $s \leq 64$. It requires a CPU with the *AVX-512 Bit Algorithms* instruction set that was introduced in the Intel Icelake architecture.
* `main.cpp`: A C++ program that computes the length of the LCS of two strings. The strings are read from files whose paths are provided via `argv`. It computes the LCS using the algorithms from Wagner and Fischer, Chin and Poon, and Castro.
* `run_tests.php`: A PHP program that invokes the main C++ LCS program against string pairs that were created by the generator.
* `run_tests.php_emulator`: A PHP program that uses the *Intel® Software Development Emulator* to invoke the main C++ LCS program against string pairs that were created by the generator. See below.

The C++ programs may be compiled with G++ 14.2 using the `-std=c++23 -O3 -march=icelake-client` flags. The PHP programs may be run under PHP 8.2. The provided algorithms were not optimized for microbenchmarking purposes but for readability, in order to easily determine their asymptotic complexities.

For CPUs that do not support the *AVX-512 Bit Algorithms* instruction set, an alternative is to use the *Intel® Software Development Emulator*. For example, under Linux you can install and run it as follows:

```
wget https://downloadmirror.intel.com/843185/sde-external-9.48.0-2024-11-25-lin.tar.xz
tar -xvf sde-external-9.48.0-2024-11-25-lin.tar.xz
mv sde-external-9.48.0-2024-11-25-lin /usr/bin/sde
/usr/bin/sde/sde -icl -- ./main filepath1 filepath2
```

  [1]: https://www.semanticscholar.org/paper/New-Algorithms-for-the-Longest-Common-Subsequence-Rick/28568a5d078ee484a30ef1b261a560612f5fc999
  [2]: https://arxiv.org/abs/2406.06321
  [3]: https://cp-algorithms.com/data_structures/sparse-table.html
  [4]: https://activities.tjhsst.edu/sct/lectures/1920/2020_4_30_RMQ.pdf
