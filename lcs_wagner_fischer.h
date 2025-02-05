#ifndef LCS_WAGNER_FISCHER_H
#define LCS_WAGNER_FISCHER_H

#include <vector>
#include <stdint.h>

int lcs_wf(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
   std::vector<std::vector<int>> mem(2, std::vector<int>(b.size( ) + 1));
   for (int i = a.size( ); i >= 0; --i) {
      for (int j = b.size( ); j >= 0; --j) {
         if (i == a.size( ) || j == b.size( )) {
            mem[i % 2][j] = 0;
         } else if (a[i] == b[j]) {
            mem[i % 2][j] = 1 + mem[(i + 1) % 2][j + 1];
         } else {
            mem[i % 2][j] = std::max(mem[(i + 1) % 2][j], mem[i % 2][j + 1]);
         }
      }
   }
   return mem[0][0];
}

#endif
