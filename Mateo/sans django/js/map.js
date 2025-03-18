// Configuration de la carte
let map = L.map('map').setView([48.7324, -3.4373], 13);

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
    
    // Réinitialiser le formulaire
    document.getElementById('add-point-form').reset();
});
