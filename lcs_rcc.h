#ifndef LCS_RCC_H
#define LCS_RCC_H

#include "algorithm.h"
#include "bit.h"
#include "lcs_chin_poon.h"
#include "string_preprocessing.h"
#include <algorithm>
#include <bit>
#include <utility>
#include <vector>
#include <stdint.h>

template<int W>
class rcc0_topmost {
   next_distinct<W> distinct_a, distinct_b;

public:
   rcc0_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distinct_a(a), distinct_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int af, int bi, int bf) const {
      if (ai != af && bi != bf) {
         const auto& list_a = distinct_a.query(ai);
         const auto& list_b = distinct_b.query(bi);
         auto tb = std::partition_point(list_b.begin( ), list_b.end( ), [&](const auto& entry) {
            return entry.index < bf;
         });
         auto ta = std::partition_point(list_a.begin( ), list_a.end( ), [&](const auto& entry) {
            return (entry.set & std::prev(tb)->set) == 0;
         });
         if (ta == list_a.end( ) || ta->index >= af) {
            return { af, bf };
         } else {
            return { ta->index, std::partition_point(list_b.begin( ), list_b.end( ), [&](const auto& entry) {
               return !entry.set[ta->symbol];
            })->index };
         }
      }
      return { af, bf };
   }
};

template<int W>
class rcc1_topmost {
   next_distinct<W> distinct_a;
   sparse_tree<W> sparse_b;
   closest_occurrence<W> closest_b;

public:
   rcc1_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distinct_a(a), sparse_b(b), closest_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int af, int bi, int bf) const {
      if (ai != af && bi != bf) {
         const auto& list_a = distinct_a.query(ai);
         auto chars_b = sparse_b.query(bi, bf);
         auto ta = doubling_search(list_a.begin( ), list_a.end( ), [&](const auto& entry) {
            return entry.index >= af || (entry.set & chars_b) != 0;
         });
         if (ta != list_a.end( ) && ta->index < af) {
            return { ta->index, closest_b.query(bi, ta->symbol) };
         }
      }
      return { af, bf };
   }
};

template<int W>
class rcc2_topmost {
   sparse_tree<W> sparse_a, sparse_b;
   order_distinct<W> order_a;
   closest_occurrence<W> closest_a, closest_b;

public:
   rcc2_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : sparse_a(a), sparse_b(b), order_a(a), closest_a(a), closest_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int af, int bi, int bf) const {
      if (ai != af && bi != bf) {
         auto common_chars = sparse_a.query(ai, af) & sparse_b.query(bi, bf);
         if (common_chars.any( )) {
            const auto& list_a = order_a.query(ai);
            auto first_a = std::countr_zero(bit_permute(common_chars, list_a).to_ullong( ));
            return { closest_a.query(ai, list_a[first_a]), closest_b.query(bi, list_a[first_a]) };
         }
      }
      return { af, bf };
   }
};

template<int W>
int lcs_rcc0(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, rcc0_topmost<W>(a, b));
}

template<int W>
int lcs_rcc1(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, rcc1_topmost<W>(a, b));
}

template<int W>
int lcs_rcc2(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, rcc2_topmost<W>(a, b));
}

#endif
