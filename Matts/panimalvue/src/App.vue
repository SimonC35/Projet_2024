<template>
  <div id="app">
    <nav class="navbar">
      <router-link to="/home" class="navbar-link">Home</router-link>
      <router-link to="/alert" class="navbar-link">Alert</router-link>
      <router-link to="/list" class="navbar-link">List</router-link>
      <router-link to="/maps" class="navbar-link">Maps</router-link>
      <router-link to="/settings" class="navbar-link">Settings</router-link>
    </nav>
    <router-view />
  </div>
</template>


<style scoped>
/* Importer la police moderne et épurée Roboto depuis Google Fonts */
@import url('https://fonts.googleapis.com/css2?family=Roboto:wght@400;500;700&display=swap');

/* Style de la navbar */
.navbar {
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #4CAF50; /* Fond vert moderne */
  padding: 1rem 2rem;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.2); /* Ombre discrète pour profondeur */
  position: sticky;
  top: 0;
  z-index: 10;
  font-family: 'Roboto', sans-serif;
  border-bottom: 3px solid #388E3C; /* Bordure subtile pour un look net */
  border-radius: 10px 10px 0 0; /* Coins arrondis pour un aspect plus doux */
}

/* Style des liens dans la navbar */
.navbar-link {
  color: #ffffff; /* Blanc pour les liens */
  font-size: 18px;
  text-decoration: none;
  margin: 0 25px;
  padding: 10px 25px;
  border-radius: 25px; /* Coins arrondis pour les liens */
  transition: all 0.3s ease;
  font-weight: 500;
  text-transform: uppercase;
}

/* Effet au survol */
.navbar-link:hover {
  background-color: #66BB6A; /* Vert plus clair au survol */
  color: #ffffff; /* Texte en blanc */
  transform: scale(1.1); /* Légère expansion pour plus de fluidité */
  text-shadow: 0 2px 8px rgba(0, 0, 0, 0.2); /* Ombre discrète au survol */
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2); /* Ajout d'une ombre plus marquée */
}

/* Lien actif */
.navbar-link.router-link-active {
  background-color: #388E3C; /* Couleur verte plus foncée pour l'élément actif */
  color: #ffffff;
  font-weight: 600;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3); /* Ombre subtile sur l'élément actif */
}
</style>
