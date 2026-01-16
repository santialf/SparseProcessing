#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <coo.hpp>
#include <csr.hpp>

void convertCOOToCSR(const COO&, CSR&);
