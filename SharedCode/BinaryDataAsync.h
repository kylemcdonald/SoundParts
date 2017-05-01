/* Copyright 2017 Kyle McDonald */
#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

template <class T>
class BinaryDataAsync {
private:
    T* data = NULL;
    size_t nbytes = 0;
    size_t nrows = 0;
    size_t ncols = 0;
public:
    BinaryDataAsync() {}
    ~BinaryDataAsync() {
        unload();
    }
    int load(std::string filename, size_t rows, size_t cols, bool sequential=false) {
        this->nrows = rows;
        this->ncols = cols;
        size_t n = rows * cols;
        nbytes = n * sizeof(T);
        int fd = open(filename.c_str(), O_RDONLY);
        data = reinterpret_cast<T*>(mmap(NULL, nbytes, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
        if (data == MAP_FAILED) {
            std::cerr << "Couldn't map " << filename << std::endl;
            return -1;
        }
        if(madvise(data, nbytes, (sequential?MADV_SEQUENTIAL:MADV_RANDOM) | MADV_WILLNEED) != 0) {
            std::cerr << "Couldn't set hints." << std::endl;
            return -1;
        }
        close(fd);
        return 0;
    }
    int unload() {
        if(data == NULL) return;
        if(munmap(data, nbytes) != 0) {
            std::cerr << "Couldn't unmap." << std::endl;
            return -1;
        }
        data = NULL;
        nrows = 0;
        ncols = 0;
        return 0;
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
