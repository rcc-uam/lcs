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
#include <stddef.h>
#include <stdint.h>

template<size_t W>
class rcc0_topmost {
   distinct_with_prefix<W> distpref_a, distpref_b;

public:
   rcc0_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distpref_a(a), distpref_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int aj, int bi, int bj) const {
      if (ai != aj && bi != bj) {
         const auto& list_a = distpref_a.query(ai);
         const auto& list_b = distpref_b.query(bi);
         auto tb = std::partition_point(list_b.begin( ), list_b.end( ), [&](const auto& entry) {
            return entry.index < bj;
         });
         auto ta = std::partition_point(list_a.begin( ), list_a.end( ), [&](const auto& entry) {
            return (entry.seen & std::prev(tb)->seen) == 0;
         });
         if (ta == list_a.end( ) || ta->index >= aj) {
            return { aj, bj };
         } else {
            return { ta->index, std::partition_point(list_b.begin( ), list_b.end( ), [&](const auto& entry) {
               return !entry.seen[a[ta->index]];
            })->index };
         }
      }
      return { aj, bj };
   }
};

template<size_t W>
int lcs_rcc0(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, rcc0_topmost<W>(a, b));
}


template<size_t W>
class rcc1_topmost {
   distinct_with_prefix<W> distpref_a;
   sparse_tree<W> sparse_b;
   closest<W> closest_b;

public:
   rcc1_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distpref_a(a), sparse_b(b), closest_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int aj, int bi, int bj) const {
      if (ai != aj && bi != bj) {
         const auto& list_a = distpref_a.query(ai);
         auto chars_b = sparse_b.query(bi, bj);
         auto ta = doubling_search(list_a.begin( ), list_a.end( ), [&](const auto& entry) {
            return entry.index >= aj || (entry.seen & chars_b) != 0;
         });
         if (ta != list_a.end( ) && ta->index < aj) {
            return { ta->index, closest_b.query(bi, a[ta->index]) };
         }
      }
      return { aj, bj };
   }
};

template<size_t W>
int lcs_rcc1(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, rcc1_topmost<W>(a, b));
}


#if defined(__AVX512BITALG__) && defined(__AVX512VL__)
   template<size_t W>
   class rcc2_topmost {
      sparse_tree<W> sparse_a, sparse_b;
      order<W> order_a;
      closest<W> closest_a, closest_b;

   public:
      rcc2_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
      : sparse_a(a), sparse_b(b), order_a(a), closest_a(a), closest_b(b) {
      }

      std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int aj, int bi, int bj) const {
         if (ai != aj && bi != bj) {
            auto common_chars = sparse_a.query(ai, aj) & sparse_b.query(bi, bj);
            if (common_chars.any( )) {
               const auto& list_a = order_a.query(ai);
               auto first_a = std::countr_zero(bit_permute(common_chars, list_a).to_ullong( ));
               return { closest_a.query(ai, list_a[first_a]), closest_b.query(bi, list_a[first_a]) };
            }
         }
         return { aj, bj };
      }
   };

   template<size_t W>
   int lcs_rcc2(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
      return chin_poon_main(a, b, rcc2_topmost<W>(a, b));
   }
#endif

#endif
