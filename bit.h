#ifndef BIT_PERMUTATION_H
#define BIT_PERMUTATION_H

#include <array>
#include <bitset>
#include <vector>
#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

template<size_t W>
std::bitset<W> to_bitset(uint8_t c) {
   std::bitset<W> res;
   res[c] = true;
   return res;
}

template<size_t W>
std::vector<std::bitset<W>> to_bitset_vector(const std::vector<uint8_t>& v) {
   std::vector<std::bitset<W>> res;
   res.reserve(v.size( ));
   for (uint8_t c : v) {
      res.push_back(to_bitset<W>(c));
   }
   return res;
}

#if defined(__AVX512BITALG__) && defined(__AVX512VL__)
   template<size_t W>
   std::bitset<W> bit_permute(std::bitset<W> b, const std::array<uint8_t, W>& indices) requires (W <= 32) {
      return _cvtmask32_u32(_mm256_bitshuffle_epi64_mask(_mm256_set1_epi64x(b.to_ulong( )), _mm256_loadu_si256((const __m256i*)indices.data( ))));
   }

   template<size_t W>
   std::bitset<W> bit_permute(std::bitset<W> b, const std::array<uint8_t, W>& indices) requires (W <= 64 && W > 32) {
      return _cvtmask64_u64(_mm512_bitshuffle_epi64_mask(_mm512_set1_epi64(b.to_ullong( )), _mm512_loadu_si512(indices.data( ))));
   }
#endif

#endif
