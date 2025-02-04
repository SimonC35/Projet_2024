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
          colorScheme: ColorScheme.fromSeed(seedColor: const Color.fromARGB(255, 49, 122, 78)),
        ),
        home: MyHomePage(),
      ),
    );
  }
}

class MyAppState extends ChangeNotifier {
}

class AssetImageWidget extends StatelessWidget {
  final String imagePath;

  AssetImageWidget({required this.imagePath});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Image.asset(
        imagePath,
        width: 70,
        height: 90,
        fit: BoxFit.cover,
      ),
    );
  }
}

class TopBanner extends StatelessWidget {
  
  final double height;
  final Color startColor;
  final Color endColor;

  TopBanner({
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
          right: 20, // Décalage depuis la droite
          child: IconButtonWidget(
            icon: Icons.person,
            onPressed: () {
              Navigator.push(context, MaterialPageRoute(builder: (context) => LoginPage()));
            },
          ),
        ),
        Positioned(
          top: 20,
          left: 40,
          child: AssetImageWidget(imagePath: "asset/images/Map.png"),
        ),
      ],
    );
  }
}

class Home extends StatelessWidget {
  
  @override
  Widget build(BuildContext context) {
    var _height = MediaQuery.of(context).size.height;

    return Scaffold(
      body: Column(
        children: [
          // Bordereau coloré avec dégradé
          TopBanner(
            height: _height / 2 * 0.3,
            startColor: const Color.fromARGB(255, 49, 122, 78),
            endColor: const Color.fromARGB(255, 111, 173, 110),
          ),
        ],
      ),
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
        page = Home();
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
        page = LoginPage();
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

class IconButtonWidget extends StatelessWidget {
  final IconData icon;
  final VoidCallback onPressed;

  IconButtonWidget({required this.icon, required this.onPressed});

  @override
  Widget build(BuildContext context) {
    return IconButton(
      color: const Color.fromARGB(255, 134, 212, 131),
      onPressed: onPressed,
      icon: Icon(icon, size: 30),
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

class LoginPage extends StatelessWidget {
  
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: EdgeInsets.all(20),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            // 🖼 Logo ou image (facultatif)
            Icon(Icons.lock, size: 80, color: Colors.blue),

            SizedBox(height: 20),

            // 📧 Champ Email
            TextField(
              decoration: InputDecoration(
                labelText: "Email",
                border: OutlineInputBorder(),
              ),
            ),

            SizedBox(height: 10),

            // 🔑 Champ Mot de passe
            TextField(
              obscureText: true,
              decoration: InputDecoration(
                labelText: "Mot de passe",
                border: OutlineInputBorder(),
              ),
            ),

            SizedBox(height: 20),

            // ✅ Bouton Connexion
            ElevatedButton(
              onPressed: () {
                print("Connexion...");
              },
              child: Text("Se connecter"),
              style: ElevatedButton.styleFrom(
                minimumSize: Size(double.infinity, 50), // Largeur max
              ),
            ),
          ],
        ),
      ),
    );
  }
}
