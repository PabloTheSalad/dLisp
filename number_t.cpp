#include "types/number_t.hpp"
#include <cmath>


bool number_t::operator == (const number_t& other) { 
    return std::abs(value - other.value) < epsilon; 
}

/*!
 * \brief Уточнение типа числа
 */
void number_t::raise_type() {
    if (type != T_INT) {
        if (std::fmod(std::abs(value),10) < epsilon) type = T_INT;
    }
}

number_t number_t::operator + (const number_t& other) {
    return number_t(value + other.value);
}

number_t number_t::operator - (const number_t& other) {
    return number_t(value - other.value);
}

number_t number_t::operator * (const number_t& other) {
    return number_t(value * other.value);;
}

number_t number_t::operator / (const number_t& other) {
    return number_t(value / other.value);
}

number_t number_t::operator % (const number_t& other) {
    return number_t(std::fmod(value, other.value));
}

/*!
 * \brief Целочисленное деление чисел
 */
number_t number_t::div (const number_t& other) {
    return number_t((value - std::fmod(value, other.value)) / other.value);
}