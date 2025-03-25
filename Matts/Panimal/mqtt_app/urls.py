from django.urls import path, include
from rest_framework.routers import DefaultRouter
router = DefaultRouter()
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView
from .views import register, login
from django.urls import path
from .views import register, login, get_gps_data, get_last_n_gps_data


urlpatterns = [
    path('register/', register, name='register'),
    path('login/', login, name='login'),
    path('gps-data/', get_gps_data, name='get_gps_data'),
    path('gps-data/last/<int:n>/', get_last_n_gps_data, name='get_last_n_gps_data'),
]


