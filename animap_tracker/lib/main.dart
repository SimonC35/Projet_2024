import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';

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
        debugShowCheckedModeBanner: false,
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
}

class Home extends StatelessWidget {
  
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.cyan,
        toolbarHeight: 150,
      )
    );
  }
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

    void onItemTapped(int index) {
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
            onTap: onItemTapped,
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
  
  final TextEditingController _login = TextEditingController();
    final TextEditingController _password = TextEditingController();

  void initstate() {
    super.initState();
    _login.addListener(() {
      final String text = _login.text.toLowerCase();
      _login.value = _login.value.copyWith(
        text: text,
        selection: 
          TextSelection(baseOffset: text.length, extentOffset: text.length),
        composing: TextRange.empty,
      );
    });
    _password.addListener(() {
      final String text = _password.text.toLowerCase();
      _password.value = _password.value.copyWith(
        text: text,
        selection: 
          TextSelection(baseOffset: text.length, extentOffset: text.length),
        composing: TextRange.empty,
      );
    });
  }
  @override
  void dispose() {
    _login.dispose();
    _password.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.amber,
        toolbarHeight: 150,
      ),
    );
  }

  Widget content() {
    return Container(
      alignment: Alignment.center,
      padding: const EdgeInsets.all(200),
      child: Column(
        children: [
          Text(
            "Login",
            style: TextStyle(fontSize: 25, fontWeight: FontWeight.bold),
          ),
          SizedBox(
            height: 10,
          ),
          TextFormField(
            controller: _login,
            decoration: const InputDecoration(border: OutlineInputBorder()),
          ),
          SizedBox(
            height: 40,
          ),
          Text(
            "Password",
            style: TextStyle(fontSize: 25, fontWeight: FontWeight.bold),
          ),
          SizedBox(
            height: 10,
          ),
          TextFormField(
            controller: _password,
            decoration: const InputDecoration(border: OutlineInputBorder()),
          ),
        ],
      ),
    );
  }
  _GoToLoginPage(BuildContext) {
     return Navigator.of(context).push(MaterialPageRoute(builder: (context) {
      return Scaffold(
        appBar: AppBar(
          title: Text('Login Page'),
        ),
        body: content(),
      );/*
          alignment: Alignment.center,
          padding: const EdgeInsets.all(200),
          children: [
            TextFormField(
              controller: _controller,
              decoration: const InputDecoration(border: OutlineInputBorder()),
            ),
            TextFormField(
              controller: _controller,
              decoration: const InputDecoration(border: OutlineInputBorder()),
            ),
          ],
        ),
      );*/
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
