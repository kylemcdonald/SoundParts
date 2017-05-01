#pragma once

#include <string>
#include <vector>

template <class T>
class BinaryDataSync {
private:
    std::vector<T> data;
    size_t nrows = 0;
    size_t ncols = 0;
public:
    void load(std::string filename, size_t rows, size_t cols) {
        this->nrows = rows;
        this->ncols = cols;
        size_t n = rows * cols;
        data.resize(n);
        FILE * file = fopen(filename.c_str() , "rb");
        fread(&data[0], sizeof(T), n, file);
        fclose(file);
    }
    T* operator[](size_t row) {
        return get_row(row);
    }
    T* get_row(size_t row) {
        return &data[row * ncols];
    }
    T& get_element(size_t row, size_t col) {
        return data[row * ncols + col];
    }
    size_t rows() const {
        return nrows;
    }
    size_t cols() const {
        return ncols;
    }
};
