from django.contrib.gis.db import models


class GPSData(models.Model):
    date = models.DateTimeField(primary_key=True)
    location = models.GeometryField()  # Nécessite PostGIS activé
    hdop = models.FloatField(null=True, blank=True)
    speed = models.FloatField(null=True, blank=True)
    course = models.FloatField(null=True, blank=True)
    sats = models.IntegerField(null=True, blank=True)
    age = models.FloatField(null=True, blank=True)

    class Meta:
        db_table = "gps_data"  # Liaison explicite avec la table existante

    def __str__(self):
        return f"{self.date} - {self.location}"
