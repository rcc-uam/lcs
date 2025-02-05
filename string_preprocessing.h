#ifndef STRING_PREPROCESSING_H
#define STRING_PREPROCESSING_H

#include "bit.h"
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include <stdint.h>

template<int W>
class closest_occurrence {    // .query(i, c) returns the index of the closest occurrence of c from s[i] to s[s.size( ) - 1], in order of occurrence
   std::vector<std::array<int, W>> table;

public:
   closest_occurrence( ) = default;
   closest_occurrence(const std::vector<uint8_t>& s)
   : table(s.size( )) {
      std::array<int, W> temp;
      std::fill(temp.begin( ), temp.end( ), s.size( ));
      for (int i = s.size( ) - 1; i >= 0; --i) {
         temp[s[i]] = i;
         table[i] = temp;
      }
   }

   int query(int i, uint8_t c) const {
      return table[i][c];
   }
};

template<int W>
class next_distinct {      // .query(i) returns the list of distinct chars that appear from s[i] to s[s.size( ) - 1], in order of occurrence; each char is accompanied by its index and a prefix set union from s[i] to s[index]
   struct entry {
      uint8_t symbol;
      int index;
      std::bitset<W> set;
   };
   std::vector<std::vector<entry>> lists;

public:
   next_distinct( ) = default;
   next_distinct(const std::vector<uint8_t>& s)
   : lists(s.size( )) {
      std::vector<entry> temp;
      for (int i = s.size( ) - 1; i >= 0; --i) {
         std::erase_if(temp, [&](const entry& e) {
            return e.symbol == s[i];
         });
         temp.insert(temp.begin( ), entry(s[i], i, std::bitset<W>( )));
         for (auto& e : temp) {
            e.set[s[i]] = true;
         }
         lists[i] = temp;
      }
   }

   const std::vector<entry>& query(int i) const {
      return lists[i];
   }
};

template<int W>
class order_distinct {     // .query(i) returns the list of distinct chars that appear from s[i] to s[s.size( ) - 1], in order of occurrence
   std::vector<std::array<uint8_t, W>> lists;

public:
   order_distinct( ) = default;
   order_distinct(const std::vector<uint8_t>& s)
   : lists(s.size( )) {
      std::vector<uint8_t> temp;
      for (int i = s.size( ) - 1; i >= 0; --i) {
         std::erase_if(temp, [&](uint8_t c) {
            return c == s[i];
         });
         temp.insert(temp.begin( ), s[i]);
         std::copy(temp.begin( ), temp.end( ), lists[i].begin( ));
      }
   }

   const std::array<uint8_t, W>& query(int i) const {
      return lists[i];
   }
};

template<int W>
class sparse_table {       // .query(i, j) returns the set union of chars that appear from s[i] to s[j - 1]
   std::vector<std::vector<std::bitset<W>>> table;

public:
   sparse_table( ) = default;
   sparse_table(const std::vector<uint8_t>& s)
   : sparse_table(to_bitset_vector<W>(s)) {
   }
   sparse_table(const std::vector<std::bitset<W>>& s)
   : table({ s }) {
      for (int k = 2; k <= s.size( ); k *= 2) {
         std::vector<std::bitset<W>> temp = table.back( );
         for (int i = 0; i < s.size( ); ++i) {
            temp[i] |= temp[std::min(i + k / 2, (int)s.size( ) - 1)];
         }
         table.push_back(temp);
      }
   }

   std::bitset<W> query(int i, int j) const {
      if (i == j) {
         return std::bitset<W>( );
      } else {
         int t = std::bit_floor(unsigned(j - i));
         int x = std::countr_zero(unsigned(t));
         return table[x][i] | table[x][i + (j - i - t)];
      }
   }
};

template<int W>
class sparse_tree {       // .query(i, j) returns the set union of chars that appear from s[i] to s[j - 1]
   struct node_segment {
      std::vector<std::bitset<W>> prefix, suffix;
   };
   struct tree_node {
      std::vector<node_segment> segments;
      sparse_table<W> table;
   };
   struct tree_layer {
      int segment_size, node_size;
      std::vector<tree_node> nodes;
   };

   const std::vector<uint8_t>* source;
   std::vector<tree_layer> layers;

public:
   sparse_tree( ) = default;
   sparse_tree(const std::vector<uint8_t>& s)
   : source(&s) {
      for (int stop = false, ss = 4, ns = 16; !stop; stop = (ns >= s.size( )), ss = (1 << ss), ns = (ns <= 16 ? (1 << ns) : s.size( ) + (ns == s.size( )))) {
         tree_layer layer = { ss, ns };
         for (int i = 0; i < s.size( ); i += ns) {
            tree_node node;
            std::vector<std::bitset<W>> repr;
            for (int j = i; j < std::min(i + ns, (int)s.size( )); j += ss) {
               auto iter1 = source->begin( ) + j, iter2 = source->begin( ) + std::min(j + ss, (int)s.size( ));
               node_segment segment;
               std::transform_inclusive_scan(iter1, iter2, std::back_inserter(segment.prefix), std::bit_or( ), to_bitset<W>);
               std::transform_inclusive_scan(std::reverse_iterator(iter2), std::reverse_iterator(iter1), std::back_inserter(segment.suffix), std::bit_or( ), to_bitset<W>);
               std::reverse(segment.suffix.begin( ), segment.suffix.end( ));
               node.segments.push_back(std::move(segment));
               repr.push_back(node.segments.back( ).prefix.back( ));
            }
            node.table = sparse_table<W>(std::move(repr));
            layer.nodes.push_back(std::move(node));
         }
         layers.push_back(std::move(layer));
      }
   }

   std::bitset<W> query(int i, int j) const {
      if (j - i <= 4) {
         return std::transform_reduce(source->begin( ) + i, source->begin( ) + j, std::bitset<W>( ), std::bit_or( ), to_bitset<W>);
      } else {
         std::bitset<W> res;
         int t = std::bit_floor(unsigned(j - i)), xlayer = (t <= 16 ? 0 : (t <= 65536 ? 1 : 2));     // in general, this can be done in O(1) with a lookup table that is computed during the construction of the sparse tree, even for very large values of t
         for (int xnode = i / layers[xlayer].node_size; xnode < j / layers[xlayer].node_size + bool(j % layers[xlayer].node_size); ++xnode) {
            int xi = std::max(i - xnode * layers[xlayer].node_size, 0), xj = std::min(j - xnode * layers[xlayer].node_size, layers[xlayer].node_size);
            res |= layers[xlayer].nodes[xnode].table.query(xi / layers[xlayer].segment_size + bool(xi % layers[xlayer].segment_size), xj / layers[xlayer].segment_size) |
               (xi % layers[xlayer].segment_size != 0 ? layers[xlayer].nodes[xnode].segments[xi / layers[xlayer].segment_size].suffix[xi % layers[xlayer].segment_size] : std::bitset<W>( )) |
               (xj % layers[xlayer].segment_size != 0 ? layers[xlayer].nodes[xnode].segments[xj / layers[xlayer].segment_size].prefix[xj % layers[xlayer].segment_size - 1] : std::bitset<W>( ));
         }
         return res;
      }
   }
};

#endif
