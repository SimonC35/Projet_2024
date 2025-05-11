/**
 * @file config.h
 * @brief Configuration globale du système Animal Connecté.
 * 
 * Définit les constantes utilisées pour la gestion du GPS, 
 * les délais de fonctionnement, les seuils de distance, 
 * et le calcul conditionnel de la taille du payload à transmettre.
 * 
 * @author Ton Nom
 * @date Avril 2025
 * @version 1.0
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

/// Timeout maximum pour l'initialisation GPS (en ms)
constexpr uint16_t GPS_INIT_TIMEOUT = 60000; ///< 1 minute

/// Timeout d'attente de nouvelles coordonnées GPS (en ms)
constexpr uint16_t GPS_UPDATE_TIMEOUT = 90000; ///< 1.5 minutes

/// Temps de mise en veille du microcontrôleur (en ms)
constexpr uint32_t LPM_SLEEP_TIME = 181000; 

/// Délai entre deux tentatives de join LoRa (en ms)
constexpr uint16_t TTN_JOIN_FAIL_WAIT = 5000; ///< 5 secondes

/// Seuil minimal de déplacement en mètres pour déclencher un envoi
constexpr float DISTANCE_THRESHOLD = 0.0;

/// Nombre maximum de tentatives pour une action critique
constexpr uint16_t RETRY_COUNT = 5;

#define BASE_SIZE 1 ///< Taille de base minimale du payload

// -------------------- GPS Payload Details --------------------

#ifdef GPS_COORDS
constexpr uint8_t GPS_COORDS_SIZE = 9; ///< Coordonnées GPS (lat/lon)
#else
constexpr uint8_t GPS_COORDS_SIZE = 0;
#endif

#ifdef GPS_ALT
constexpr uint8_t GPS_ALT_SIZE = 3; ///< Altitude
#else
constexpr uint8_t GPS_ALT_SIZE = 0;
#endif

#ifdef GPS_HDOP
constexpr uint8_t GPS_HDOP_SIZE = 2; ///< HDOP (qualité du signal GPS)
#else
constexpr uint8_t GPS_HDOP_SIZE = 0;
#endif

#ifdef GPS_SPEED
constexpr uint8_t GPS_SPEED_SIZE = 2; ///< Vitesse
#else
constexpr uint8_t GPS_SPEED_SIZE = 0;
#endif

#ifdef GPS_COURSE
constexpr uint8_t GPS_COURSE_SIZE = 2; ///< Cap (direction)
#else
constexpr uint8_t GPS_COURSE_SIZE = 0;
#endif

#ifdef GPS_SATS
constexpr uint8_t GPS_SATS_SIZE = 2; ///< Nombre de satellites visibles
#else
constexpr uint8_t GPS_SATS_SIZE = 0;
#endif

/// Calcul automatique de la taille du payload selon les options activées
constexpr uint8_t PAYLOAD_SIZE_CALC =
    BASE_SIZE +
    GPS_COORDS_SIZE +
    GPS_ALT_SIZE +
    GPS_HDOP_SIZE +
    GPS_SPEED_SIZE +
    GPS_COURSE_SIZE +
    GPS_SATS_SIZE;

/// Taille finale du payload à transmettre via LoRa
constexpr uint8_t PAYLOAD_SIZE = PAYLOAD_SIZE_CALC;

#endif // CONFIG_H
