#include "flagSettings.h"

// CONFIG pour ID ;LAT,LNG ; ALT 
#if defined(ENABLE_COORDS) && defined(ENABLE_ALT) && defined(ENABLE_TIME)
  uint8_t flagSettings = FLAG_coords | FLAG_alt | FLAG_time;
#elif defined(ENABLE_COORDS) && defined(ENABLE_ALT)
  uint8_t flagSettings = FLAG_coords | FLAG_alt;
#elif defined(ENABLE_COORDS)
  uint8_t flagSettings = FLAG_coords;
#elif defined(ENABLE_ALT)
  uint8_t flagSettings = FLAG_alt;
#else
  uint8_t flagSettings = 0; // Aucun flag activé
#endif
