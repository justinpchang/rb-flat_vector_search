#ifndef METRICS_HPP
#define METRICS_HPP 1

#include <cmath>
#include <vector>

double cosine_distance(const std::vector<double> &A, const std::vector<double> &B, unsigned int n_dims)
{
    double dot = 0.0;
    double denom_a = 0.0;
    double denom_b = 0.0;

    for (unsigned int i = 0u; i < n_dims; i += 1)
    {
        dot += A[i] * B[i];
        denom_a += A[i] * A[i];
        denom_b += B[i] * B[i];
    }

    return 1 - dot / (sqrt(denom_a) * sqrt(denom_b));
}

#endif /* METRICS_HPP */