#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>

// Base class: Matrix
class Matrix {
public:
    virtual ~Matrix() = default;
    virtual void print() const = 0;
    virtual void exportToFile(const std::string& filename) const = 0;
    virtual void importFromFile(const std::string& filename) = 0;
};

// Dense Matrix Class
template <typename T = double>
class MatrixDense : public Matrix {
private:
    unsigned _m, _n;
    T* _data;

public:
    MatrixDense(unsigned n, unsigned m) : _m(m), _n(n) {
        _data = new T[_m * _n];
        for (unsigned i = 0; i < _m * _n; ++i) {
            _data[i] = static_cast<T>(0); 
        }
    }

    ~MatrixDense() {
        delete[] _data;
    }

    T& operator()(unsigned i, unsigned j) {
        if (i >= _m || j >= _n) {
            std::cerr << "Ошибка: индекс вне границ!" << std::endl;
            throw std::out_of_range("Index out of bounds");
        }
        return _data[i * _n + j];
    }

    const T& operator()(unsigned i, unsigned j) const {
        if (i >= _m || j >= _n) {
            std::cerr << "Ошибка: индекс вне границ!" << std::endl;
            throw std::out_of_range("Index out of bounds");
        }
        return _data[i * _n + j];
    }

    MatrixDense<T> operator+(const MatrixDense<T>& other) const {
        if (_m != other._m || _n != other._n) {
            std::cerr << "Ошибка: размеры матриц не совпадают!" << std::endl;
            throw std::invalid_argument("Matrix dimensions must match");
        }
        MatrixDense<T> result(_n, _m);
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j); 
            }
        }
        return result;
    }

    MatrixDense<T> operator*(const T scalar) const {
        MatrixDense<T> result(_n, _m);
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result(i, j) = (*this)(i, j) * scalar; 
            }
        }
        return result;
    }

    void print() const override {
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                std::cout << std::setw(5) << (*this)(i, j) << " "; 
            }
            std::cout << "\n";
        }
    }

    void exportToFile(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для записи!" << std::endl;
            throw std::runtime_error("Unable to open file");
        }
        file << "MatrixDense\n";
        file << _m << " " << _n << "\n";
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                file << (*this)(i, j) << " ";
            }
            file << "\n";
        }
        file.close();
    }

    void importFromFile(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для чтения!" << std::endl;
            throw std::runtime_error("Unable to open file");
        }
        std::string line;
        std::getline(file, line);
        if (line != "MatrixDense") {
            std::cerr << "Ошибка: файл не содержит MatrixDense!" << std::endl;
            throw std::invalid_argument("File does not contain MatrixDense");
        }
        unsigned m, n;
        file >> m >> n;
        if (m != _m || n != _n) {
            std::cerr << "Ошибка: размеры матриц не совпадают!" << std::endl;
            throw std::invalid_argument("Matrix dimensions do not match");
        }
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                file >> (*this)(i, j);
            }
        }
        file.close();
    }
};

int main() {
    try {
        MatrixDense<> mat1(3, 3);
        mat1(0, 0) = 1.0;
        mat1(1, 1) = 2.0;
        mat1(2, 2) = 3.0;

        std::cout << "Matrix 1:\n";
        mat1.print();

        mat1.exportToFile("matrix_dense.txt");

        MatrixDense<> mat2(3, 3);
        mat2.importFromFile("matrix_dense.txt");

        std::cout << "Matrix 2 (imported):\n";
        mat2.print();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}
