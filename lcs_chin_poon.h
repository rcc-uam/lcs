#ifndef LCS_CHIN_POON_H
#define LCS_CHIN_POON_H

#include "string_preprocessing.h"
#include <algorithm>
#include <utility>
#include <vector>
#include <stddef.h>
#include <stdint.h>

template<typename TM>
int chin_poon_main(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, const TM& topmost) {
   std::vector<std::vector<std::pair<int, int>>> mem(2, std::vector<std::pair<int, int>>(std::min(a.size( ), b.size( ))));
   std::pair<std::pair<int, int>*, int> current_contour(mem[0].data( ), 0), next_contour(mem[1].data( ), 0);
   current_contour.first[current_contour.second++] = { -1, -1 };

   for (int length = 0; ; ++length) {
      next_contour.second = 0;
      for (int i = 0, bj = b.size( ); i < current_contour.second; ++i) {
         int ai = current_contour.first[i].first + 1, bi = current_contour.first[i].second + 1;
         int aj = (i + 1 < current_contour.second ? current_contour.first[i + 1].first + 1 : a.size( ));

         for (;;) {
            auto [ak, bk] = topmost(a, b, ai, aj, bi, bj);
            if (ak == aj) {
               break;
            }
            next_contour.first[next_contour.second++] = { ak, bk };
            ai = ak + 1;
            bj = bk;
         }
      }

      std::swap(current_contour, next_contour);
      if (current_contour.second == 0) {
         return length;
      }
   }
}

template<size_t W>
class chin_poon_topmost {
   distinct<W> distinct_a;
   closest<W> closest_b;

public:
   chin_poon_topmost(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
   : distinct_a(a), closest_b(b) {
   }

   std::pair<int, int> operator()(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, int ai, int aj, int bi, int bj) const {
      if (ai != aj && bi != bj) {
         const auto& list_a = distinct_a.query(ai);
         for (auto it = list_a.begin( ); it != list_a.end( ) && *it < aj; ++it) {
            int bk = closest_b.query(bi, a[*it]);
            if (bk < bj) {
               return { *it, bk };
            }
         }
      }
      return { aj, bj };
   }
};

template<size_t W>
int lcs_cp(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   return chin_poon_main(a, b, chin_poon_topmost<W>(a, b));
}

#endif
