import 'package:flutter/material.dart';

class MonEcran extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Mon Application Flutter"),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[
            // Un widget de texte
            Text(
              'Bienvenue sur Flutter !',
              style: TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 20),
            
            // Un autre texte avec style
            Text(
              'Voici un exemple de classe contenant des widgets.',
              style: TextStyle(fontSize: 16, color: Colors.grey[600]),
            ),
            SizedBox(height: 40),
            
            // Un bouton
            ElevatedButton(
              onPressed: () {
                // Action à réaliser lorsqu'on appuie sur le bouton
                print('Le bouton a été cliqué');
              },
              child: Text('Appuie ici'),
            ),
            SizedBox(height: 40),
            
            // Une image
            //Image.asset('assets/mon_image.png'),  // Assurez-vous que l'image est dans le dossier "assets" de votre projet
          ],
        ),
      ),
    );
  }
}
