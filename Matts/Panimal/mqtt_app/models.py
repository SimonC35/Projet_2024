from django.contrib.gis.db import models


#  TABLE COMPTE (Utilisateurs)
class Compte(models.Model):
    email = models.EmailField(unique=True)
    nom = models.CharField(max_length=100)
    password = models.CharField(max_length=255)  # Stocké de façon sécurisée par Django

    class Meta:
        db_table = "Compte"

    def __str__(self):
        return self.nom



#  TABLE OBJET (Objets suivis)
class Objet(models.Model):
    id_objet = models.AutoField(primary_key=True)
    nom_objet = models.CharField(max_length=100)

    class Meta:
        db_table = "Objet"

    def __str__(self):
        return self.nom_objet


#  TABLE DONNÉE (Données GPS)
class Donnee(models.Model):
    id_data = models.AutoField(primary_key=True)
    date = models.DateTimeField()
    location = models.GeometryField()  # Nécessite PostGIS activé
    hdop = models.FloatField(null=True, blank=True)
    speed = models.FloatField(null=True, blank=True)
    course = models.FloatField(null=True, blank=True)
    sats = models.IntegerField(null=True, blank=True)
    id_objet = models.ForeignKey(Objet, on_delete=models.CASCADE)  # Relation avec Objet

    class Meta:
        db_table = "Donnee2"

    def __str__(self):
        return f"{self.date} - {self.location}"


#  TABLE AFFECTATION (Relation Compte ↔ Objet)
class Affectation(models.Model):
    utilisateur = models.ForeignKey(Compte, on_delete=models.CASCADE)
    objet = models.ForeignKey(Objet, on_delete=models.CASCADE)

    class Meta:
        db_table = "affectation"
        unique_together = ('utilisateur', 'objet')  # Empêche les doublons

    def __str__(self):
        return f"{self.utilisateur.nom} ↔ {self.objet.nom_objet}"