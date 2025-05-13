#pragma once

#include <iostream>

#define LOG(str) std::cerr << __FILE__ << ' ' << __LINE__ << ' ' << __TIMESTAMP__ << ' ' << str << std::endl;