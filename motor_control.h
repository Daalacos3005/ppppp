#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// --- DRIVER 1 (HW-039 con PWM)
#define DRIVER1_LPWM 22
#define DRIVER1_RPWM 23
#define CH_D1_LPWM 0
#define CH_D1_RPWM 1

// --- DRIVER 2 y 3 (L298N SIN PWM)
#define D2_IN1 21
#define D2_IN2 19
#define D2_IN3 4
#define D2_IN4 15

#define D3_IN1 18
#define D3_IN2 5
#define D3_IN3 14   // CAMBIADO: antes era pin 2 (LED azul), ahora es pin 14
#define D3_IN4 0

// --- Servos de dirección (PWM)
#define SERVO1_PIN 17
#define SERVO2_PIN 16
#define SERVO1_CH 2
#define SERVO2_CH 3

// PWM configuración
#define PWM_FREQ_MOTOR 1000
#define PWM_RES_MOTOR 10
#define PWM_FREQ_SERVO 50
#define PWM_RES_SERVO 16

void iniciarDrivers() {
  // Driver 1 (HW-039 con PWM)
  ledcSetup(CH_D1_LPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);
  ledcAttachPin(DRIVER1_LPWM, CH_D1_LPWM);

  ledcSetup(CH_D1_RPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);
  ledcAttachPin(DRIVER1_RPWM, CH_D1_RPWM);

  // L298N
  pinMode(D2_IN1, OUTPUT); pinMode(D2_IN2, OUTPUT);
  pinMode(D2_IN3, OUTPUT); pinMode(D2_IN4, OUTPUT);
  pinMode(D3_IN1, OUTPUT); pinMode(D3_IN2, OUTPUT);
  pinMode(D3_IN3, OUTPUT); pinMode(D3_IN4, OUTPUT);

  // Servos (PWM sin librería)
  ledcSetup(SERVO1_CH, PWM_FREQ_SERVO, PWM_RES_SERVO);
  ledcAttachPin(SERVO1_PIN, SERVO1_CH);

  ledcSetup(SERVO2_CH, PWM_FREQ_SERVO, PWM_RES_SERVO);
  ledcAttachPin(SERVO2_PIN, SERVO2_CH);
}

// -------- DRIVER 1 (HW-039) --------
void avanzar(int duty) {
  ledcWrite(CH_D1_LPWM, duty);
  ledcWrite(CH_D1_RPWM, 0);

  digitalWrite(D2_IN1, HIGH); digitalWrite(D2_IN2, LOW);
  digitalWrite(D2_IN3, LOW);  digitalWrite(D2_IN4, HIGH);
  digitalWrite(D3_IN1, HIGH); digitalWrite(D3_IN2, LOW);
  digitalWrite(D3_IN3, LOW);  digitalWrite(D3_IN4, HIGH);
}

void retroceder(int duty) {
  ledcWrite(CH_D1_LPWM, 0);
  ledcWrite(CH_D1_RPWM, duty);

  digitalWrite(D2_IN1, LOW);  digitalWrite(D2_IN2, HIGH);
  digitalWrite(D2_IN3, HIGH); digitalWrite(D2_IN4, LOW);
  digitalWrite(D3_IN1, LOW);  digitalWrite(D3_IN2, HIGH);
  digitalWrite(D3_IN3, HIGH); digitalWrite(D3_IN4, LOW);
}

void detener() {
  ledcWrite(CH_D1_LPWM, 0);
  ledcWrite(CH_D1_RPWM, 0);

  digitalWrite(D2_IN1, LOW); digitalWrite(D2_IN2, LOW);
  digitalWrite(D2_IN3, LOW); digitalWrite(D2_IN4, LOW);
  digitalWrite(D3_IN1, LOW); digitalWrite(D3_IN2, LOW);
  digitalWrite(D3_IN3, LOW); digitalWrite(D3_IN4, LOW);
}

// -------- SERVOS --------
void direccionCentro() {
  int duty = 65535 * 0.075; // 1.5 ms
  ledcWrite(SERVO1_CH, duty);
  ledcWrite(SERVO2_CH, duty);
}

void girarIzquierdaServos() {
  int duty = 65535 * 0.05; // 1.0 ms
  ledcWrite(SERVO1_CH, duty);
  ledcWrite(SERVO2_CH, duty);
}

void girarDerechaServos() {
  int duty = 65535 * 0.10; // 2.0 ms
  ledcWrite(SERVO1_CH, duty);
  ledcWrite(SERVO2_CH, duty);
}

#endif
