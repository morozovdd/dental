#pragma once
#include <cmath>

namespace dental::geometry {
inline constexpr double kEpsilon = 1e-9;

inline bool nearZero(double value) { return std::abs(value) < kEpsilon; }
} // namespace dental::geometry
