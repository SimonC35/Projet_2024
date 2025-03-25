import { createRouter, createWebHistory } from 'vue-router'
import Home from '../views/Home.vue'
import Alert from '../views/Alert.vue'
import List from '../views/List.vue'
import Maps from '../views/Maps.vue'
import Settings from '../views/Settings.vue'
import AuthPage from '@/views/AuthPage.vue';

const routes = [
  {
    path: '/',
    component: AuthPage, // Page de connexion/inscription
  },
  {
    path: '/Home',
    name: 'Home',
    component: Home,
    beforeEnter: (to, from, next) => {
      const token = localStorage.getItem('access_token');
      if (!token) {
        next('/'); // Redirige vers la page de connexion si pas de token
      } else {
        next(); // Permet l'accès à la page si le token est présent
      }
    }
    
  },
  {
    path: '/Alert',
    name: 'Alert',
    component: Alert,
    beforeEnter: (to, from, next) => {
      const token = localStorage.getItem('access_token');
      if (!token) {
        next('/'); // Redirige vers la page de connexion si pas de token
      } else {
        next(); // Permet l'accès à la page si le token est présent
      }
    }
  },
  {
    path: '/List',
    name: 'List',
    component: List,
    beforeEnter: (to, from, next) => {
      const token = localStorage.getItem('access_token');
      if (!token) {
        next('/'); // Redirige vers la page de connexion si pas de token
      } else {
        next(); // Permet l'accès à la page si le token est présent
      }
    }
  },
  {
    path: '/Maps',
    name: 'Maps',
    component: Maps,
    beforeEnter: (to, from, next) => {
      const token = localStorage.getItem('access_token');
      if (!token) {
        next('/'); // Redirige vers la page de connexion si pas de token
      } else {
        next(); // Permet l'accès à la page si le token est présent
      }
    }
  },
  {
    path: '/Settings',
    name: 'Settings',
    component: Settings,
    beforeEnter: (to, from, next) => {
      const token = localStorage.getItem('access_token');
      if (!token) {
        next('/'); // Redirige vers la page de connexion si pas de token
      } else {
        next(); // Permet l'accès à la page si le token est présent
      }
    }
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
