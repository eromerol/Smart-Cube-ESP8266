# Cómo configurar nuestros nodos Alexa

## Instalar app
NOTA: Para hacer los pasos no es necesario tener un Alexa.
- Descargar la app Amazon Alexa y seguir los pasos para su configuración.
- Una vez la app esté configurada, le damos a Más -> Skills y juegos.
- Descargamos Spotify y Node-RED Smart Home Control y autorizamos el uso de ambas.

## Creamos los dispositivos a utilizar en Node-RED
- Nos vamos al siguiente enlace: https://red.cb-net.co.uk/
- Creamos una cuenta e iniciamos sesión.
- En https://red.cb-net.co.uk/devices añadimos los dispositivos necesarios junto a su descripción y la habitación en la que están.
- Estos dispositivos, una vez creados, deberían de aparecer en al app de Alexa, en el apartado de Dispositivos.

## Configuración en Node-RED
- Instalamos el paquete node-red-contrib-alexa-smart-home en Manege-Palette.
- Para poder usar los nodos necesitamos crear un nuevo autor, introduciendo los datos de la cuenta de https://red.cb-net.co.uk/
![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/1d11f738-f66b-44ba-9ae5-afb4abee3725)

- Para añadir los dispositivos que poseemos en la app de Alexa usamos el nodo: alexa-smart-home-v3-state.
- Para añadir el propio Alexa usamos el nodo: alexa-smart-home-v3.
