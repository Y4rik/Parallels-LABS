#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>

// Template class definition
template <typename T>
class Vector {
private:
    T* data;
    size_t n;
    bool is_initialized;
    std::mutex mtx;

public:
    // Constructor
    Vector(size_t size) {
        n = size;
        is_initialized = false;
        data = new T[n];
        for (size_t i = 0; i < n; ++i) {
            data[i] = T(); // Initialize elements with default value
        }
    }

    // Destructor
    ~Vector() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    // Initialize all elements with a constant
    void initialize_with_constant(T value) {
        for (size_t i = 0; i < n; i++) {
            data[i] = value;
        }
        is_initialized = true;
    }

    // Initialize with random values in a range
    void initialize_with_random(T min_val, T max_val) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dist(min_val, max_val);

        for (size_t i = 0; i < n; i++) {
            data[i] = dist(gen);
        }
        is_initialized = true;
    }

    // Export vector to a file
    void export_to_file(const std::string& filename) {
        if (!is_initialized) {
            std::cout << "Vector is not initialized! Cannot export." << std::endl;
            return;
        }

        std::ofstream file;
        file.open(filename);
        if (!file) {
            std::cout << "Error opening file for export!" << std::endl;
            return;
        }

        for (size_t i = 0; i < n; i++) {
            file << data[i] << "\n";
        }
        file.close();
    }

    // Import vector from a file
    void import_from_file(const std::string& filename) {
        std::ifstream file;
        file.open(filename);
        if (!file) {
            std::cout << "Error opening file for import!" << std::endl;
            return;
        }

        for (size_t i = 0; i < n && file; i++) {
            file >> data[i];
        }
        is_initialized = true;
        file.close();
    }

    // Find minimum element and its index
    std::pair<T, size_t> find_min() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return { T(), 0 };
        }

        T min_val = data[0];
        size_t min_index = 0;
        for (size_t i = 1; i < n; i++) {
            if (data[i] < min_val) {
                min_val = data[i];
                min_index = i;
            }
        }
        return { min_val, min_index };
    }

    // Find maximum element and its index
    std::pair<T, size_t> find_max() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return { T(), 0 };
        }

        T max_val = data[0];
        size_t max_index = 0;
        for (size_t i = 1; i < n; i++) {
            if (data[i] > max_val) {
                max_val = data[i];
                max_index = i;
            }
        }
        return { max_val, max_index };
    }

    // Calculate sum of elements
    T sum() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return T();
        }

        T total = 0;
        for (size_t i = 0; i < n; i++) {
            total += data[i];
        }
        return total;
    }

    // Calculate mean of elements
    double mean() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return 0.0;
        }

        return static_cast<double>(sum()) / n;
    }

    // Calculate Euclidean norm
    double euclidean_norm() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return 0.0;
        }

        double norm = 0.0;
        for (size_t i = 0; i < n; i++) {
            norm += static_cast<double>(data[i] * data[i]);
        }
        return sqrt(norm);
    }

    // Calculate Manhattan norm
    T manhattan_norm() {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return T();
        }

        T norm = 0;
        for (size_t i = 0; i < n; i++) {
            norm += abs(data[i]);
        }
        return norm;
    }

    // Scalar product of two vectors
    static T scalar_product(const Vector& a, const Vector& b) {
        if (!a.is_initialized || !b.is_initialized) {
            std::cout << "One or both vectors are not initialized!" << std::endl;
            return T();
        }

        if (a.n != b.n) {
            std::cout << "Vectors are not the same size!" << std::endl;
            return T();
        }

        T product = 0;
        for (size_t i = 0; i < a.n; i++) {
            product += a.data[i] * b.data[i];
        }
        return product;
    }

    // Multithreaded processing example (parallel sum)
    T parallel_sum(size_t num_threads) {
        if (!is_initialized) {
            std::cout << "Vector is not initialized!" << std::endl;
            return T();
        }

        std::vector<std::thread> threads;
        T result = 0;
        size_t chunk_size = n / num_threads;
        std::vector<T> partial_sums(num_threads, 0);

        for (size_t t = 0; t < num_threads; t++) {
            threads.emplace_back([&, t]() {
                size_t start = t * chunk_size;
                size_t end = (t == num_threads - 1) ? n : start + chunk_size;
                for (size_t i = start; i < end; i++) {
                    partial_sums[t] += data[i];
                }
                });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        for (const auto& partial_sum : partial_sums) {
            result += partial_sum;
        }

        return result;
    }
};

int main() {
    size_t vector_size = 1000; // Small size for demonstration
    Vector<double> vec(vector_size);

    vec.initialize_with_random(-10.0, 10.0);
    std::cout << "Sum: " << vec.sum() << std::endl;
    std::cout << "Mean: " << vec.mean() << std::endl;

    return 0;
}
