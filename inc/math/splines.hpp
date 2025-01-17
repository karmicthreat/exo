#pragma once

#include "linalg.hpp"

namespace exo
{
namespace math
{

template<typename S, ssize_t D>
struct CatmullRom {
    Vec<S, D>* P = nullptr;

    CatmullRom() = default;

    /**
     * @brief Represents a curve segment that contains exactly 4 control points.
     *        any other points in the array are ignored.
     */
    CatmullRom(Vec<S, D>* control_points)
    {
        P = control_points;
    }

    /**
     * @brief Compute centripetal time knots
     * @returns CatmullRom reference
     */
    CatmullRom& centripetal()
    {
        for (int i = 0; i < 3; ++i)
        {
            auto t = (S)pow((P[i + 1] - P[i]).len(), 0.5);

            _t[i + 1] = t + _t[i];
        }

        return *this;
    }

    /**
     * @brief Compute uniform time knots
     * @returns CatmullRom reference
     */
    CatmullRom& uniform()
    {
        for (int i = 4; i--;) { _t[i] = i / 4.f; }
        return *this;
    }

    /**
     * @brief Computes a point on the spline
     * @parameter t interpolation between control points p1 and p2
     * @returns point on curve at time t
     */
    Vec<S, D> point(S t)
    {
        // t is assumed [0, 1], scale to range [_t0, _t3]
        t *= (_t[2] - _t[1]);
        t += _t[1];

        // compute A's and B's
        compute_A(t);
        compute_B(t);

        { // compute C
            auto den = _t[2] - _t[1];
            return B[0] * ((_t[2] - t) / den) + B[1] * ((t - _t[1]) / den);
        }
    }

    /**
     * @brief Computes the tangent of the spline
     * @parameter t interpolation between control points p1 and p2
     * @returns tangent of curve at time t
     */
    Vec<S, D> tangent(S t)
    {
        Vec<S, D> dA[3] = {};
        Vec<S, D> dB[2] = {};

        // t is assumed [0, 1], scale to range [_t0, _t3]
        t *= (_t[2] - _t[1]);
        t += _t[1];

        compute_A(t);
        compute_B(t);

        { // compute dA's
            for (int i = 0; i < 3; ++i)
            {
                dA[i] = (P[i + 1] - P[0]) / (_t[i + 1] - _t[i]);
            }
        }

        { // compute dB's
            for (int i = 0; i < 2; ++i)
            {
                auto den = _t[i + 2] + _t[i];
                dB[i] = (A[i + 1] - A[i]) / den + dA[i] * ((_t[2] - t) / den) + dA[i + 1] * ((t - _t[i]) / den);
            }
        }

        { // compute dC
            auto den = _t[2] - _t[1];
            return (B[1] - B[0]) / den + dB[0] * ((_t[2] - t) / den) + dB[1] * ((t - _t[1]) / den);
        }
    }

private:
    S _t[4] = {};
    Vec<S, D> A[3] = {};
    Vec<S, D> B[2] = {};

    void compute_A(S t)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto den = _t[i + 1] - _t[i];
            A[i] = P[i] * ((_t[i + 1] - t) / den) + P[i + 1] * ((t - _t[i]) / den);
        }
    }

    void compute_B(S t)
    {
        for (int i = 0; i < 2; ++i)
        {
            auto den = _t[i + 2] - _t[i];
            B[i] = A[i] * ((_t[i + 2] - t) / den) + A[i + 1] * ((t - _t[i]) / den);
        }
    }
};

}
}
