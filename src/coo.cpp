#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> 

#include "coo.hpp"

void COO::print()
{
    for (int i=0; i<values.size(); i++)
    {
        std::cout << row_indices[i] << " "
                  << col_indices[i] << " "
                  << values[i]
                  << "\n";
    }
}

void sortCOO(COO& coo)
{
    const std::size_t n = coo.values.size();

    std::vector<std::size_t> perm(n);
    std::iota(perm.begin(), perm.end(), 0);

    std::sort(perm.begin(), perm.end(),
        [&](std::size_t a, std::size_t b) {
            if (coo.row_indices[a] != coo.row_indices[b])
                return coo.row_indices[a] < coo.row_indices[b];
            return coo.col_indices[a] < coo.col_indices[b];
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

    apply_perm(coo.row_indices);
    apply_perm(coo.col_indices);
    apply_perm(coo.values);
}