const express = require('express');
const cors = require('cors');
const app = express();
const path = require('path');

// Middleware
app.use(cors());
app.use(express.static(__dirname));

// Route par défaut
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Route pour les données GPS simulées
app.get('/api/gps-data/', (req, res) => {
    const simulatedData = [
        {
            location: { type: 'Point', coordinates: [-3.4373, 48.7324] },
            date: new Date().toISOString(),
            sats: 10,
            hdop: 1.5,
            speed: 10
        }
    ];
    res.json(simulatedData);
});

// Démarrer le serveur
const PORT = 8000;
app.listen(PORT, '127.0.0.1', () => {
    console.log(`Serveur démarré sur http://127.0.0.1:${PORT}`);
});
