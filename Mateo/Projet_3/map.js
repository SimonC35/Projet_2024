// Configuration de la carte
let map = L.map('map').setView([48.7324, -3.4373], 13); // Centré sur Lannion
let markers = [];
let isTracking = false;
let trackingInterval;
let allGpsData = [];
const MAX_POINTS = 30; // Nombre maximum de points à afficher

// Ajouter la couche OpenStreetMap
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: ' OpenStreetMap contributors'
}).addTo(map);

// Fonction pour récupérer et afficher les données GPS
async function fetchAndDisplayGPSData(isInitial = false) {
    try {
        // URL avec paramètre pour différencier le chargement initial
        const url = `http://127.0.0.1:8000/api/gps-data/?initial=${isInitial}`;
        console.log('Fetching data from:', url);

        // Récupérer les données GPS
        const response = await fetch(url, {
            method: 'GET',
            headers: {
                'Accept': 'application/json',
            },
            mode: 'cors'
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const newData = await response.json();
        console.log('Received data:', newData);

        if (newData && newData.length > 0) {
            if (isInitial) {
                // Pour le chargement initial, remplacer toutes les données
                allGpsData = newData;
            } else {
                // Pour les mises à jour, ajouter le nouveau point
                allGpsData.push(newData[0]);
                // Garder seulement les 30 derniers points
                if (allGpsData.length > MAX_POINTS) {
                    allGpsData = allGpsData.slice(-MAX_POINTS);
                }
            }
            
            console.log('Total points tracked:', allGpsData.length);
            
            // Effacer tous les marqueurs existants
            clearAllMarkers();

            // Créer un tableau pour la ligne du tracé
            const trackPoints = allGpsData.map(data => [
                data.location.coordinates[1], // Latitude
                data.location.coordinates[0]  // Longitude
            ]);

            // Dessiner la ligne du tracé
            if (window.currentTrack) {
                map.removeLayer(window.currentTrack);
            }
            window.currentTrack = L.polyline(trackPoints, {
                color: '#3498db',
                weight: 3,
                opacity: 0.8
            }).addTo(map);

            // Ajouter les marqueurs pour chaque point
            allGpsData.forEach((data, index) => {
                const lat = data.location.coordinates[1];
                const lng = data.location.coordinates[0];
                const marker = L.marker([lat, lng]).addTo(map);
                
                // Créer le popup avec les informations
                const date = new Date(data.date).toLocaleString();
                marker.bindPopup(`
                    <b>Point ${allGpsData.length - index}</b><br>
                    Date: ${date}<br>
                    Latitude: ${lat}<br>
                    Longitude: ${lng}<br>
                    Vitesse: ${data.speed || 'N/A'} km/h<br>
                    HDOP: ${data.hdop || 'N/A'}<br>
                    Satellites: ${data.sats || 'N/A'}<br>
                    Cap: ${data.course || 'N/A'}°
                `);

                markers.push(marker);
            });

            // Centrer la carte sur le dernier point
            if (allGpsData.length > 0) {
                const lastPoint = allGpsData[0];
                const centerLat = lastPoint.location.coordinates[1];
                const centerLng = lastPoint.location.coordinates[0];
                map.setView([centerLat, centerLng], 13);
            }

            // Mettre à jour les statistiques
            updateStats();
        }

    } catch (error) {
        console.error('Erreur lors de la récupération des données:', error);
    }
}

// Fonction pour supprimer tous les marqueurs
function clearAllMarkers() {
    markers.forEach(marker => map.removeLayer(marker));
    markers = [];
}

// Fonction pour mettre à jour les statistiques
function updateStats() {
    if (allGpsData.length === 0) return;

    // Calculer la distance totale
    let distance = 0;
    for (let i = 1; i < allGpsData.length; i++) {
        const p1Lat = allGpsData[i-1].location.coordinates[1];
        const p1Lng = allGpsData[i-1].location.coordinates[0];
        const p2Lat = allGpsData[i].location.coordinates[1];
        const p2Lng = allGpsData[i].location.coordinates[0];

        const p1 = L.latLng(p1Lat, p1Lng);
        const p2 = L.latLng(p2Lat, p2Lng);
        distance += p1.distanceTo(p2);
    }

    // Mettre à jour les informations dans l'interface
    document.getElementById('total-distance').textContent = `${(distance / 1000).toFixed(2)} km`;
    
    const lastPoint = allGpsData[0];
    document.getElementById('current-consumption').textContent = 
        lastPoint.speed ? `${lastPoint.speed} km/h` : 'N/A';
    
    // Simuler le niveau de batterie
    const batteryLevel = Math.max(0, 100 - (distance / 1000 * 5));
    document.getElementById('estimated-battery').textContent = `${batteryLevel.toFixed(0)}%`;
    
    // Mettre à jour les informations du dispositif
    document.getElementById('device-id').textContent = 
        `Points: ${allGpsData.length} | Satellites: ${lastPoint.sats || 'N/A'} | HDOP: ${lastPoint.hdop || 'N/A'}`;
}

// Gérer le suivi automatique
document.getElementById('start-tracking').addEventListener('click', function() {
    if (isTracking) {
        // Arrêter le suivi
        clearInterval(trackingInterval);
        this.textContent = 'Démarrer le suivi';
        isTracking = false;
    } else {
        // Démarrer le suivi avec le chargement initial des 30 points
        fetchAndDisplayGPSData(true); // true pour le chargement initial
        
        // Configurer la mise à jour toutes les 5 minutes avec un seul point
        trackingInterval = setInterval(() => fetchAndDisplayGPSData(false), 300000);
        
        this.textContent = 'Arrêter le suivi';
        isTracking = true;
    }
});

// Charger les données au démarrage
fetchAndDisplayGPSData(true);
