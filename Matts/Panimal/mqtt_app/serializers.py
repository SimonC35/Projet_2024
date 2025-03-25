from rest_framework_gis.serializers import GeoFeatureModelSerializer
from rest_framework import serializers
from .models import Donnee



class DonneeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Donnee
        fields = '__all__'  # Inclure tous les champs

