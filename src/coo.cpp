#include <numeric> 
#include "coo.hpp"

template<typename valueType>
void COO<valueType>::print()
{
    for (size_t i=0; i<values.size(); i++)
    {
        std::cout << row_indices[i] << " "
                  << col_indices[i] << " "
                  << values[i]
                  << "\n";
    }
}

template<typename valueType>
void COO<valueType>::add_entry(size_t row, size_t col, const valueType& value) 
{
    if (row >= num_rows || col >= num_cols) 
    {
        throw std::out_of_range("COO: index out of bounds");
    }
    row_indices.push_back(row);
    col_indices.push_back(col);
    values.push_back(value);
}

template<typename valueType>
void COO<valueType>::clear() 
{
    row_indices.clear();
    col_indices.clear();
    values.clear();
}

template<typename valueType>
void COO<valueType>::sort()
{
    const std::size_t n = values.size();

    std::vector<std::size_t> perm(n);
    std::iota(perm.begin(), perm.end(), 0);

    std::sort(perm.begin(), perm.end(),
        [&](std::size_t a, std::size_t b) {
            if (row_indices[a] != row_indices[b])
                return row_indices[a] < row_indices[b];
            return col_indices[a] < col_indices[b];
        });

    auto apply_perm = [&](auto& v) {
        using V = typename std::decay<decltype(v)>::type::value_type;
        std::vector<V> tmp;
        tmp.reserve(n);
        for (auto i : perm)
            tmp.push_back(v[i]);
        v.swap(tmp);
    };

    apply_perm(row_indices);
    apply_perm(col_indices);
    apply_perm(values);
}