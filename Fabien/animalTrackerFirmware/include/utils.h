/**
 * @file utils.h
 * @brief Divers et macros.
 * @author Fabien
 * @date 05 2025
 * @version 1.0
 * 
 * Ce fichier contient des macros utiles pour faciliter le développement
 * 
 */
#ifndef UTILS_H
#define UTILS_H

#include "inttypes.h"

/**
 * Constante pour la multiplication par 100'000, utile pour conserver une précision de 5 chiffres après la virgule
 */
constexpr uint32_t digitPrecision5 = 100'000;
constexpr uint32_t digitPrecision6 = 1'000'000;
constexpr uint8_t digitPrecision2 = 100;
constexpr uint8_t digitPrecision1 = 10;

#endif