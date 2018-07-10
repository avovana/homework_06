#pragma once

#include "proxy.h"

template <typename T, std::size_t Size = 2>
struct Data
{
    using ElementType = T;
    using Container = std::map<Indexes<Size>, ElementType>;
};