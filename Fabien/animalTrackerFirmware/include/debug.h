/**
 * @file debug.h
 * @brief Définition de la fonction debug
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */
#ifndef DEBUG_H
#define DEBUG_H

#include "inttypes.h"

/**
 * @brief Affichage des informations de debug
 * 
 * Utilisation d'une logique de décodage basé sur les identifiants pour afficher les différentes valeurs.
 */
void printDebugInfo(uint8_t datalen, uint8_t *data);

#endif