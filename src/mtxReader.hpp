#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "coo.hpp"

extern "C" 
{
#include "mmio.h"
}

bool readMtxLine(FILE*, MM_typecode, size_t&, size_t&, double&);
COO readMtxCoordinates(FILE*);
