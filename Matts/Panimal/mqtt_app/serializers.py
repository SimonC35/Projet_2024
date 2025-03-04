from rest_framework_gis.serializers import GeoFeatureModelSerializer
from rest_framework import serializers
from .models import GPSData



class GPSDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = GPSData
        fields = '__all__'  # Inclure tous les champs
