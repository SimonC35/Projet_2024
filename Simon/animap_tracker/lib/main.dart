import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// Other file .dart
import 'package:animap_tracker/home.dart';
import 'package:animap_tracker/list_page.dart';
import 'package:animap_tracker/map_page.dart';
import 'package:animap_tracker/alert_page.dart';
import 'package:animap_tracker/login_page.dart';
import 'package:animap_tracker/first_page.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

// Global language variable
String lang = "fr";

void main() {
  runApp(
    ChangeNotifierProvider(
      create: (context) => NavigationProvider(),
      child: MyApp(),
    ),
  );
}

class NavigationProvider extends ChangeNotifier {
  int _currentIndex = 0;

  int get currentIndex => _currentIndex;

  void changeTab(int index) {
    _currentIndex = index;
    notifyListeners();
  }
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
        '/': (context) => FirstPage(lang),
        '/MyHomePage': (context) => MyHomePage(),
        '/Home': (context) => Home(lang),
        '/ListPage': (context) => ListPage(lang),
        '/MapPage': (context) => MapPage(),
        '/AlertPage': (context) => AlertPage(lang),
        '/LoginPage': (context) => LoginPage(),
      },
    );
  }
}

class MyHomePage extends StatelessWidget {
  const MyHomePage({super.key});

  @override
  Widget build(BuildContext context) {
    int currentIndex = context.watch<NavigationProvider>().currentIndex;

    return Scaffold(
      body: IndexedStack(
        index: currentIndex,
        children: [
          Home(lang), 
          ListPage(lang), 
          MapPage(), 
          AlertPage(lang), 
          LoginPage()
        ],
      ),
      bottomNavigationBar: BottomNavigationBar(
        
        items: [
          BottomNavigationBarItem(
            icon: Icon(Icons.home, size: 35),
            label: AppLocalization(lang: lang).translation("_home"),
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.dehaze, size: 35),
            label: AppLocalization(lang: lang).translation("_list"),
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.map, size: 35),
            label: AppLocalization(lang: lang).translation("_map"),
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.notifications, size: 35),
            label: AppLocalization(lang: lang).translation("_alert"),
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.settings, size: 35),
            label: AppLocalization(lang: lang).translation("_setting"),
          ),
        ],
        selectedItemColor: const Color.fromARGB(255, 128, 38, 180),
        unselectedItemColor: const Color.fromARGB(255, 62, 167, 185),
        showUnselectedLabels: true,
        currentIndex: currentIndex,
        onTap: (index) {
          context.read<NavigationProvider>().changeTab(index);
        },
      ),
    );
  }
}
