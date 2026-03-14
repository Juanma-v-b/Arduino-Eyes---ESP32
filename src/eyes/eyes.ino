#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Coordenadas de los dos ojos
int cxIzq, cxDer, cy;
int eyeW = 45, eyeH = 30;        
int irisR = 14, pupilR = 6;       
int separacion = 70;             

// Control de mirada
float pupilX = 0, pupilY = 0;
float targetX = 0, targetY = 0;
float velocidad = 0.15;

// Estados de la mirada
enum EstadoMirada {
  MIRANDO_CENTRO,
  MOVIENDO_A_DERECHA,
  MANTENIENDO_DERECHA,
  MOVIENDO_A_IZQUIERDA,
  MANTENIENDO_IZQUIERDA,
  MOVIENDO_ARRIBA,
  MANTENIENDO_ARRIBA,
  MOVIENDO_ABAJO,
  MANTENIENDO_ABAJO,
  MOVIENDO_DIAGONAL
};

EstadoMirada estado = MIRANDO_CENTRO;
unsigned long tiempoCambio = 0;
int tiempoEspera = 0;

bool blinking = false;
float blinkT = 1.0;
unsigned long lastBlink = 0;
int blinkFrames = 0;

// Guardar estado anterior para cada ojo
float lastPupilX = 0, lastPupilY = 0;
float lastBlinkT = 1.0;

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);

  // Calcular posiciones de los dos ojos
  cy = tft.height() / 2;
  cxIzq = tft.width() / 2 - separacion;
  cxDer = tft.width() / 2 + separacion;

  randomSeed(analogRead(0));
  
  // Dibujar los ojos por primera vez
  drawEye(cxIzq, blinkT, 0, 0);
  drawEye(cxDer, blinkT, 0, 0);
  lastPupilX = 0;
  lastPupilY = 0;
  lastBlinkT = blinkT;
  
  tiempoCambio = millis();
}

void actualizarMirada() {
  unsigned long ahora = millis();
  
  switch(estado) {
    case MIRANDO_CENTRO:
      targetX = 0;
      targetY = 0;
      if (ahora - tiempoCambio > 2000) {
        int dir = random(4);
        if (dir == 0) {
          estado = MOVIENDO_A_DERECHA;
          targetX = 8;        // Ajustado para ojo más pequeño
          targetY = 0;
        } else if (dir == 1) {
          estado = MOVIENDO_A_IZQUIERDA;
          targetX = -8;
          targetY = 0;
        } else if (dir == 2) {
          estado = MOVIENDO_ARRIBA;
          targetX = 0;
          targetY = -5;
        } else {
          estado = MOVIENDO_ABAJO;
          targetX = 0;
          targetY = 5;
        }
        tiempoCambio = ahora;
      }
      break;
      
    case MOVIENDO_A_DERECHA:
    case MOVIENDO_A_IZQUIERDA:
    case MOVIENDO_ARRIBA:
    case MOVIENDO_ABAJO:
      if (abs(pupilX - targetX) < 0.8 && abs(pupilY - targetY) < 0.8) {
        if (estado == MOVIENDO_A_DERECHA) {
          estado = MANTENIENDO_DERECHA;
          tiempoEspera = random(800, 1500);
        } else if (estado == MOVIENDO_A_IZQUIERDA) {
          estado = MANTENIENDO_IZQUIERDA;
          tiempoEspera = random(800, 1500);
        } else if (estado == MOVIENDO_ARRIBA) {
          estado = MANTENIENDO_ARRIBA;
          tiempoEspera = random(800, 1500);
        } else if (estado == MOVIENDO_ABAJO) {
          estado = MANTENIENDO_ABAJO;
          tiempoEspera = random(800, 1500);
        }
        tiempoCambio = ahora;
      }
      break;
      
    case MANTENIENDO_DERECHA:
    case MANTENIENDO_IZQUIERDA:
    case MANTENIENDO_ARRIBA:
    case MANTENIENDO_ABAJO:
      if (ahora - tiempoCambio > tiempoEspera) {
        estado = MOVIENDO_DIAGONAL;
        if (estado == MANTENIENDO_DERECHA) {
          targetX = 6;
          targetY = random(2) ? 3 : -3;
        } else if (estado == MANTENIENDO_IZQUIERDA) {
          targetX = -6;
          targetY = random(2) ? 3 : -3;
        } else if (estado == MANTENIENDO_ARRIBA) {
          targetX = random(2) ? 4 : -4;
          targetY = -4;
        } else if (estado == MANTENIENDO_ABAJO) {
          targetX = random(2) ? 4 : -4;
          targetY = 4;
        }
        tiempoCambio = ahora;
      }
      break;
      
    case MOVIENDO_DIAGONAL:
      if (abs(pupilX - targetX) < 0.8 && abs(pupilY - targetY) < 0.8) {
        estado = MIRANDO_CENTRO;
        targetX = 0;
        targetY = 0;
        tiempoCambio = ahora;
      }
      break;
  }
  
  pupilX += (targetX - pupilX) * velocidad;
  pupilY += (targetY - pupilY) * velocidad;
}

void drawEye(int centroX, float blinkFactor, float pupilOffsetX, float pupilOffsetY) {
  bool parpadeoCambio = (abs(blinkFactor - lastBlinkT) > 0.01);
  bool pupilaCambio = (abs(pupilOffsetX - lastPupilX) > 0.3 || abs(pupilOffsetY - lastPupilY) > 0.3);
  
    if (centroX == cxDer) {
      lastBlinkT = blinkFactor;
      lastPupilX = pupilOffsetX;
      lastPupilY = pupilOffsetY;
    }

    tft.fillCircle(centroX + lastPupilX, cy + lastPupilY, irisR + 2, TFT_WHITE);
    tft.drawEllipse(centroX, cy, eyeW, eyeH * blinkFactor, TFT_BLACK);
    
    tft.fillCircle(centroX + pupilOffsetX, cy + pupilOffsetY, irisR, TFT_LIGHTGREY);
    tft.fillCircle(centroX + pupilOffsetX, cy + pupilOffsetY, pupilR, TFT_WHITE);
    tft.fillCircle(centroX + pupilOffsetX - 3, cy + pupilOffsetY - 2, 2, TFT_BLACK);
    
    if (centroX == cxDer) {
      lastPupilX = pupilOffsetX;
      lastPupilY = pupilOffsetY;
    }
  
}

void loop() {
  unsigned long now = millis();

  actualizarMirada();

  // Dibujar ambos ojos
  drawEye(cxIzq, blinkT, pupilX, pupilY);
  drawEye(cxDer, blinkT, pupilX, pupilY);

  delay(30);
}