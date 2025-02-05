#include <ctime>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

void generate(int n, int size, int sigma, const std::string& dtype, auto& gen, auto dist) {
   std::string path = std::format("./test_{}_{}_{}", size, sigma, dtype);
   std::filesystem::create_directory(path);
   for (int i = 0; i < n; ++i) {
      std::ofstream file(path + "/" + std::to_string(i) + ".dat", std::ios::binary | std::ios::out);
      for (int j = 0; j < size; ++j) {
         file.put(dist(gen));
      }
   }
}

int main( ) {
   int n, size, sigma; std::string dtype;
   std::cin >> n >> size >> sigma >> dtype;

   std::mt19937 gen(std::time(nullptr));
   if (dtype == "linear") {
      generate(n, size, sigma, dtype, gen, std::uniform_int_distribution<uint8_t>(0, sigma - 1));
   } else if (dtype == "binomial") {
      generate(n, size, sigma, dtype, gen, std::binomial_distribution<uint8_t>(sigma - 1));
   }
}
