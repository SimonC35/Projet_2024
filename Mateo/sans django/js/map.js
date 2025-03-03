// Configuration de la carte
let map = L.map('map').setView([48.7324, -3.4373], 13);
let markers = [];
let polyline = L.polyline([], {color: 'red', weight: 3}).addTo(map);
let markerClusterGroup = L.markerClusterGroup();
let updateInterval = 300000; // 5 minutes
let allPoints = []; // Stockage de tous les points
let measureMode = false;
let measurePoints = [];
let measureLayer = L.layerGroup().addTo(map);

// Fonds de carte
const basemaps = {
    'OpenStreetMap': L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: ' OpenStreetMap contributors'
    }),
    'Satellite': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
        attribution: ' Esri'
    }),
    'Terrain': L.tileLayer('https://stamen-tiles-{s}.a.ssl.fastly.net/terrain/{z}/{x}/{y}.jpg', {
        attribution: ' Stamen Design'
    })
};

// Ajouter le fond de carte par défaut
basemaps['OpenStreetMap'].addTo(map);

// Ajouter le contrôle des fonds de carte
L.control.layers(basemaps).addTo(map);

// Ajouter le groupe de clusters à la carte
map.addLayer(markerClusterGroup);

// Fonction pour formater une date
function formatDate(date) {
    return new Date(date).toLocaleString('fr-FR', {
        day: '2-digit',
        month: '2-digit',
        year: 'numeric',
        hour: '2-digit',
        minute: '2-digit'
    });
}

// Fonction pour nettoyer les marqueurs
function clearMarkers() {
    markerClusterGroup.clearLayers();
    markers = [];
    map.removeLayer(polyline);
    polyline = L.polyline([], {color: 'red', weight: 3}).addTo(map);
    measureLayer.clearLayers();
    measurePoints = [];
}

// Fonction pour calculer la distance
function calculateDistance(point1, point2) {
    return map.distance(
        [point1[1], point1[0]],
        [point2[1], point2[0]]
    );
}

// Fonction pour formater la distance
function formatDistance(meters) {
    return meters >= 1000 
        ? `${(meters/1000).toFixed(2)} km`
        : `${Math.round(meters)} m`;
}

// Fonction pour mettre à jour les statistiques
function updateStatistics(points) {
    if (points.length === 0) return;

    // Calcul de la distance totale
    let totalDistance = 0;
    for (let i = 1; i < points.length; i++) {
        totalDistance += calculateDistance(
            points[i-1].location.coordinates,
            points[i].location.coordinates
        );
    }

    // Calcul de la durée
    const startTime = new Date(points[0].date);
    const endTime = new Date(points[points.length - 1].date);
    const duration = (endTime - startTime) / 1000; // en secondes

    // Calcul de la vitesse moyenne
    const avgSpeed = totalDistance / duration * 3.6; // en km/h

    // Mise à jour de l'affichage
    document.getElementById('total-distance').textContent = formatDistance(totalDistance);
    document.getElementById('avg-speed').textContent = `${avgSpeed.toFixed(1)} km/h`;
    document.getElementById('total-duration').textContent = `${Math.round(duration/60)} min`;
    document.getElementById('point-count').textContent = points.length;
}

// Fonction pour filtrer par date
function applyDateFilter() {
    const startDate = document.getElementById('date-start').value;
    const endDate = document.getElementById('date-end').value;

    if (!startDate && !endDate) {
        updateMap(allPoints);
        return;
    }

    const filteredPoints = allPoints.filter(point => {
        const pointDate = new Date(point.date);
        if (startDate && endDate) {
            return pointDate >= new Date(startDate) && pointDate <= new Date(endDate);
        } else if (startDate) {
            return pointDate >= new Date(startDate);
        } else {
            return pointDate <= new Date(endDate);
        }
    });

    updateMap(filteredPoints);
}

// Fonction pour exporter en GeoJSON
function exportGeoJSON() {
    const geojson = {
        type: 'FeatureCollection',
        features: allPoints.map(point => ({
            type: 'Feature',
            geometry: point.location,
            properties: {
                date: point.date,
                sats: point.sats,
                hdop: point.hdop,
                speed: point.speed
            }
        }))
    };

    const blob = new Blob([JSON.stringify(geojson, null, 2)], {type: 'application/json'});
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'gps_points.geojson';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}

// Fonction pour mettre à jour la carte
function updateMap(points) {
    clearMarkers();
    
    if (points.length === 0) return;

    const coordinates = points.map(point => {
        const coords = point.location.coordinates;
        const timestamp = new Date(point.date);
        
        // Créer le marqueur
        let marker = L.marker([coords[1], coords[0]])
            .bindPopup(`
                <div class="popup-content">
                    <h3>Position GPS</h3>
                    <table>
                        <tr><td>Date:</td><td>${formatDate(timestamp)}</td></tr>
                        <tr><td>Latitude:</td><td>${coords[1].toFixed(6)}°</td></tr>
                        <tr><td>Longitude:</td><td>${coords[0].toFixed(6)}°</td></tr>
                        ${point.sats ? `<tr><td>Satellites:</td><td>${point.sats}</td></tr>` : ''}
                        ${point.hdop ? `<tr><td>HDOP:</td><td>${point.hdop}</td></tr>` : ''}
                        ${point.speed ? `<tr><td>Vitesse:</td><td>${point.speed} km/h</td></tr>` : ''}
                    </table>
                </div>
            `);
        
        markerClusterGroup.addLayer(marker);
        markers.push(marker);
        
        return [coords[1], coords[0]];
    });
    
    // Mettre à jour la polyligne
    polyline.setLatLngs(coordinates);
    
    // Ajuster la vue
    map.fitBounds(polyline.getBounds(), {padding: [50, 50]});
    
    // Mettre à jour les statistiques
    updateStatistics(points);
}

// Données par défaut si aucune donnée n'est récupérée
const DEFAULT_POINTS = [
    {
        location: { type: 'Point', coordinates: [-3.4373, 48.7324] },
        date: new Date().toISOString(),
        sats: null,
        hdop: null,
        speed: null
    }
];

// Fonction pour charger les points GPS
function loadLatestPoints() {
    fetch('http://127.0.0.1:8000/api/gps-data/')
        .then(response => {
            if (!response.ok) {
                throw new Error('Réponse réseau incorrecte');
            }
            return response.json();
        })
        .then(data => {
            // Si des données sont reçues, les utiliser
            allPoints = data.length > 0 
                ? data.sort((a, b) => new Date(a.date) - new Date(b.date))
                : DEFAULT_POINTS;
            updateMap(allPoints);
        })
        .catch(error => {
            console.warn('Erreur lors du chargement des points:', error);
            // En cas d'erreur, utiliser les points par défaut
            allPoints = DEFAULT_POINTS;
            updateMap(allPoints);
        });
}

// Fonction pour afficher une erreur
function showError(message) {
    const errorDiv = document.createElement('div');
    errorDiv.className = 'error-message';
    errorDiv.textContent = message;
    document.body.appendChild(errorDiv);
    setTimeout(() => errorDiv.remove(), 5000);
}

// Gestion des mesures
function handleMeasureClick(e) {
    const point = [e.latlng.lng, e.latlng.lat];
    measurePoints.push(point);
    
    if (measurePoints.length === 1) {
        L.circleMarker([point[1], point[0]], {
            color: '#2196F3',
            radius: 6,
            fillOpacity: 0.7
        }).addTo(measureLayer);
    } else {
        // Ajouter un marqueur au point
        L.circleMarker([point[1], point[0]], {
            color: '#2196F3',
            radius: 6,
            fillOpacity: 0.7
        }).addTo(measureLayer);
        
        // Tracer une ligne entre les deux derniers points
        const lastPoint = measurePoints[measurePoints.length - 2];
        L.polyline([
            [lastPoint[1], lastPoint[0]],
            [point[1], point[0]]
        ], {
            color: '#2196F3',
            weight: 2,
            dashArray: '5,10'
        }).addTo(measureLayer);
        
        // Calculer et afficher la distance totale
        let totalDistance = 0;
        for (let i = 1; i < measurePoints.length; i++) {
            totalDistance += calculateDistance(measurePoints[i-1], measurePoints[i]);
        }
        
        document.getElementById('measure-result').textContent = 
            `Distance totale: ${formatDistance(totalDistance)}`;
    }
}

function toggleMeasureMode() {
    measureMode = !measureMode;
    const button = document.getElementById('measure-button');
    const result = document.getElementById('measure-result');
    
    if (measureMode) {
        button.classList.add('active');
        button.textContent = 'Arrêter la mesure';
        result.textContent = 'Cliquez sur la carte pour mesurer une distance';
        map.on('click', handleMeasureClick);
    } else {
        button.classList.remove('active');
        button.textContent = 'Mesurer une Distance';
        result.textContent = '';
        map.off('click', handleMeasureClick);
        measureLayer.clearLayers();
        measurePoints = [];
    }
}

// Initialisation
document.addEventListener('DOMContentLoaded', () => {
    document.getElementById('measure-button').addEventListener('click', toggleMeasureMode);
    loadLatestPoints();
    setInterval(loadLatestPoints, updateInterval);
});
