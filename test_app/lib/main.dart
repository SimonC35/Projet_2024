import 'package:english_words/english_words.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';
import 'package:latlong2/spline.dart';
import 'package:flutter_osm_plugin/flutter_osm_plugin.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (context) => MyAppState(),
      child: MaterialApp(
        title: 'Namer App',
        theme: ThemeData(
          useMaterial3: true,
          colorScheme: ColorScheme.fromSeed(seedColor: const Color.fromARGB(255, 35, 88, 56)),
        ),
        home: MyHomePage(),
      ),
    );
  }
}

class MyAppState extends ChangeNotifier {
  var current = WordPair.random();
}

class MyHomePage extends StatefulWidget {
  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  
  int selectedIndex = 0;
  
  @override
  Widget build(BuildContext context) {

    Widget page;
    switch (selectedIndex) {
      case 0:
        page = HomePage();
        break;
      case 1:
        page = ListPage();
        break;
      case 2:
        page = MapPage();
        break;
      case 3:
        page = AlertPage();
        break;
      case 4:
        page = SettingsPage();
        break;
      default:
        throw UnimplementedError('no widget for $selectedIndex');
    }

    void _onItemTapped(int index) {
    setState(() {
      selectedIndex = index;
      IconButton;
    });
  }
    return LayoutBuilder(
      builder: (context, constraints) {
        return Scaffold(
          body: Center(
            child: page,
          ),
          bottomNavigationBar: BottomNavigationBar(
            items: const <BottomNavigationBarItem>[
              BottomNavigationBarItem(
                icon: Icon(Icons.home),
                label: 'Home',
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.dehaze),
                label: 'List',
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.map),
                label: 'Map',
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.notifications),
                label: 'Alerte',
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.settings),
                label: 'Settings',
              ),
            ],
            currentIndex: selectedIndex,
            selectedItemColor: const Color.fromARGB(255, 128, 38, 180),
            unselectedItemColor: const Color.fromARGB(255, 62, 167, 185),
            showUnselectedLabels: true,
            onTap: _onItemTapped,
          ),
        );
      }
    );
  }
}
class MapPage extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: content(),
    );
  }

  Widget content() {
    return FlutterMap(
      options: MapOptions(
        initialCenter: LatLng(48.733333, -3.449757),
        initialZoom: 16,
        minZoom: 3,
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

class HomePage extends StatefulWidget {
  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.start,
      children: [
        SizedBox(height: 50),
        Row(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            IconButton(
              icon: Icon(Icons.person),
              iconSize: 40,
              tooltip: 'Login',
              onPressed: () {
                setState(() {
                  _GoToLoginPage(context);
                });
              }, 
            ),
            SizedBox(width: 50),
          ],
        ),
      ],
    );
  }
  _GoToLoginPage(BuildContext) {
     return Navigator.of(context).push(MaterialPageRoute(builder: (context) {
      return Scaffold(
        appBar: AppBar(
          title: Text('Login Page'),
        ),
      );
    }));
  }
}

class ListPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Text('Animal Tracker List Page'),
    );
  }
}

class AlertPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Text('Alerte Page'),
    );
  }
}

class SettingsPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Text('Settings Page'),
    );
  }
}
