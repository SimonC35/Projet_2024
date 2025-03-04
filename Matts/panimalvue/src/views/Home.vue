<template>
    <div class="home">
      <!-- Header -->
      <header>
        <div class="header-left">
          <h1>Animap Tracker</h1>
        </div>
        <div class="header-right">
          <img src="@/assets/Map.png" alt="Login" class="login-logo"/>
        </div>
      </header>
  
      <!-- Main Content -->
      <main>
        <!-- Two Widgets in a Row -->
        <div class="widgets-row">
          <div class="widget">
            <ListWidget />
          </div>
          <div class="widget">
            <AlertWidget />
          </div>
        </div>
  
        <!-- Bottom Widget -->
        <div class="widget full-width">
          <MapWidget />
        </div>
      </main>
    </div>
  </template>
  
  <script>
  import ListWidget from "@/components/ListWidget.vue";
  import AlertWidget from "@/components/AlertWidget.vue";
  import MapWidget from "@/components/MapWidget.vue";
  
  export default {
    name: "HomePage",
    components: {
      ListWidget,
      AlertWidget,
      MapWidget,
    },
  };
  </script>
  
  <style scoped>
  .home {
    font-family: Arial, sans-serif;
    padding: 0;
    margin: 0;
  }
  
  header {
    display: flex;
    justify-content: space-between;
    padding: 20px;
    background-color: #3e8e41;
    color: white;
  }
  
  .header-left h1 {
    margin: 0;
  }
  
  .header-right .login-logo {
    width: 40px;
    height: 40px;
    cursor: pointer;
  }
  
  .widgets-row {
    display: flex;
    justify-content: space-between;
    margin: 20px;
  }
  
  .widget {
    width: 48%; /* To make both widgets take half of the width each */
    margin: 10px 0;
  }
  
  .full-width {
    width: 100%; /* The bottom widget takes full width */
    margin-top: 20px;
  }
  
  h1 {
    font-size: 2em;
  }
  </style>
  
