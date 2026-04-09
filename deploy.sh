#!/bin/bash

cd /home/ec2-user/app

# detener contenedor anterior
docker stop flask-app || true
docker rm flask-app || true

# construir imagen
docker build -t flask-app .

# correr contenedor
docker run -d -p 80:5000 --name flask-app flask-app
