<template>
  <div id="app">
    <nav>
      <router-link to="/">Home </router-link>
      <router-link to="/Alert">Alert </router-link>
      <router-link to="/List">List </router-link>
      <router-link to="/Login">Login </router-link>
      <router-link to="/Maps">Maps </router-link>
      <router-link to="/Settings">Settings </router-link>
    </nav>
    <router-view />
  </div>
</template>
