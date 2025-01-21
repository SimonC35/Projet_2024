import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';
import 'package:latlong2/spline.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: MyHomePage(),
    );
  }

   Widget content() {
    return FlutterMap(
      options: MapOptions(
        initialCenter: LatLng(48.733333, -3.449757),
        initialZoom: 30,
        interactionOptions: 
          const InteractionOptions(flags: ~InteractiveFlag.doubleTapDragZoom),
      ),
      children: [
        openStreetMapTileLayer,
      ],
    );
  }
}

TileLayer get openStreetMapTileLayer => TileLayer(
  urlTemplate: 'https://tile.openstreetmap.org/#18/48.731745/-3.450657.png',
  userAgentPackageName: 'dev.fleaflet.flutter_map.exemple',
);

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(
          'Open Street Map',
          style: TextStyle(fontSize: 22),
        ),
      ),
      body: content(),
    );
  }

  Widget content() {
    return FlutterMap(
      options: MapOptions(
        initialCenter: LatLng(48.733333, -3.449757),
        initialZoom: 11,
        interactionOptions: 
          const InteractionOptions(flags: ~InteractiveFlag.doubleTapDragZoom),
      ),
      children: [
        openStreetMapTileLayer,
      ],
    );
  }

  TileLayer get openStreetMapTileLayer => TileLayer(
    urlTemplate: 'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
    userAgentPackageName: 'dev.fleaflet.flutter_map.exemple',
  );
}

