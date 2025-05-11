/**
 * @file main.h
 * @brief En-tête principal du projet Animal Connecté.
 * 
 * Ce fichier regroupe les inclusions nécessaires à l'exécution du programme principal.
 * Il centralise les dépendances logicielles et matérielles utilisées dans le fichier main.cpp.
 * 
 * @author Prénom NOM
 * @date 04 2025
 * @version 1.0
 */

#ifndef MAIN_H
#define MAIN_H

// Librairies système et LoRa
#include "Arduino.h"
#include "LoRaWanMinimal_APP.h"

// Modules internes du projet
#include "gps.h"              ///< Gestion du module GPS
#include "powerManagement.h"  ///< Fonctions liées à l'économie d'énergie
#include "loraWAN.h"          ///< Configuration et communication LoRaWAN
#include "dataSender.h"       ///< Encodage et envoi des données
#include "config.h"           ///< Paramètres de configuration du système
#include "debug.h"            ///< Macros de débogage
#include "utils.h"            ///< Fonctions utilitaires
#include "secret.h"           ///< Clés et identifiants privés (non versionnés)

#endif
