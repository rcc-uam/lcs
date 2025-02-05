#ifndef LCS_CHIN_POON_H
#define LCS_CHIN_POON_H

#include "string_preprocessing.h"
#include <algorithm>
#include <utility>
#include <vector>
#include <stdint.h>

template<typename TM>
int chin_poon_main(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, const TM& topmost) {
   std::vector<std::vector<std::pair<int, int>>> mem(2, std::vector<std::pair<int, int>>(std::min(a.size( ), b.size( ))));
   std::pair<std::pair<int, int>*, int> current_contour(mem[0].data( ), 0), next_contour(mem[1].data( ), 0);
   current_contour.first[current_contour.second++] = { -1, -1 };

   for (int length = 0; ; ++length) {
      next_contour.second = 0;
      for (int i = 0, bf = b.size( ); i < current_contour.second; ++i) {
         int ai = current_contour.first[i].first + 1, bi = current_contour.first[i].second + 1;
         int af = (i + 1 < current_contour.second ? current_contour.first[i + 1].first + 1 : a.size( ));

         for (;;) {
            auto [aj, bj] = topmost(a, b, ai, af, bi, bf);
            if (aj == af) {
               break;
            }
            next_contour.first[next_contour.second++] = { aj, bj };
            ai = aj + 1;
            bf = bj;
         }
      }

      std::swap(current_contour, next_contour);
      if (current_contour.second == 0) {
         return length;
      }
   }
}

template<int W>
class chin_poon_topmost {
   next_distinct<W> distinct_a;
   closest_occurrence<W> closest_b;

public:
   chin_poon_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distinct_a(a), closest_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int af, int bi, int bf) const {
      if (ai != af && bi != bf) {
         const auto& list_a = distinct_a.query(ai);
         for (auto it = list_a.begin( ); it != list_a.end( ) && it->index < af; ++it) {
            int bj = closest_b.query(bi, it->symbol);
            if (bj < bf) {
               return { it->index, bj };
            }
         }
      }
      return { af, bf };
   }
};

template<int W>
int lcs_cp(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, chin_poon_topmost<W>(a, b));
}

#endif
