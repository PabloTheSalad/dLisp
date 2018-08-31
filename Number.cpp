#include "types/Number.hpp"
#include <cmath>
#include <algorithm>


bool Number::operator == (const Number& other) { 
    return std::abs(value - other.value) < epsilon; 
}

/*!
 * \brief Уточнение типа числа
 */
void Number::raise_type() {
    if (std::fmod(std::abs(value),10) < epsilon) type = T_INT;
    else type = T_REAL;
}

Number Number::operator + (const Number& other) {
    return Number(value + other.value);
}

Number& Number::operator += (const Number& other) {
    value += other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator - (const Number& other) {
    return Number(value - other.value);
}

Number& Number::operator -= (const Number& other) {
    value -= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator * (const Number& other) {
    return Number(value * other.value);;
}

Number& Number::operator *= (const Number& other) {
    value *= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator / (const Number& other) {
    return Number(value / other.value);
}

Number& Number::operator /= (const Number& other) {
    value /= other.value;
    type = std::min(type, other.type);
    return *this;
}

Number Number::operator % (const Number& other) {
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
Number Number::div (const Number& other) {
    return Number((value - std::fmod(value, other.value)) / other.value);
}

Number& Number::divInplace (const Number& other) {
    value = (value - std::fmod(value, other.value)) / other.value;
    type = std::min(type, other.type);
    return *this;
}
