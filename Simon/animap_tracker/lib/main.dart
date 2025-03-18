import 'package:flutter/material.dart';

// Other file .dart
import 'package:animap_tracker/home.dart';
import 'package:animap_tracker/list_page.dart';
import 'package:animap_tracker/map_page.dart';
import 'package:animap_tracker/alert_page.dart';
import 'package:animap_tracker/login_page.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

// Global language variable
String lang = "fr";

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Animap Tracker',
      theme: ThemeData(
        useMaterial3: true,
        colorScheme: ColorScheme.fromSeed(seedColor: const Color.fromARGB(255, 49, 122, 78)),
      ),
      routes: {
        '/': (context) => MyHomePage(),
        '/Home': (context) => Home(lang),
        '/ListPage': (context) => ListPage(lang),
        '/MapPage': (context) => MapPage(),
        '/AlertPage': (context) => AlertPage(lang),
        '/LoginPage': (context) =>LoginPage(),
      },
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});
  
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
        page = Home(lang);
        break;
      case 1:
        page = ListPage(lang);
        break;
      case 2:
        page = MapPage();
        break;
      case 3:
        page = AlertPage(lang);
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
            items: <BottomNavigationBarItem>[
              BottomNavigationBarItem(
                icon: Icon(Icons.home, size: 30),
                label: AppLocalization(lang: lang).translation("_home"),
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.dehaze, size: 30),
                label: AppLocalization(lang: lang).translation("_list"),
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.map, size: 30),
                label: AppLocalization(lang: lang).translation("_map"),
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.notifications, size: 30),
                label: AppLocalization(lang: lang).translation("_alert"),
              ),
              BottomNavigationBarItem(
                icon: Icon(Icons.settings, size: 30),
                label: AppLocalization(lang: lang).translation("_setting"),
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
