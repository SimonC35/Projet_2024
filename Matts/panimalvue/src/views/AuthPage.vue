<template>
    <div class="auth-container">
      <h2 v-if="isLogin">Connexion</h2>
      <h2 v-else>Inscription</h2>
      <form @submit.prevent="submitForm" class="auth-form">
        <input type="email" v-model="email" placeholder="Email" required class="input-field" />
        <input type="text" v-model="name" placeholder="Nom" v-if="!isLogin" required class="input-field" />
        <input type="password" v-model="password" placeholder="Mot de passe" required class="input-field" />
        <button type="submit" class="submit-btn">{{ isLogin ? 'Se connecter' : 'S\'inscrire' }}</button>
      </form>
      <p v-if="message" :class="messageClass" class="message">{{ message }}</p>
  
      <div v-if="!isLogin" class="toggle-link">
        <p>Déjà un compte ? <a href="#" @click="toggleForm">Se connecter</a></p>
      </div>
      <div v-else class="toggle-link">
        <p>Pas encore de compte ? <a href="#" @click="toggleForm">S'inscrire</a></p>
      </div>
    </div>
  </template>
  
  <script>
  import axios from 'axios';
  
  export default {
    data() {
      return {
        email: '',
        name: '',
        password: '',
        message: '',
        messageClass: '',
        isLogin: true // Contrôle si l'on est dans le mode connexion ou inscription
      };
    },
    methods: {
      async submitForm() {
        const url = this.isLogin
          ? 'http://localhost:8000/api/login/'
          : 'http://localhost:8000/api/register/';
        
        const body = this.isLogin
          ? { email: this.email, password: this.password }
          : { name: this.name, email: this.email, password: this.password };
  
        try {
          const response = await axios.post(url, body);
          this.message = response.data.message || 'Action réussie!';
          this.messageClass = 'success';
          
          // Si la connexion est réussie, rediriger l'utilisateur vers la page protégée
          if (this.isLogin) {
            localStorage.setItem('access_token', response.data.access_token);
            localStorage.setItem('refresh_token', response.data.refresh_token);
            
            // Redirection vers une page protégée après la connexion
            this.$router.push('/Home');
          }
        } catch (error) {
          this.message = error.response.data.error || 'Une erreur est survenue.';
          this.messageClass = 'error';
        }
      },
      toggleForm() {
        this.isLogin = !this.isLogin;
        this.message = ''; // Réinitialiser le message à chaque changement de formulaire
      }
    }
  };
  </script>
  
  <style scoped>
  /* Contexte général de la page */
  .auth-container {
    max-width: 450px;
    margin: 0 auto;
    margin-top: 10%;
    padding: 30px;
    background-color: #f9f9f9;
    border-radius: 15px;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
  }
  
  /* Titre de la page */
  h2 {
    font-family: 'Roboto', sans-serif;
    text-align: center;
    color: #388E3C; /* Vert plus foncé */
    margin-bottom: 20px;
    font-weight: 700;
  }
  
  /* Style des champs de saisie */
  .input-field {
    width: 100%;
    padding: 12px;
    margin: 15px 0;
    border-radius: 8px;
    border: 1px solid #ddd;
    font-size: 16px;
    transition: border 0.3s ease, box-shadow 0.3s ease;
  }
  
  .input-field:focus {
    border-color: #4CAF50; /* Bordure verte au focus */
    box-shadow: 0 0 5px rgba(72, 181, 97, 0.5);
  }
  
  /* Bouton de soumission */
  .submit-btn {
    width: 100%;
    padding: 12px;
    background-color: #4CAF50; /* Vert principal */
    color: white;
    font-size: 16px;
    border: none;
    border-radius: 8px;
    cursor: pointer;
    transition: background-color 0.3s ease;
  }
  
  .submit-btn:hover {
    background-color: #66BB6A; /* Vert plus clair au survol */
  }
  
  /* Message de retour */
  .message {
    text-align: center;
    margin-top: 20px;
    font-size: 14px;
    font-weight: bold;
  }
  
  .message.success {
    color: #388E3C; /* Vert pour le succès */
  }
  
  .message.error {
    color: #D32F2F; /* Rouge pour les erreurs */
  }
  
  /* Liens pour basculer entre inscription et connexion */
  .toggle-link {
    text-align: center;
    margin-top: 20px;
  }
  
  .toggle-link a {
    color: #4CAF50;
    text-decoration: none;
    font-weight: 500;
  }
  
  .toggle-link a:hover {
    text-decoration: underline;
  }
  </style>
  