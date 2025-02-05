# The Longest Common Subsequence Problem for Small Alphabets in the Word RAM Model

This repository provides a sample implementation of the Longest Common Subsequence (LCS) algorithms described in the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM" by Rodrigo Castro. The following files are provided.

* `generator.cpp`: A C++ program that generates a set of random strings of a certain length and alphabet size, following either a linear or binomial distribution. The generated strings are then stored as files inside a directory.
* `run_generator.php`: A PHP program that invokes the generator several times using distinct configuration parameters.
* `algorithm.h`: A C++ header file that implements a predicate-based doubling search, also called double binary or galloping search.
* `bit.h`: A C++ header file that implements a couple of convenience functions related to bitsets, and also implements an arbitrary $W$-bit permutation operation in $O(1)$ using AVX-512 intrinsics.
* `string_preprocessing.h`: A C++ header file that implements a collection of data structures used during the preprocessing phase of several LCS algorithms:
 - `closest_occurrence`: Implements the *closest* data structure. Given a string $A$ of length $n$, constructing the data structure takes $O(ns)$ time, where $s = |\Sigma|$. Once constructed, an $O(1)$ time query returns the index of the closest occurrence of a symbol from $A_i$ onwards.
 - `next_distinct`: Implements the *distinct* data structure. Constructing the data structure takes $O(ns)$ time. Once constructed, an $O(1)$ time query returns a list of the distinct symbols from $A_i$ onwards, together with their indices and the prefix set union of such symbols.
 - `order_distinct`: Implements the *order* data structure. Constructing the data structure takes $O(ns)$ time. Once constructed, an $O(1)$ time query returns a list of the distinct symbols from $A_i$ onwards.
 - `sparse_table`: Implements a sparse table that computes the bitset union operation. It is a weaker data structure than the one described in [Optimal Preprocessing for Answering On-Line Product Queries][1] as it only handles idempotent operations, but it is [widely known in competitive programming][2] and it is very easy to implement. Constructing the data structure takes $O(n \log n)$ time. Once constructed, an $O(1)$ time query returns a bitset representing the set union of symbols from $A$ in the queried range.
 - `sparse_tree`: Implements a "summary based" or "block hybrid" [recursive variation][3] of the previous data structure that may be constructed in $O(n \log^* n)$ while retaining $O(1)$ time queries.
* `lcs_wagner_fischer.h`: A C++ header file that implements the textbook dynamic programming algorithm due to Wagner and Fischer for computing the LCS.
* `lcs_chin_poon.h`: A C++ header file that provides a generic implementation of the contour based algorithm due to Chin and Poon for computing the LCS of two strings. Such implementation decouples the main contour based strategy (the `chin_poon_main` function) from the implementation of the *topmost* subroutine. An implementation of the original *topmost* subroutine from Chin and Poon is provided as `chin_poon_topmost`, and it takes $O(\min(s, \min(i, i_2) - i_1))$ time, where $i$ is the index of the topmost match and $[i_1, i_2]$ is the search interval.
* `lcs_rcc.h`: A C++ header file that provides three implementations of the *topmost* subroutine by Rodrigo Castro and uses them with the `chin_poon_main` contour based algorithm.
 - `rcc0_topmost`: Implements an unpublished $O(\log s)$ algorithm for *topmost*. It is simple, but not necessarily better than the one from Chin and Poon, and asymptotically worse than the following methods.
 - `rcc1_topmost`: Implements the first contribution from the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM". It implements *topmost* in $O(\log \min(s, \min(i, i_2) - i_1))$ time.
 - `rcc2_topmost`: Implements the second contribution from the paper "The Longest Common Subsequence Problem for Small Alphabets in the Word RAM". It implements *topmost* in $O(1)$ time when $s \leq 64$.
* `main.cpp`: A C++ program that computes the length of the LCS of two strings. The strings are read from files whose paths are provided via `argv`. It computes the LCS using the algorithms from Wagner and Fischer, Chin and Poon, and Castro.
* `run_tests.php`: A PHP program that invokes the main C++ LCS program against some string pairs that were created by the generator.

The C++ programs may be compiled with G++ 14.2 using the `-O3 -std=c++23 -march=native` flags. The PHP programs may be run under PHP 8.2. There are no additional dependencies.

  [1]: https://arxiv.org/abs/2406.06321
  [2]: https://cp-algorithms.com/data_structures/sparse-table.html
  [3]: https://activities.tjhsst.edu/sct/lectures/1920/2020_4_30_RMQ.pdf
