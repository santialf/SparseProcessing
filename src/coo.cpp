#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> 

#include "coo.hpp"

void printCOO(const COO& entries)
{
    for (int i=0; i<entries.values.size(); i++)
    {
        std::cout << entries.row_indices[i] << " "
                  << entries.col_indices[i] << " "
                  << entries.values[i]
                  << "\n";
    }
}

void sortCOO(COO& entries)
{
    const std::size_t n = entries.values.size();

    std::vector<std::size_t> perm(n);
    std::iota(perm.begin(), perm.end(), 0);

    std::sort(perm.begin(), perm.end(),
        [&](std::size_t a, std::size_t b) {
            if (entries.row_indices[a] != entries.row_indices[b])
                return entries.row_indices[a] < entries.row_indices[b];
            return entries.col_indices[a] < entries.col_indices[b];
        });

    // Step 2: Apply permutation to all arrays
    auto apply_perm = [&](auto& v) {
        using V = typename std::decay<decltype(v)>::type::value_type;
        std::vector<V> tmp;
        tmp.reserve(n);
        for (auto i : perm)
            tmp.push_back(v[i]);
        v.swap(tmp);
    };

    apply_perm(entries.row_indices);
    apply_perm(entries.col_indices);
    apply_perm(entries.values);
}