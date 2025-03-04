"""
URL configuration for projet_animal project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from . import views
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('animals.urls')),  # Ajout des URLs de l'application animals
    path('api/gps-data/', views.get_gps_data, name='get_gps_data'),
    path('api/animals/', views.get_animals, name='get_animals'),
    path('api/animals/<int:animal_id>/positions/', views.get_animal_positions, name='get_animal_positions'),
    path('api/position/', views.add_position, name='add_position'),
    path('api/external-gps/', views.get_external_gps_data, name='get_external_gps_data'),
] + static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
