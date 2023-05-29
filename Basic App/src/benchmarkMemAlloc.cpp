// #include "App.hpp"

// #include <stdexcept>
// #include <iostream>
// #include <cstring>
// #include <chrono>
// #include <vector>
// #include <numeric>

// char **cpy2Darr(const char **arr, const size_t size) {
//     char **copy = new char*[size];
//     for (int i = 0; i < size; i++) {
//         size_t len = strlen(arr[i]);
//         copy[i] = new char[len];
//         std::memcpy(copy[i], arr[i], len);
//     }
//     return copy;
// }


// void destroy2DArr(char **arr, const size_t size) {
//     for (int i = 0; i < size; i++) {
//         delete[] arr[i];
//     }
//     delete[] arr;
// }

// std::vector<const char *> getVec(const char **arr, const size_t len) {
//     std::vector<const char *> vec(len);
//     for (int i = 0; i < len; i++) {
//         vec.push_back(arr[i]);
//     }
//     return vec;
// }



// void benchmark(const char **arr, const size_t len, const uint32_t n_episode) {
//     std::vector<int64_t> manualAllocTimes(n_episode);
//     std::vector<int64_t> vectorAllocTimes(n_episode);

//     for (int i = 0; i < n_episode; i++) {
//         auto start = std::chrono::high_resolution_clock::now();
//         char **temp = cpy2Darr(arr, 8);
//         destroy2DArr(temp, len);
//         auto end = std::chrono::high_resolution_clock::now();
//         manualAllocTimes[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
//     }
//     for (int i = 0; i < n_episode; i++) {
//         auto start = std::chrono::high_resolution_clock::now();
//         std::vector<const char*> vec = getVec(arr, len);
//         vec.clear();
//         auto end = std::chrono::high_resolution_clock::now();
//         vectorAllocTimes[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
//     }
//     std::cout << "For " << n_episode << " episodes : " << std::endl;
//     std::cout << "Manual allocation mean time : "
//     << static_cast<double>( std::accumulate(manualAllocTimes.begin(), manualAllocTimes.end(), 0) ) / manualAllocTimes.size()
//     << " nanoseconds" << std::endl;
//     std::cout << "Vector allocation mean time : "
//     << static_cast<double>( std::accumulate(vectorAllocTimes.begin(), vectorAllocTimes.end(), 0) ) / vectorAllocTimes.size()
//     << " nanoseconds" << std::endl;
// }

// // int main() {
// //     const char *arr[] = {"Hello", "World", "!", "bonjour", "a ", "tout ", "le ", "monde"};
// //     const char **ptr = arr;
// //     const size_t len = 8;
// //     benchmark(ptr, len, 1);
// //     benchmark(ptr, len, 100000000);
// //     benchmark(ptr, len, 100);
// //     benchmark(ptr, len, 100000);
// // }
