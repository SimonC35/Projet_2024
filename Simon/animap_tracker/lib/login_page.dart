import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';
import 'package:animap_tracker/account_page.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

class LoginPage extends StatefulWidget {
  const LoginPage({super.key});

  @override
  State<LoginPage> createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: EdgeInsets.all(20),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.lock, size: 80, color: const Color.fromARGB(255, 66, 150, 101)),

            SizedBox(height: 20),

            TextField(
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_user"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.person, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 10),

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
                print("Connexion...");
              },
              child: Text(
                    AppLocalization(lang: lang).translation("_login"),
            )),
            Divider(
              color: Colors.black,
              height: 50,
            ),
            ElevatedButton(
              onPressed: () {
                Navigator.push(context,
                  MaterialPageRoute(builder: (context) => AccountPage()),
                );
              },
              child: Text(
                AppLocalization(lang: lang).translation("_account"),
            )),
            ElevatedButton(
              onPressed: changeLang,
              child: Text(
                AppLocalization(lang: lang).translation("changeLanguage"),
            )),
          ],
        ),
      ),
    );
  }
  void changeLang() {
    if (lang == "fr") {
      setState(() {
        lang = "en";
      });
    } else {
      setState(() {
        lang = "fr";
      });
    }
    context.read<NavigationProvider>().changeTab(4);
  }
}
