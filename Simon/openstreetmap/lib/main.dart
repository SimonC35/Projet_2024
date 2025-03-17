import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';

class UmapFlutterMap extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Carte uMap")),
      body: FlutterMap(
        options: MapOptions(
          initialCenter: LatLng(48.8566, 2.3522), // Coordonnées de Paris (remplace par celles de ta carte)
          initialZoom: 10.0,
        ),
        children: [
          TileLayer(
            urlTemplate: "https://umap.openstreetmap.fr/fr/map/testosm_1189050",
            subdomains: ['a', 'b', 'c'],
          ),
        ],
      ),
    );
  }
}
