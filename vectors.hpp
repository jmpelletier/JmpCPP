/*
MIT License

Copyright (c) 2023 Jean-Marc Pelletier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <type_traits>
#include <cmath>

namespace JMP
{
    template <typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
    class Vector2 {
    private:
        T _x;
        T _y;
    public:
        static Vector2 Zero() { return Vector2(0, 0); }
        static Vector2 One() { return Vector2(1, 1); }
        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }

        static Vector2 AngleMagnitude(T angleRadians, T magnitude) {
            return Vector2(std::cos(angleRadians) * magnitude, std::sin(angleRadians) * magnitude);
        }

        Vector2() : _x(0), _y(0) {}
        Vector2(T const & x, T const & y) : _x(x), _y(y) {}
        Vector2(Vector2 const & vec) : _x(vec._x), _y(vec._y) {}
        Vector2& operator=(Vector2 const & vec) {
            _x = vec._x;
            _y = vec._y;
            return *this;
        }

        T x() const {return _x;}
        T y() const {return _y;}

        void set(T x, T y) {
            _x = x;
            _y = y;
        }

        T magnitude() const {
            return std::hypot(_x, _y);
        }

        Vector2 normalized() const {
            const T l = magnitude();
            if (l > 0) {
                const T inv_l = 1 / l;
                return Vector2(_x * inv_l, _y * inv_l);
            }
            else {
                return Zero();
            }
        }

        Vector2 tangent() const {
            return Vector2(-_y, _x);
        }

        T angle() const {
            return std::atan2(_y, _x);
        }

        void normalize() {
            *this = this->normalized();
        }

        void translate(T dx, T dy) {
            _x += dx;
            _y += dy;
        }

        void translate(Vector2 const& delta) {
            translate(delta._x, delta._y);
        }

        Vector2 operator+(Vector2 const & v) const {
            return Vector2(_x + v._x, _y + v._y);
        }

        Vector2 operator-(Vector2 const & v) const {
            return Vector2(_x - v._x, _y - v._y);
        }

        Vector2 operator*(T scale) const {
            return Vector2(_x * scale, _y * scale);
        }

        T distance_to(Vector2 const & v) const {
            return (v - *this).magnitude();
        }

        T dot(Vector2 const & v) const {
            return _x * v._x + _y * v._y;
        }

        T scalar_projection(Vector2 const & v) const {
            return dot(v.normalized());
        }

        Vector2 projection(Vector2 const & v) const {
            const Vector2 vn = v.normalized();
            return vn * dot(vn);
        }

        Vector2 reflect(Vector2 const & normal) const {
            const T dot_prod = dot(normal);
            const Vector2 proj = normal * dot_prod;
            return Vector2(2 * proj._x - _x, 2 * proj._y - _y);
        }

        Vector2 rotate(T radians) const {
            return Vector2::AngleMagnitude(angle() + radians, magnitude());
        }
    };
}