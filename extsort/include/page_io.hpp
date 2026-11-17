#pragma once
#include <iostream>
#include <vector>
#include <cassert>


inline constexpr std::size_t PAGE_SIZE = 8192;

struct Page {
    std::vector<uint32_t> data;
    uint16_t value_per_page;
};

static_assert(sizeof(Page) == 32);