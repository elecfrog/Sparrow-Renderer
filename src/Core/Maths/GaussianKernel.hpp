#pragma once

#include "MathUtils.h"
#include "Exp.hpp"
#include "Pow.hpp"
#include "glm/glm.hpp"

namespace Maths
{
// Function to compute Gaussian density in 2D
double gaussian2D(double x, double y, double sigma, double mean) {
    // Compute the exponent part of the Gaussian formula
    double exponent = -0.5 * (Pow2((x - mean) / sigma) + Pow2((y - mean) / sigma));
    // Return the Gaussian value
    return exp(exponent) / (2 * M_PI * Pow2(sigma));
}

// Template function to compute Gaussian kernel of given size
template<size_t KERNEL_SIZE>
constexpr std::array<double, KERNEL_SIZE * KERNEL_SIZE> GaussianKernel(double sigma = 1.0) {
    // Calculate the center or mean of the kernel
    constexpr double mean = (KERNEL_SIZE - 1) / 2.0;
    std::array<double, KERNEL_SIZE * KERNEL_SIZE> kernel;
    double sum = 0.0;  // For normalization

    for (size_t i = 0; i < KERNEL_SIZE; i++) {
        for (size_t j = 0; j < KERNEL_SIZE; j++) {
            // Compute each value in the kernel
            kernel[i * KERNEL_SIZE + j] = gaussian2D(i, j, sigma, mean);
            sum += kernel[i * KERNEL_SIZE + j];
        }
    }

    // Normalize the kernel so that its values sum up to 1
    for (size_t i = 0; i < KERNEL_SIZE; i++) {
        for (size_t j = 0; j < KERNEL_SIZE; j++) {
            kernel[i * KERNEL_SIZE + j] /= sum;
        }
    }

    return kernel;
}

// Function to convert Gaussian kernel to glm::mat3 (3x3 matrix)
constexpr glm::mat3 GaussianKernelMat3(double sigma = 1.0) {
    glm::mat3 res;
    // Get the 3x3 kernel
    auto arr = GaussianKernel<3>(sigma);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            // Assign the value from kernel array to the matrix
            res[row][col] = static_cast<float>(arr[row * 3 + col]);
        }
    }
    return res;
}

}  // namespace maths
