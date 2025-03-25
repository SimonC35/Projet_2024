<template>
    <div class="widget-content">
      <h2>Map Widget</h2>
      <p>Map will go here!</p>
    </div>
        <iframe width="100%" height="400px" frameborder="0" allowfullscreen allow="geolocation" src="//umap.openstreetmap.fr/fr/map/testosm_1189050?scaleControl=false&miniMap=false&scrollWheelZoom=false&zoomControl=true&editMode=disabled&moreControl=true&searchControl=null&tilelayersControl=null&embedControl=null&datalayersControl=true&onLoadPanel=none&captionBar=false&captionMenus=true"></iframe><p><a href="//umap.openstreetmap.fr/fr/map/testosm_1189050?scaleControl=false&miniMap=false&scrollWheelZoom=true&zoomControl=true&editMode=disabled&moreControl=true&searchControl=null&tilelayersControl=null&embedControl=null&datalayersControl=true&onLoadPanel=none&captionBar=false&captionMenus=true">Voir en plein écran</a></p>
  </template>
  
  <script>
  export default {
    name: "MapWidget",
  };
  </script>
  
  <style scoped>
  .widget-content {
    background-color: #cce6ff;
    padding: 20px;
    border-radius: 5px;
  }
  
  h2 {
    color: #0066cc;
  }
  </style>
  