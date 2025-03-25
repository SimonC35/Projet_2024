<template>
  <div class="settings-page">
    <header class="settings-header">
      <h1 class="settings-title">Page de Paramètres</h1>
      <p class="settings-description">Gérez vos informations personnelles et paramètres du compte. Vous pouvez également vous déconnecter ici.</p>
    </header>

    <section class="settings-content">
      <div class="settings-options">
        <h2 class="section-title">Mes Informations</h2>
        <ul class="settings-list">
          <li>Nom d'utilisateur: <span class="settings-value">testuser</span></li>
          <li>Email: <span class="settings-value">test@example.com</span></li>
          <li><a href="#" class="edit-link">Modifier mes informations</a></li>
        </ul>
      </div>

      <div class="settings-actions">
        <router-link to="/" class="settings-link">Retour à l'Accueil</router-link>
        <button class="logout-button" @click="logout">Se Déconnecter</button>
      </div>
    </section>

    <footer class="settings-footer">
      <p>© 2024 Panimal. Tous droits réservés.</p>
    </footer>
  </div>
</template>

<script>
export default {
  name: 'SettingsPage',
  methods: {
    logout() {
      // Supprimer les tokens lors de la déconnexion
      localStorage.removeItem('access_token');
      localStorage.removeItem('refresh_token');
      
      // Rediriger l'utilisateur vers la page de connexion
      this.$router.push('/');
    }
  }
}
</script>

<style scoped>
/* Global styles for the page */
.settings-page {
  background-color: #fff;
  color: #333;
  font-family: 'Roboto', sans-serif;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  min-height: 100vh;
  padding: 40px;
}

/* Header section */
.settings-header {
  text-align: center;
  margin-bottom: 30px;
}

.settings-title {
  font-size: 2.5rem;
  font-weight: bold;
  margin-bottom: 10px;
  color: #388E3C;
}

.settings-description {
  font-size: 1.2rem;
  color: #777;
  margin-bottom: 20px;
}

/* Main content */
.settings-content {
  flex: 1;
  background-color: #F4F4F4;
  border-radius: 10px;
  padding: 20px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.settings-options {
  margin-bottom: 30px;
}

.section-title {
  font-size: 1.8rem;
  margin-bottom: 10px;
  color: #388E3C;
}

.settings-list {
  list-style: none;
  padding: 0;
  margin: 0;
  font-size: 1.2rem;
  color: #333;
}

.settings-list li {
  margin-bottom: 15px;
}

.settings-value {
  font-weight: bold;
}

.edit-link {
  color: #388E3C;
  text-decoration: none;
  font-weight: bold;
  transition: color 0.3s;
}

.edit-link:hover {
  color: #FF9800;
}

/* Buttons and links */
.settings-link {
  color: #388E3C;
  font-size: 1rem;
  text-decoration: none;
  padding: 12px 24px;
  border: 2px solid #388E3C;
  border-radius: 8px;
  transition: background-color 0.3s, color 0.3s;
  display: inline-block;
  margin-top: 20px;
}

.settings-link:hover {
  background-color: #388E3C;
  color: white;
}

.logout-button {
  background-color: #FF5722;
  border: none;
  padding: 12px 24px;
  color: white;
  font-size: 1.2rem;
  font-weight: bold;
  border-radius: 8px;
  cursor: pointer;
  transition: background-color 0.3s, transform 0.3s;
  margin-top: 20px;
}

.logout-button:hover {
  background-color: #D32F2F;
  transform: translateY(-2px);
}

.logout-button:focus {
  outline: none;
}

/* Footer section */
.settings-footer {
  text-align: center;
  margin-top: 30px;
  font-size: 1rem;
  color: #777;
}

/* Mobile responsiveness */
@media screen and (max-width: 768px) {
  .settings-title {
    font-size: 2rem;
  }

  .settings-description {
    font-size: 1rem;
  }

  .settings-list li {
    font-size: 1rem;
  }

  .edit-link {
    font-size: 1rem;
  }

  .logout-button,
  .settings-link {
    font-size: 1rem;
    padding: 10px 20px;
  }
}
</style>
