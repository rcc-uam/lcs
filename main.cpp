#include "lcs_chin_poon.h"
#include "lcs_rcc.h"
#include "lcs_wagner_fischer.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <stdint.h>

template<int I, int J, int S = (I <= J ? +1 : -1), auto M = std::plus<int>( )>
constexpr void unrolled_for(auto&& f) noexcept {
   if constexpr(I == J) {
      return;
   } else if (f(std::integral_constant<int, I>( ))) {
      unrolled_for<M(I, S), J, S, M>(f);
   }
}

int main(int argc, const char* argv[]) {
   std::cout.setf(std::ios::unitbuf);

   if (argc - 1 < 2) {
      std::cout << "Usage: " << argv[0] << " file1 file2\n";
      return 0;
   }

   std::vector<std::vector<uint8_t>> strings;
   for (const auto& file : { argv[1], argv[2] }) {
      std::string temp = std::move(std::stringstream( ) << std::ifstream(file, std::ios::binary).rdbuf( )).str( );
      strings.emplace_back(temp.begin( ), temp.end( ));
   }

   std::cout << "Computing alphabet size: ";
   auto [min0, max0] = std::minmax_element(strings[0].begin( ), strings[0].end( ));
   auto [min1, max1] = std::minmax_element(strings[1].begin( ), strings[1].end( ));
   int min = std::min(*min0, *min1), max = std::max(*max0, *max1);
   for (auto& string : strings) {
      for (auto& c : string) {
         c -= min;
      }
   }
   int sigma = max - min + 1;
   std::cout << sigma << "\n";

   if (strings[0].empty( ) || strings[1].empty( )) {
      std::cout << "Empty input string\n";
      return 0;
   } else if (sigma > 256) {
      std::cout << "Unsupported alphabet size (symbols encoded as uint8_t)\n";
      return 0;
   }

   std::vector<std::pair<const char*, int(*)(const std::vector<uint8_t>&, const std::vector<uint8_t>&)>> algorithms;
   unrolled_for<4, 512, 2, (std::multiplies<int>( ))>([&](auto W) {
      if (sigma <= W) {
         algorithms = {
            { "Wagner-Fischer", lcs_wf },
            { "Chin & Poon",    lcs_cp<W> },
            { "RCC-0",          lcs_rcc0<W> },
            { "RCC-1",          lcs_rcc1<W> }
         };
         #if defined(__AVX512BITALG__) && defined(__AVX512VL__)
            if constexpr(W <= 64) {
               algorithms.emplace_back("RCC-2", lcs_rcc2<W>);
            } else {
               std::cout << "RCC-2 algorithm sample implementation only supports sigma <= 64\n";
            }
         #else
            std::cout << "RCC-2 algorithm not supported with the current CPU architecture\n";
         #endif
         return false;
      }
      return true;
   });

   for (const auto& [label, function] : algorithms) {
      std::cout << "Computing " << label << ": ";
      auto t0 = std::clock( );
      int res = function(strings[0], strings[1]);
      auto t1 = std::clock( );
      std::cout << res << " in " << double(t1 - t0) / CLOCKS_PER_SEC << " seconds\n";
   }
}
