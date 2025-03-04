import { createRouter, createWebHistory } from 'vue-router'
import Home from '../views/Home.vue'
import Alert from '../views/Alert.vue'
import List from '../views/List.vue'
import Maps from '../views/Maps.vue'
import Login from '../views/Login.vue'
import Settings from '../views/Settings.vue'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home
  },
  {
    path: '/Alert',
    name: 'Alert',
    component: Alert
  },
  {
    path: '/List',
    name: 'List',
    component: List
  },
  {
    path: '/Maps',
    name: 'Maps',
    component: Maps
  },
  {
    path: '/Login',
    name: 'Login',
    component: Login
  },
  {
    path: '/Settings',
    name: 'Settings',
    component: Settings
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
