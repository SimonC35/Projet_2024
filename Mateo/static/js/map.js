// Initialisation de la carte
let map = L.map('map').setView([48.7324, -3.4373], 13);
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '© OpenStreetMap contributors'
}).addTo(map);

// Variables globales
let markers = {};
let paths = {};
let updateInterval = 5000; // 5 secondes

// Fonction pour charger les animaux
async function loadAnimals() {
    try {
        const response = await fetch('/api/animals');
        const animals = await response.json();
        
        animals.forEach(animal => {
            // Créer un marqueur pour chaque animal s'il n'existe pas déjà
            if (!markers[animal.id]) {
                markers[animal.id] = L.marker([0, 0]).addTo(map)
                    .bindPopup(`${animal.name} (ID: ${animal.device_id})`);
                paths[animal.id] = L.polyline([], {color: getRandomColor()}).addTo(map);
            }
            // Charger les positions de l'animal
            loadAnimalPositions(animal.id);
        });
    } catch (error) {
        console.error('Erreur lors du chargement des animaux:', error);
    }
}

// Fonction pour charger les positions d'un animal
async function loadAnimalPositions(animalId) {
    try {
        const response = await fetch(`/api/animals/${animalId}/positions`);
        const positions = await response.json();
        
        if (positions.length > 0) {
            const lastPosition = positions[0];
            
            // Mettre à jour le marqueur
            markers[animalId].setLatLng([lastPosition.latitude, lastPosition.longitude]);
            
            // Mettre à jour le tracé
            const coordinates = positions.map(pos => [pos.latitude, pos.longitude]);
            paths[animalId].setLatLngs(coordinates);
            
            // Mettre à jour les informations
            updateAnimalInfo(animalId, lastPosition);
        }
    } catch (error) {
        console.error(`Erreur lors du chargement des positions pour l'animal ${animalId}:`, error);
    }
}

// Fonction pour mettre à jour les informations d'un animal
function updateAnimalInfo(animalId, position) {
    const infoDiv = document.getElementById(`animal-${animalId}-info`);
    if (!infoDiv) {
        const container = document.getElementById('animals-info');
        const div = document.createElement('div');
        div.id = `animal-${animalId}-info`;
        div.className = 'animal-info';
        container.appendChild(div);
    }
    
    document.getElementById(`animal-${animalId}-info`).innerHTML = `
        <h3>Animal ${animalId}</h3>
        <p>Dernière position: ${position.latitude.toFixed(6)}, ${position.longitude.toFixed(6)}</p>
        <p>Altitude: ${position.altitude ? position.altitude.toFixed(1) + 'm' : 'N/A'}</p>
        <p>Batterie: ${position.battery_level ? position.battery_level + '%' : 'N/A'}</p>
        <p>Dernière mise à jour: ${new Date(position.timestamp).toLocaleString()}</p>
    `;
}

// Fonction utilitaire pour générer une couleur aléatoire
function getRandomColor() {
    const letters = '0123456789ABCDEF';
    let color = '#';
    for (let i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

// Démarrer le suivi
loadAnimals();
// Mettre à jour régulièrement
setInterval(loadAnimals, updateInterval);
