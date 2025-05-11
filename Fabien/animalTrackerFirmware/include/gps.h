#ifndef GPS_H
#define GPS_H

#include "GPS_Air530Z.h"
#include "utils.h"
#include "debug.h"
#include "config.h"
#include "powerManagement.h"

extern Air530ZClass GPS;

extern double previousLatitude;
extern double previousLongitude;

void readGPSStoreAsBytes(uint8_t *fullArray);

/**
 * @brief Lit les données du GPS, applique un statut de carte selon le dépassement ou non du seuil.
 * 
 * Analyse la validité de la position GPS actuelle. Si un fix est détecté,
 * la position est stockée, et le système passe à l'état d'envoi. Sinon, 
 * il reste en acquisition pour retenter à la prochaine itération.
 * 
 * @note Cette fonction modifie la variable globale mcu_status.
 * @pre GPS.begin() doit avoir été appelé dans setup().
 * 
 * @post 
 * - Si GPS.location.isValid() est vrai, mcu_status devient STATE_SEND_THRESHOLD_EXCEEDED.
 * - Sinon, mcu_status reste ou redevient STATE_GPS_ACQUIRE.
 */
void gpsAcquire();

#endif