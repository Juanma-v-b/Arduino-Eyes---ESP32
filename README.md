# ESP32 Eyes 

![Arduino](https://img.shields.io/badge/plataforma-Arduino-blue)
![ESP32](https://img.shields.io/badge/board-ESP32-red)
![Display](https://img.shields.io/badge/display-ST7789-orange)
![Licencia](https://img.shields.io/badge/licencia-MIT-green)

Una simulación de ojos en movimiento minimalista para dispositivos ESP32 con pantalla TFT ST7789.

Este proyecto proporciona una interfaz divertida con dos ojos que se mueven para mirar a varios lados sincronizados entre sí, con una interfaz muy simple y limpia.

---

## Características

- Animación fluida de pupilas siguiendo patrones aleatorios.  
- Código optimizado para no recargar toda la pantalla en cada frame.  
- Fácil de personalizar tamaño del ojo, pupila e iris. 

---

## Hardware

- ESP32
- Pantalla TFT ST7789 (240x320)
- Conexión SPI

---

## Librerías

- TFT_eSPI
- SPI

---

## Instalación

1. Instalar la librería **TFT_eSPI**.
2. Configurar tu pantalla en el archivo `User_Setup.h`.
3. Subir el archivo `.ino` al ESP32.

---

## Personalización

Puedes cambiar:

- Tamaño del ojo: `eyeW` y `eyeH`  
- Tamaño del iris y pupila: `irisR`, `pupilR`  
- Velocidad de movimiento: `velocidad`  
- Separación entre ojos: `separacion`  

---

## Vista previa

![Vista previa](images/vista-previa.jpg)


