#include <complex>
#include <vector>
#include <omp.h>

const double PI = 3.141592653589793238460;

#ifdef ZFP_RECURSIVE_FORM
template <typename T>
void fft(std::vector<std::complex<T>>& a) {
    int n = a.size();
    if (n <= 1) return;

    std::vector<std::complex<T>> even(n / 2);
    std::vector<std::complex<T>> odd(n / 2);

    #pragma omp parallel for
    for (int i = 0; i < n / 2; ++i) {
        even[i] = a[i * 2];
        odd[i] = a[i * 2 + 1];
    }

    fft(even);
    fft(odd);

    #pragma omp parallel for
    for (int k = 0; k < n / 2; ++k) {
        std::complex<T> t = std::polar(T(1.0), T(-2 * PI * k / n)) * odd[k];
        a[k] = even[k] + t;
        a[k + n / 2] = even[k] - t;
    }
}
#else
template <typename T>
void fft(std::vector<std::complex<T>>& a) {
    int n = a.size();
    int k = n;

    for (int i = 0; i < n; ++i) {
        if (i < k) {
            std::swap(a[i], a[k]);
        }

        int j = n;
        while (j <= k) {
            k -= j;
            j /= 2;
        }

        k += j;
    }

    for (int m = 1; m < n; m *= 2) {
        std::complex<T> wm = std::polar(T(1.0), T(-2 * PI / m));
        for (int k = 0; k < n; k += 2 * m) {
            std::complex<T> w = 1;
            for (int j = 0; j < m; ++j) {
                std::complex<T> t = w * a[k + j + m];
                std::complex<T> u = a[k + j];
                a[k + j] = u + t;
                a[k + j + m] = u - t;
                w *= wm;
            }
        }
    }
}
#endif

// Explicit template instantiation
template void fft<float>(std::vector<std::complex<float>>&);
template void fft<double>(std::vector<std::complex<double>>&);
