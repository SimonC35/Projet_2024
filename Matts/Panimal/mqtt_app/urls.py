from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import get_gps_data
from .views import get_last_n_gps_data
router = DefaultRouter()



urlpatterns = [
    path('gps-data/', get_gps_data, name='gps-data'),
    path('gps-data/last/<int:n>/', get_last_n_gps_data, name='get_last_n_gps_data'),
]


