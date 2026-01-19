#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

namespace mtx {

class sparseFormat {
protected:
    size_t num_rows;
    size_t num_cols;

public:
    sparseFormat(size_t rows_, size_t cols_)
    : num_rows(rows_), num_cols(cols_) {}

    virtual ~sparseFormat() = default;

    size_t getRows() const { return num_rows; }
    size_t getCols() const { return num_cols; }

    virtual std::size_t getNnz() const = 0;

    virtual double getDensity() const {
        return static_cast<double>(getNnz()) / static_cast<double>(num_rows * num_cols);
    }
};

} // namespace mtx