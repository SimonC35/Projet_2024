/**
 * @file gps.cpp
 * @brief Fichier de gestion de la logique du GPS.
 * 
 * Contient les fonctions concernant le GPS.
 * 
 * @author Fabien
 * @date   2025-04
 */

#include "gps.h"

/**
 * Création d'une instance de la classe Air530ZClass nommée GPS,
 * utilisée pour interagir avec le module GPS Air530Z.
 */
Air530ZClass GPS;

/**
 * Variable pour stocker la latitude précédente de localisation GPS.
 */
double previousLatitude = 0.0;

/**
 * Variable pour stocker la longitude précédente de localisation GPS.
 */
double previousLongitude = 0.0;

/**
 * @brief Lit les variables fournies par le module GPS et les stocke sous forme d'octets
 * 
 * En fonction des flags de compilation présent, cette fonction ajoute dans le tableau passé en paramètre 
 * les valeurs fournies par le module GPS.
 * 
 * Par souci de performance, trois variables temporaires de taille 1, 2 et 4 octets sont allouées 
 * et réutilisées selon la taille des données à traiter. Les valeurs sont transformées pour conserver 
 * uniquement la précision utile et éventuellement les rééchelonnées.
 * 
 * @note Cette fonction est appelée lorsque le système est dans l'état STATE_SEND_THRESHOLD_EXCEEDED.
 */

void readGPSStoreAsBytes(uint8_t *fullArray)
{

    uint8_t *ptr = fullArray; // ptr est un pointeur de type octet non signé (uint8_t) qui pointe vers l'adresse de début de fullArray

    /**
     * Variable 1 octet non signé pour traitement temporaire
     */
    uint8_t tempUint8_t = 0;
    /**
     * Variable 2 octets non signés pour traitement temporaire
     */
    uint16_t tempUint16_t = 0;
    /**
     * Variable 4 octets non signés pour traitement temporaire
     */    
    uint32_t tempUint32_t = 0;
    /**
     * Variable 4 octets signées pour traitement temporaire
     */    
    int32_t tempInt32_t = 0;

#ifdef GPS_COORDS
    if (GPS.location.isValid()) { // Si les coordonnées sont valides, le traitement est effectué
        tempUint8_t = 0x01;       // Ajout de l'identifiant 1 correspondant aux coordonnées GPS (latitude, longitude)
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t)); // Copie vers ptr de l'identifiant
        ptr += sizeof(uint8_t); // Avancer le pointeur pour préparer la copie des autres données (memcpy ne modifie pas la position du pointeur il faut donc incrémenter)

        tempInt32_t = (int32_t)(GPS.location.lat() * digitPrecision5);
        /** 
         * Conversion de la latitude : 
         * Multiplication par 100'000 pour conserver une précision de 5 chiffres après la virgule
         * Cast en entier signée
         */ 
        memcpy(ptr, &tempInt32_t, sizeof(int32_t)); // Copie de la latitude convertie vers ptr sur 4 octets
        ptr += sizeof(int32_t); // Avancer le pointeur pour préparer la copie des autres données 

#ifdef DEBUG
        Serial.printf("GPS Coordinates added: Lat=%d", tempInt32_t);
#endif

        // Opération similaire à la latitude
        tempInt32_t = (int32_t)(GPS.location.lng() * digitPrecision5);
        memcpy(ptr, &tempInt32_t, sizeof(int32_t));
        ptr += sizeof(int32_t);

#ifdef DEBUG
        Serial.printf(" Lng=%d\n", tempInt32_t);
#endif
    }
#endif

#ifdef GPS_ALT
    if (GPS.altitude.isValid()) {
        tempUint8_t = 0x02; // Identifiant 2 pour l'altitude
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.altitude.meters() >= 65535.0) tempUint16_t = 65535; // Dans le cas ou l'altitude dépasse la taille de 2 octets non signés (65 535m)
                                                                    // celle-ci est mise a 65535
        else if (GPS.altitude.meters() <= 0) tempUint16_t = 0;      // Si l'altitude fournie est négative l'altitude est mise à 0
        else {
            tempUint16_t = (uint16_t)(GPS.altitude.meters()); // Dans le cas ou l'altitude est entre les bornes 65 535 et 0 on l'encode correctement
        }

        memcpy(ptr, &tempUint16_t, sizeof(uint16_t)); // Copie vers le pointeur
        ptr += sizeof(uint16_t); // Avancer le pointeur

#ifdef DEBUG
        Serial.printf("Altitude added: %d meters\n", tempUint16_t);
#endif
    }
#endif

#ifdef GPS_HDOP
    if (GPS.hdop.isValid()) {
        tempUint8_t = 0x03; // Identifiant 3 pour le HDOP
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);
        if (GPS.hdop.value() > 2550) tempUint8_t = 255; // Borne 255 
        else if (GPS.hdop.value() <= 0) tempUint8_t = 0;
        else {
            tempUint8_t = (uint8_t)(GPS.hdop.value() / digitPrecision1); // Division par 10 pour une précision de 1 chiffre après la virgule.
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("HDOP added: %d\n", tempUint8_t);
#endif
    }
#endif

#ifdef GPS_SPEED
    if (GPS.speed.isValid()) {
        tempUint8_t = 0x04; // Identifiant 4 pour la vitesse
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.speed.mps() >= 255) tempUint8_t = 255;
        else if (GPS.speed.mps() <= 0) tempUint8_t = 0;
        else {
            tempUint8_t = (uint8_t)(GPS.speed.mps() * digitPrecision1); // Précision conservé respectant la précision fournie par la documentation
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Speed added: %d m/s\n", tempUint8_t);
#endif
    }
#endif

#ifdef GPS_COURSE
    if (GPS.course.isValid()) {
        tempUint8_t = 0x05; // Identifiant 5 pour le cap
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.course.deg() >= 360)
        {
            tempUint8_t = 255;
        } else if (GPS.course.deg() < 0) tempUint8_t = 0;
        else { tempUint8_t = (uint8_t) (((GPS.course.deg() * 255) / 360)); } // Réechelonnage des degréees du cap de [0,360] vers [0,255]
             
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Course added: %d degrees\n", tempUint8_t);
#endif
    }
#endif

#ifdef GPS_SATS
    if (GPS.satellites.isValid()) {
        tempUint8_t = 0x06; // Identifiant 6 pour le nombre de sattelite
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.satellites.value() >= 255) tempUint8_t = 255;
        else if (GPS.satellites.value() <= 0) {
            tempUint8_t = 0;
        } else {
            tempUint8_t = (uint8_t) GPS.satellites.value();
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Satellites added: %d\n", tempUint8_t);
#endif
    }
#endif

#ifdef DEBUG
    printDebugInfo(ptr - fullArray, fullArray); // ptr = adresse de fin. fullArray = adresse de début. L'opération : fin - début donne la taille
#endif
}

/**
 * @brief Lit les variables fournies par le module GPS et les stocke sous forme d'octets
 * 
 * Cette fonction réalise l'acquisition des données du GPS, c'est à dire l'allumage de celui-ci
 * 
 * @note Cette fonction est appelée lorsque le système est dans l'état STATE_GPS_ACQUIRE.
 */
void gpsAcquire()
{
    GPS.begin(); // Démarrage du GPS, mise en tension via les broches nécessaire, initialisation baudrate

    #ifdef DEBUG
        Serial.println("DEBUG: Acquiring GPS...");
    #endif

    uint32_t start = millis(); // Stockage du temps de départ, millisecondes depuis le lancement de la carte.

    while ((millis() - start) < GPS_UPDATE_TIMEOUT) { // Timer simple. millis() (temps actuel) - start donne le temps écoulé dans le contexte du timer.
        // Si ce temps viens à dépasser GPS_UPDATE_TIME alors sortie de boucle

        while (GPS.available()) GPS.encode(GPS.read());
            // Tant que des données sont disponibles dans le flux GPS, on lit et on décode.
            // GPS.read() lit le flux série NMEA fournit par le GPS
            // GPS.encode() interprète pour mettre à jour des attributs interne à l'objet GPS

  
        if (GPS.location.age() < 1000) break; // Si la position GPS est récente, on considère qu'elle est présente, sortie de la boucle
    }

    if (!GPS.location.isValid()) {  // Si la position est invalide, passage de la carte dans l'état STATE_SLEEP et sortie de la fonction
    #ifdef DEBUG
            Serial.println("DEBUG: Invalid GPS. Skipping send.");
    #endif
        mcuStatus = STATE_SLEEP;
        return;
    }

    double distance = GPS.distanceBetween(previousLatitude, previousLongitude, GPS.location.lat(), GPS.location.lng()); // Calcul de la distance parcourue

    #ifdef DEBUG
        Serial.printf("Previous: %.6f / %.6f\n", previousLatitude, previousLongitude);
        Serial.printf("Current : %.6f / %.6f\n", GPS.location.lat(), GPS.location.lng()); 
        Serial.printf("Distance: %.2f meters\n", distance);
    #endif

    if (DISTANCE_THRESHOLD == 0.0 || distance > DISTANCE_THRESHOLD) { // Si la distance parcourue est supérieur au seuil déclaré dans
        // config.h (DISTANCE_THRESHOLD) alors on passe la carte dans l'état STATE_SEND_THRESHOLD_EXCEEDED
        mcuStatus = STATE_SEND_THRESHOLD_EXCEEDED;
    } else { // Dans le cas ou la distance parcourue n'est pas suffisante, on passe la carte dans l'état STATE_SLEEP
    #ifdef DEBUG
            Serial.println("DEBUG: No significant movement.");
    #endif
        mcuStatus = STATE_SLEEP;
    }
}
