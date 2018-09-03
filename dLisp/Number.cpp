#include "types/Number.hpp"
#include <cmath>
#include <algorithm>


bool Number::operator == (const Number& other) const {
    return std::abs(value - other.value) < epsilon; 
}

Number Number::operator + (const Number& other) const {
    return Number(value + other.value);
}

Number& Number::operator += (const Number& other) {
    value += other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator - (const Number& other) const {
    return Number(value - other.value);
}

Number& Number::operator -= (const Number& other) {
    value -= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator * (const Number& other) const {
    return Number(value * other.value);;
}

Number& Number::operator *= (const Number& other) {
    value *= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator / (const Number& other) const {
    return Number(value / other.value);
}

Number& Number::operator /= (const Number& other) {
    value /= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator % (const Number& other) const {
    return Number(std::fmod(value, other.value));
}

Number& Number::operator %= (const Number& other) {
    value = std::fmod(value, other.value);
    type = std::min(type, other.type);
    return *this;
}

/*!
 * \brief Целочисленное деление чисел
 */
Number Number::div (const Number& other) const {
    return Number((value - std::fmod(value, other.value)) / other.value);
}

Number& Number::divInplace (const Number& other) {
    value = (value - std::fmod(value, other.value)) / other.value;
    type = std::min(type, other.type);
    return *this;
}
