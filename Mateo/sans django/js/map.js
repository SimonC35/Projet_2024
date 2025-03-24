let map = L.map('map').setView([48.7324, -3.4373], 13);
let markers = [];
let polyline = L.polyline([], {color: 'purple', weight: 3}).addTo(map);
let markerClusterGroup = L.markerClusterGroup();
let allPoints = []; // Stockage de tous les points
let measureMode = false;
const updateInterval = 60000; // 5 minutes en millisecondes
const sendDataInterval = 600000; // 5 minutes en millisecondes
let measurePoints = [];
let measureLayer = L.layerGroup().addTo(map);

// Fonds de carte
const basemaps = {
  'OpenStreetMap': L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: 'OpenStreetMap contributors'
  }),
  'Satellite': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
    attribution: 'Esri'
  }),
  'Terrain': L.tileLayer('https://stamen-tiles-{s}.a.ssl.fastly.net/terrain/{z}/{x}/{y}.jpg', {
    attribution: 'Stamen Design'
  })
};

// Carte par défaut
basemaps['OpenStreetMap'].addTo(map);

// Formater une date
function formatDate(date) {
  const parsedDate = new Date(date);
  if (isNaN(parsedDate.getTime())) {
    console.error("Date invalide :", date);
    return "Date invalide";
  }
  return parsedDate.toLocaleString('fr-FR', {
    day: '2-digit',
    month: '2-digit',
    year: 'numeric',
    hour: '2-digit',
    minute: '2-digit'
  });
}

function clearMarkers() {
  markerClusterGroup.clearLayers(); // Efface tous les marqueurs
  markers = [];
  map.removeLayer(polyline); // Efface la ligne
  polyline = L.polyline([], {color: 'red', weight: 3}).addTo(map);
  measureLayer.clearLayers(); // Efface les mesures
  measurePoints = [];
}

// Formater la distance
function formatDistance(meters) {
  return meters >= 1000
    ? `${(meters / 1000).toFixed(2)} km` // > 1000 = km
    : `${Math.round(meters)} m`; // < 1000 = arrondi
}

// Calculer les statistiques
function updateStatistics(points) {
  if (!points || points.length < 2) return; // Vérification de sécurité

  // Distance totale
  let totalDistance = 0;
  for (let i = 1; i < points.length; i++) {
    const prevPoint = points[i - 1].location.coordinates;
    const currentPoint = points[i].location.coordinates;
    totalDistance += L.latLng(prevPoint[1], prevPoint[0]).distanceTo(L.latLng(currentPoint[1], currentPoint[0]));
  }

  // Durée
  const startTime = new Date(points[0].date);
  const endTime = new Date(points[points.length - 1].date);
  const duration = (endTime - startTime) / 1000;

  // Vitesse moyenne
  const avgSpeed = totalDistance / duration * 3.6;

  // Mise à jour de l'affichage
  document.getElementById('total-distance').textContent = formatDistance(totalDistance);
  document.getElementById('avg-speed').textContent = `${avgSpeed.toFixed(1)} km/h`;
  document.getElementById('total-duration').textContent = `${Math.round(duration / 60)} min`;
  document.getElementById('point-count').textContent = points.length;
}

// Exporter en GeoJSON
function exportGeoJSON(points) {
  const geojson = {
    type: 'FeatureCollection',
    features: points.map(point => ({
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

  const blob = new Blob([JSON.stringify(geojson)], { type: 'application/json' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.href = url;
  link.download = 'gps_points.geojson';
  link.click();
  URL.revokeObjectURL(url);
}

// Mettre à jour la carte
function updateMap(points) {
  console.log('Mise à jour de la carte avec les points:', points);
  clearMarkers();
  if (points.length === 0) {
    console.log('Aucun point à afficher');
    return;
  }

  // Limiter le nombre de points affichés à 30
  const limitedPoints = points.slice(-30);

  const coordinates = limitedPoints.map(point => {
    const coords = point.location.coordinates; // Erreur potentielle
    console.log('Coordonnées extraites:', coords);
    const timestamp = new Date(point.date);

    // Créer un marqueur
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

    // Vérifier et ajouter le marqueur
    if (!map.hasLayer(markerClusterGroup)) {
      console.log('markerClusterGroup non ajouté à la carte, ajout en cours...');
      map.addLayer(markerClusterGroup);
    }
    if (!markerClusterGroup.hasLayer(marker)) {
      // Ajouter le marqueur au groupe
      markerClusterGroup.addLayer(marker);
    }
  });

  // Exporter en GeoJSON à chaque mise à jour
  exportGeoJSON(points);
}

// Charger le dernier point depuis l'API
async function loadLatestPoints() {
  const url = 'http://172.20.14.7:8000/api/gps-data/last/'; // Récupérer initialement le dernier point
  try {
    const response = await fetch(url, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      }
    });
    if (!response.ok) {
      throw new Error(`Erreur HTTP : ${response.status}`);
    }
    const data = await response.json();
    console.log('Données reçues de l\'API:', data); // Log des données reçues
    if (data.length > 0) {
      updateMap(data); // Mettre à jour la carte avec les 30 points
    } else {
      console.log('Aucun point reçu');
    }
  } catch (error) {
    console.error('Erreur lors de la récupération des données:', error);
  }
}

// Ajouter un nouveau point à la carte
async function addNewPoint() {
  const url = 'http://172.20.14.7:8000/api/gps-data/last/1/';
  try {
    const response = await fetch(url, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      }
    });
    if (!response.ok) {
      throw new Error(`Erreur HTTP : ${response.status}`);
    }
    const data = await response.json();
    if (data.length > 0) {
      const newPoint = data[0];
      markers.push(newPoint); // Ajouter le nouveau point à la liste des marqueurs
      updateMap(markers); // Mettre à jour la carte avec le nouveau point
    }
  } catch (error) {
    console.error('Erreur lors de la récupération du nouveau point:', error);
  }
}

// Envoyer les données à uMap
async function sendDataToUmap(points) {
  const mapId = '1189050'; 
  const dataLayerId = '16'; 
  const url = `https://umap.openstreetmap.fr/en/map/${mapId}/datalayer/${dataLayerId}/`;

  const geojson = {
    type: 'FeatureCollection',
    features: points.map(point => ({
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

  console.log('Données à envoyer à uMap:', geojson);

  try {
    const response = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(geojson),
    });
    if (!response.ok) {
      throw new Error(`Erreur HTTP : ${response.status}`);
    }
    console.log('Données envoyées avec succès à uMap');
  } catch (error) {
    console.error('Erreur lors de l\'envoi des données à uMap:', error);
  }
}

// Chargement initial des points
loadLatestPoints();

// Recharger les points toutes les 5 minutes
setInterval(addNewPoint, updateInterval);

// Envoyer les données à uMap toutes les 5 minutes
setInterval(() => {
  if (allPoints.length >= 5) {
    sendDataToUmap(allPoints);
  } else {
    console.log('Attente de 5 points avant d\'envoyer les données.');
  }
}, sendDataInterval);

// Initialisation de l'app
document.addEventListener('DOMContentLoaded', () => {
  const measureButton = document.getElementById('measure-button');
  const clearPointsButton = document.getElementById('clearPoints');
  if (clearPointsButton) {
    clearPointsButton.addEventListener('click', clearMarkers);
  } else {
    console.error("Le bouton 'Effacer tous les points' n'a pas été trouvé.");
  }
});

// Gestion de l'ajout manuel de points
document.getElementById('add-point-form').addEventListener('submit', function(e) {
    e.preventDefault();

    const latitude = parseFloat(document.getElementById('latitude').value);
    const longitude = parseFloat(document.getElementById('longitude').value);

    if (isNaN(latitude) || isNaN(longitude)) {
        alert('Veuillez entrer des coordonnées valides');
        return;
    }

    const newPoint = {
        location: { type: 'Point', coordinates: [longitude, latitude] },
        date: new Date().toISOString()
    };

    // Ajouter le point à la carte
    L.marker([latitude, longitude]).addTo(map);

    // Ajouter le point à la liste des points
    allPoints.push(newPoint);

    // Réinitialiser le formulaire
    document.getElementById('add-point-form').reset();
});