import 'package:flutter/material.dart';

// Main Class => global var (lang)
import 'package:animap_tracker/main.dart';

// For Language Anglais / Français
import 'package:animap_tracker/localization.dart';

class AlertPage extends StatelessWidget {
 const AlertPage(String lang, {super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Text(AppLocalization(lang: lang).translation("_alertText")),
    );
  }
}