import 'package:flutter/material.dart';

void main() {
  runApp(const MainApp());
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      routes: {
        "/Home" : (context) => Home(),
        "/Page1" : (context) => Page1(),
        "/Page2" : (context) => Page2(),
        "/Page3" : (context) => Page3(),
      },
      home: Scaffold(
        appBar: AppBar(
          title: Text("App Demo"),
          centerTitle: true,
          backgroundColor: Color.fromARGB(198, 126, 197, 126),
        ),
        body: Center(
          child: Column(
            children: [
              const Text("Hello World !"),
              ElevatedButton(
                onPressed: () {
                  print("Connexion...");
                }, 
                child: Text("Connexion"),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class MyHomePage extends StatelessWidget{
  const MyHomePage({super.key});

  @override
  Widget build(BuildContext context) {
      return Scaffold(
        bottomNavigationBar: BottomNavigationBar(
          items: [
            BottomNavigationBarItem(
              icon: Icon(Icons.home, size: 35,),
              label: "Home",
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.home, size: 35,),
              label: "Home",
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.home, size: 35,),
              label: "Home",
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.home, size: 35,),
              label: "Home",
            ),
          ] 
        )
      );
  }
}

class Home extends StatefulWidget {
  const Home({super.key});

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}

class Page1 extends StatefulWidget {
  const Page1({super.key});

  @override
  State<Page1> createState() => _Page1State();
}

class _Page1State extends State<Page1> {
  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}

class Page2 extends StatefulWidget {
  const Page2({super.key});

  @override
  State<Page2> createState() => _Page2State();
}

class _Page2State extends State<Page2> {
  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}

class Page3 extends StatefulWidget {
  const Page3({super.key});

  @override
  State<Page3> createState() => _Page3State();
}

class _Page3State extends State<Page3> {
  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}
