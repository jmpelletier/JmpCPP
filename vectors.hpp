/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <type_traits>
#include <cmath>
#include <random>
#include <optional>

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

        // Creates a vector with the given angle in radians and magnitude.
        static Vector2 AngleMagnitude(T angleRadians, T magnitude) {
            return Vector2(std::cos(angleRadians) * magnitude, std::sin(angleRadians) * magnitude);
        }

        // Generate a vector pointing in a random direction with a fixed length.
        // Usage:
        //
        // std::random_device random_device;
        // std::minstd_rand0 generator(random_device());
        // std::uniform_real_distribution<sample_t> distribution (-M_PI, M_PI);
        // Vector2<float> random_vector = Vector2<float>::Random(generator, distribution);
        // 
        // Using a normal distribution:
        //
        // float angle_radians = 1.2f;
        // float angle_std_dev = 0.3f;
        // std::random_device random_device;
        // std::minstd_rand0 generator(random_device());
        // std::normal_distribution<sample_t> distribution {angle_radians, angle_std_dev};
        // Vector2<float> random_vector = Vector2<float>::Random(generator, distribution);
        //
        template <class Generator, class Distribution>
        static Vector2 Random(Generator & generator, Distribution & distribution, T length = 1) {
            const T theta = distribution(generator);
            return Vector2(std::cos(theta) * length, std::sin(theta) * length);
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

        Vector2 translate(T dx, T dy) const {
            return translate(Vector2(dx, dy));
        }

        Vector2 translate(Vector2 const& delta) const {
            return *this + delta;
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

        Vector2 project_on(Vector2 const & v) const {
            return projection(v);
        }

        Vector2 reflect(Vector2 const & normal) const {
            const T dot_prod = dot(normal);
            const Vector2 proj = normal * dot_prod * 2;
            return *this - proj;
        }

        Vector2 rotate(T radians) const {
            return Vector2::AngleMagnitude(angle() + radians, magnitude());
        }

        bool isNaN() const {
            return std::isnan(_x) || std::isnan(_y);
        }
    };

    template <typename T>
    std::ostream& operator << ( std::ostream& outs, const Vector2<T> & v)
    {
        return outs << "(" << v.x() << "," << v.y() << ")";
    }

    template <typename T>
    class Ray2 {
    private:
        Vector2<T> _position;
        Vector2<T> _direction;
        T _length;
    public:
        Ray2() : _position(), _direction(1, 0), _length(0) {}
        Ray2(Vector2<T> const & position, Vector2<T> const & direction) : _position(position), _direction(direction), _length(0) {}

        const Vector2<T>& position() const { return _position; }
        const Vector2<T>& direction() const { return _direction; }
        T length() const { return _length; }

        void move(T distance) {
            _position = _position.translate(_direction * distance);
            _length += distance;
        }

        // Move ray to target position. Both position and direction are updated.
        void move_to(Vector2<T> const & target) {
            Vector2<T> delta = target - _position;
            _direction = target.normalized();
            _position = target;
            _length += delta.magnitude();
        }

        void reflect(Vector2<T> const & normal) {
            _direction = _direction.reflect(normal);
        }

        // Returns the closest point on a circle that intersects the ray.
        // Returns an empty std::optional if there is no intersection.
        std::optional<Vector2<T>> intersect_circle(Vector2<T> const & origin, T radius) const {
            Vector2<T> U = origin - _position;
            Vector2<T> U1 = U.project_on(_direction);
            Vector2<T> U2 = U - U1;
            T d = U2.magnitude();
            
            if (d > radius) {
                return {};
            }

            T m = std::sqrt(radius * radius - d * d);
            Vector2<T> p = _position + U1 + _direction * m;

            return p;
        }
    };
}