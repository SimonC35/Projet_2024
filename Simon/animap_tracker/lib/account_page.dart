import 'package:animap_tracker/home.dart';
import 'package:flutter/material.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

class AccountPage extends StatelessWidget {
  const AccountPage({super.key});

    @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(
        AppLocalization(lang: lang).translation("_account"),
      )),
      body: Padding(
        padding: EdgeInsets.all(20),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            // 🖼 Logo ou image (facultatif)
            Icon(Icons.lock, size: 80, color: const Color.fromARGB(255, 66, 150, 101)),

            SizedBox(height: 20),

            TextField(
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_mail"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.mail, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 10),

            // 📧 Champ Email
            TextField(
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_user"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.person, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 10),

            // 🔑 Champ Mot de passe
            TextField(
              obscureText: true,
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_pass"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.lock, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 20),

            ElevatedButton(
              onPressed: () {
                Navigator.push(context,
                  MaterialPageRoute(builder: (context) => MyHomePage()),
                );
              },
              child: Text(
                AppLocalization(lang: lang).translation("_account"),
            )),
          ],
        ),
      ),
    );
  }
}
