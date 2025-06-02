/**
 * @file config.h
 * @brief Configuration globale du système Animal Connecté.
 * 
 * Définit les constantes utilisées pour la gestion du GPS, 
 * les délais de fonctionnement, les seuils de distance, 
 * et le calcul conditionnel de la taille du payload à transmettre.
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

/**
 * Temps en ms de timeout GPS
 */
constexpr uint32_t GPS_UPDATE_TIMEOUT = 90000;

/**
 * Temps de sommeil du dispositif en ms
 */
constexpr uint32_t LPM_SLEEP_TIME = 90000; 

/**
 * Temps entre les réessai d'envoi de données à TTN
 */
constexpr uint16_t TTN_JOIN_FAIL_WAIT = 5000;

constexpr float DISTANCE_THRESHOLD = 0.0;

/**
 * Nombre de tentative de réessai en cas d'échec d'envoi des données à TTN
 */
constexpr uint16_t RETRY_COUNT = 5;

#define BASE_SIZE 1 


/**
 * Structure qui en fonction des flags présent définit une taille en constante des données
 */
#ifdef GPS_COORDS
constexpr uint8_t GPS_COORDS_SIZE = 9; // Si le flag coordonnées est présent 9 octets sont nécessaire pour stocker identifiant + 8 octets de données
#else
constexpr uint8_t GPS_COORDS_SIZE = 0; // Si le flag est non présent, aucun octets
#endif

#ifdef GPS_ALT
constexpr uint8_t GPS_ALT_SIZE = 3; 
#else
constexpr uint8_t GPS_ALT_SIZE = 0;
#endif

#ifdef GPS_HDOP
constexpr uint8_t GPS_HDOP_SIZE = 2;
#else
constexpr uint8_t GPS_HDOP_SIZE = 0;
#endif

#ifdef GPS_SPEED
constexpr uint8_t GPS_SPEED_SIZE = 2; 
#else
constexpr uint8_t GPS_SPEED_SIZE = 0;
#endif

#ifdef GPS_COURSE
constexpr uint8_t GPS_COURSE_SIZE = 2;
#else
constexpr uint8_t GPS_COURSE_SIZE = 0;
#endif

#ifdef GPS_SATS
constexpr uint8_t GPS_SATS_SIZE = 2;
#else
constexpr uint8_t GPS_SATS_SIZE = 0;
#endif


/**
 * Calcul de la taille du payload en fonction des flags présent
 */
constexpr uint8_t PAYLOAD_SIZE_CALC =
    BASE_SIZE +
    GPS_COORDS_SIZE +
    GPS_ALT_SIZE +
    GPS_HDOP_SIZE +
    GPS_SPEED_SIZE +
    GPS_COURSE_SIZE +
    GPS_SATS_SIZE;


constexpr uint8_t PAYLOAD_SIZE = PAYLOAD_SIZE_CALC;

#endif 