#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

int main( ) {
   int size, sigma, percentage;
   std::cin >> size >> sigma >> percentage;

   std::mt19937 gen((std::random_device( ))( ));
   std::vector<uint8_t> v0(size), v1(size), v2(size), *s[2] = { &v1, &v2 };
   std::bernoulli_distribution bdist(percentage / 100.0);
   std::uniform_int_distribution<uint8_t> equal(0, sigma / 2 - 1), unequal(sigma / 2, sigma - 1);

   for (int i = 0; i < size; ++i) {
      v0[i] = equal(gen);
   }
   for (auto fill : s) {
      auto iter = v0.begin( );
      for (int i = 0; i < size; ++i) {
         if (bdist(gen)) {
            (*fill)[i] = *iter++;
         } else {
            (*fill)[i] = (fill == &v1 ? equal : unequal)(gen);
         }
      }
   }

   std::string path = std::format("./test_{}_{}_{}", size, sigma, percentage);
   std::filesystem::create_directory(path);
   for (int i = 0; i < 2; ++i) {
      std::ofstream file(path + "/" + std::to_string(i) + ".dat", std::ios::binary | std::ios::out);
      file.write((const char*)s[i]->data( ), size);
   }
}
