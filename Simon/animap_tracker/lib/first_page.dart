import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';
import 'package:animap_tracker/account_page.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

import 'package:http/http.dart' as http;

class FirstPage extends StatefulWidget {
  const FirstPage(String lang, {super.key});

  @override
  State<FirstPage> createState() => _FirstPageState();
}

class _FirstPageState extends State<FirstPage> {
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();
  bool _isLoading = false;
  String _errorMessage = '';

  Future<void> _login() async {
    setState(() {
      _isLoading = true;
      _errorMessage = '';
    });

    final url = Uri.parse("http://172.20.14.7:8000/api/login/");
    final body = jsonEncode({
      "email": _emailController.text,           // matts@matts.com   simon@simon.com
      "password": _passwordController.text,     // matts             felix22
    });

    try {
      final response = await http.post(
        url,
        headers: {"Content-Type": "application/json"},
        body: body,
      );
      print(response.statusCode);
      if (response.statusCode == 200) {
        final data = jsonDecode(response.body);
        Navigator.push(context,
                  MaterialPageRoute(builder: (context) => MyHomePage()),
                );
      } else {
        setState(() {
          _errorMessage = "Identifiants incorrects.";
        });
      }
    } catch (e) {
      setState(() {
        _errorMessage = "Erreur de connexion: $e";
      });
    } finally {
      setState(() {
        _isLoading = false;
      });
    }
  }

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
              controller: _emailController,
              keyboardType: TextInputType.emailAddress,
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_user"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.person, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 10),

            TextField(
              controller: _passwordController,
              obscureText: true,
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_pass"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.lock, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 20),
            if (_errorMessage.isNotEmpty)
              Text(_errorMessage, style: TextStyle(color: Colors.red)),
            SizedBox(height: 10),
            _isLoading
                ? CircularProgressIndicator()
                : ElevatedButton(
                  onPressed: () {
                    Navigator.push(context,
                      MaterialPageRoute(builder: (context) => MyHomePage()),
                    );
                    //_login();
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
        lang = "bzh";
      });
    } else {
      setState(() {
        lang = "fr";
      });
    }
    context.read<NavigationProvider>().changeTab(4);
  }
}
