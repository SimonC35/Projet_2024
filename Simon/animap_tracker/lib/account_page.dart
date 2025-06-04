import 'dart:convert';
import 'package:flutter/material.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

import 'package:http/http.dart' as http;

class AccountPage extends StatefulWidget {
  const AccountPage({super.key});

  @override
  State<AccountPage> createState() => _AccountPageState();
}

class _AccountPageState extends State<AccountPage> {
  final TextEditingController _nameController = TextEditingController();
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();
  bool _isLoading = false;
  String _errorMessage = '';

  Future<void> _register() async {
    setState(() {
      _isLoading = true;
      _errorMessage = '';
    });

    final url = Uri.parse("http://172.20.14.7:8000/api/register/");
    final body = jsonEncode({
      "name": _nameController.text,
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
      appBar: AppBar(title: Text(
        AppLocalization(lang: lang).translation("_account"),
      )),
      body: Padding(
        padding: EdgeInsets.all(20),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.lock, size: 80, color: const Color.fromARGB(255, 66, 150, 101)),

            SizedBox(height: 20),

            TextField(
              controller: _nameController,
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_user"),
                border: OutlineInputBorder(),
                suffixIcon: Icon(Icons.mail, size: 20, color: const Color.fromARGB(255, 66, 150, 101)),
              ),
            ),

            SizedBox(height: 10),

            TextField(
              controller: _emailController,
              decoration: InputDecoration(
                labelText: AppLocalization(lang: lang).translation("_mail"),
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

            ElevatedButton(
              onPressed: () {
                _register();
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
