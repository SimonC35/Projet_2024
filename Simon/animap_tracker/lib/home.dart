import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';

import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';

class Home extends StatefulWidget {
  const Home(String lang, {super.key});

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  Widget build(BuildContext context) {
    var height_ = MediaQuery.of(context).size.height;

    // ignore: avoid_print
    print(lang);
    return Scaffold(
      body: Column(
        children: [
          // Bordereau coloré avec dégradé
          TopBanner(
            height: height_ / 2 * 0.35,
            startColor: const Color.fromARGB(255, 49, 122, 78),
            endColor: const Color.fromARGB(255, 111, 173, 110),
          ),
          SizedBox(height: 40),
          GestureDetector(
            onTap: () {
              context.read<NavigationProvider>().changeTab(2);
            },
            child: Container(
              width: 400,
              height: 200,
              decoration: BoxDecoration(
                border: Border.all(color: Colors.blue, width: 10),
                borderRadius: BorderRadius.all(Radius.circular(50)),
              ),
              child: MapPreview(), // 📌 Miniature de la carte
              ),
          )
        ],
      ),
    );
  }
}

class MapPreview extends StatelessWidget {
  const MapPreview({super.key});

  @override
  Widget build(BuildContext context) {
    return FlutterMap(
      options: MapOptions(
        initialCenter: LatLng(48.73137, -3.44936), // 📍 Paris
        initialZoom: 15.0,
        //onTap: context.read<NavigationProvider>().changeTab(2),      //onTap: context.read<NavigationProvider>().changeTab(2),
      ),
      children: [
        TileLayer(
          urlTemplate: "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
          subdomains: ['a', 'b', 'c'],
        ),
      ],
    );
  }
}

// ignore: must_be_immutable
class TopBanner extends StatelessWidget {

  final double height;
  final Color startColor;
  final Color endColor;

  const TopBanner({super.key, 
    this.height = 100,
    this.startColor = Colors.blue,
    this.endColor = Colors.purple,
  });

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Container(
          width: double.infinity,
          height: height,
          decoration: BoxDecoration(
            gradient: LinearGradient(
              begin: Alignment.topLeft,
              end: Alignment.bottomRight,
              colors: [startColor, endColor],
            ),
            borderRadius: BorderRadius.vertical(bottom: Radius.circular(20)), // Coins arrondis en bas
          ),
        ),
        Positioned(
          top: 40, // Décalage depuis le haut
          right: 40, // Décalage depuis la droite
          child: IconButtonWidget(
            icon: Icons.person,
            onPressed: () {
              context.read<NavigationProvider>().changeTab(4);
            },
          ),
        ),
        Positioned(
          top: 35,
          left: 40,
          child: AssetImageWidget(imagePath: "asset/images/Map.png"),
        ),
      ],
    );
  }
}

class IconButtonWidget extends StatelessWidget {
  final IconData icon;
  final VoidCallback onPressed;

  const IconButtonWidget({super.key, required this.icon, required this.onPressed});

  @override
  Widget build(BuildContext context) {
    return IconButton(
      color: const Color.fromARGB(255, 134, 212, 131),
      onPressed: onPressed,
      icon: Icon(icon, size: 45),
      style: ElevatedButton.styleFrom(
        backgroundColor: const Color.fromARGB(255, 50, 85, 64),
        padding: EdgeInsets.symmetric(horizontal: 15, vertical: 15),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20),
        ),
      ),
    );
  }
}

class AssetImageWidget extends StatelessWidget {
  final String imagePath;

  const AssetImageWidget({super.key, required this.imagePath});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Image.asset(
        imagePath,
        width: 60,
        height: 90,
        fit: BoxFit.cover,
      ),
    );
  }
}
