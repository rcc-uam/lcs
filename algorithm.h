#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <algorithm>
#include <functional>

template<typename RI>
RI doubling_search(RI begin, RI end, const auto& pred) {
   int i = 0;
   while (i < end - begin && !pred(begin[i])) {
      i = std::max(i + 1, i * 2);
   }
   return std::partition_point(begin + i / 2, begin + std::min(i, int(end - begin)), std::not_fn(pred));
}

#endif
