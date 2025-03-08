#pragma once

namespace Maths
{
    namespace detail {
        template<typename T>
        inline constexpr T pow_n(T x, std::size_t n) {
            return n == 0 ? T(1) : x * pow_n(x, n - 1);
        }

        template<typename T>
        inline constexpr T factorial(std::size_t n) {
            return n <= 1 ? T(1) : n * factorial<T>(n - 1);
        }

        template<typename T>
        inline constexpr T exp_impl_1(T x, std::size_t n, std::size_t last) {
            return last - n == 1
                ? pow_n(x, n) / factorial<T>(n)
                : exp_impl_1(x, n, n + (last - n) / 2)
                + exp_impl_1(x, n + (last - n) / 2, last);
        }

        template<typename T>
        inline constexpr T exp_impl(T x) {
            // Assume a fixed limit for factorial, here 10 is used but it's arbitrary and can be adjusted
            const std::size_t factorial_limit = 10;
            return !(x > -1)
                ? T(1) / (T(1) + exp_impl_1(-x, 1, factorial_limit))
                : T(1) + exp_impl_1(x, 1, factorial_limit);
        }

        constexpr int MAX_TAYLOR_TERMS = 20;  // Adjusted for better accuracy

        template<typename T>
        inline constexpr T taylor_exp_approx(T x, int n = 1) {
            if (n > MAX_TAYLOR_TERMS) {
                return 1.0;  // Base case, last term
            }
            T term = pow_n(x, n) / factorial<T>(n);
            return term + taylor_exp_approx(x, n + 1);
        }
    }  // namespace detail

    template<typename doubleType>
    inline constexpr doubleType Exp(doubleType x) {
        doubleType result = 1.0 + detail::taylor_exp_approx(x);
        if (std::isnan(result)) {
            std::cerr << "NaN detected in my_exp for x=" << x << std::endl;
        }
        return result;
    }
}
