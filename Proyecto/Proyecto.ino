/**
 * @file Proyecto.ino
 * @brief Implementación principal del proyecto que utiliza las definiciones y funciones anteriores.
 * @author Juan Manuel Cerón Certuche
 * @author Jorge Ignacio Astudillo
 * @author Guillermo Segura
 */


#include "pinout.h" /** Inclusión del archivo de definiciones de pines */
#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "AsyncTaskLib.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); /** Inicialización del objeto LCD */

/** Declaración de matriz de teclas utilizando filas y columnas */
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); /** Inicialización del objeto keypad */
const String password = "1234"; /**Contraseña para el sistema de seguridad */
int attempts = 0; /** Contador de intentos de contraseña */

int buzzer = 7; /** Pin para el buzzer */
int tempo = 140; /** Tempo para la melodía */
int wholenote = (60000 * 4) / tempo; /** Duración de una nota entera */

int divider = 0, noteDuration = 0; /** Variables para la generación de sonido */
int melody[] = {

  
  NOTE_C5,4, //1
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,

  NOTE_F5,2,
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;  /** Cantidad de notas en la melodía */

/** 
Declaracion de funciones y tareas asincronas
*/

String Escribir_Contrasenia(); 

void Seguridad();
AsyncTask asyncTask_Seguridad(0, Seguridad);

void MonitorL();
AsyncTask asyncTask2MonitorL(0, true, MonitorL);

void Alerta_led();
AsyncTask asyncTask3Alerta(0, true, Alerta_led);

void Temperatura();
AsyncTask asyncTask4Temperatura(500, true, Temperatura);

void AlarmaB();
AsyncTask asyncTask4Alarma(0, true, AlarmaB);

void Timeout_4sg();
AsyncTask asyncTaskTime1(4000, Timeout_4sg);
void Timeout_3sg();
AsyncTask asyncTaskTime2(3000, Timeout_3sg);
void Timeout_6sg();
AsyncTask asyncTaskTime3(6000, Timeout_6sg);
void Timeout_5sg();
AsyncTask asyncTaskTime4(5000, Timeout_5sg);

enum State
{
  INIT = 0,
  BLOQUEO = 1,
  MONITOR_LUZ = 2,
  ALERTA = 3,
  MONITOR_TEMP = 4,
  ALARMA = 5
};

enum Input
{
  CLAVECORRECTA = 0,
  BLOQUEADO = 1,
  TIME_OUT = 2,
  LUZ = 3,
  TEMP = 4,
  Unknown = 5,
};

StateMachine stateMachine(6, 9); /** Inicialización de la máquina de estados */
Input currentInput; /** Variable para el estado actual de entrada */

/**
 * @brief Configuración inicial del programa.
 */
void setup()
{
  Serial.begin(9600);  /** Inicialización de la comunicación serial */
  pinMode(pingreen, OUTPUT);  /** Configuración de pines como salidas */
  pinMode(pinblue, OUTPUT);
  pinMode(pinred, OUTPUT);
  Serial.println("Starting State Machine...");
  setup_State_Machine(); /** Configuración de la máquina de estados */
  Serial.println("Start Machine Started");
  stateMachine.SetState(State::INIT, false, true); /** Establecimiento del estado inicial */
  lcd.begin(16, 2); /** Inicialización del LCD */
}

/**
 * @brief Bucle principal del programa.
 */
void loop()
{
  /**
   Actualización de tareas asíncronas 
   */
  asyncTask_Seguridad.Update();
  asyncTask2MonitorL.Update();
  asyncTask3Alerta.Update();
  asyncTask4Temperatura.Update();
  asyncTask4Alarma.Update();
  
  asyncTaskTime1.Update();
  asyncTaskTime2.Update();
  asyncTaskTime3.Update();
  asyncTaskTime4.Update();

  /**
  Actualización de la máquina de estados
  */
  stateMachine.Update();
 
}

/**
 * @brief Función de temporización para 4 segundos.
 */
void Timeout_4sg()
{
  currentInput = Input::TIME_OUT;
}
/**
 * @brief Función de temporización para 3 segundos.
 */
void Timeout_3sg()
{
  currentInput = Input::TIME_OUT; 
 
}
/**
 * @brief Función de temporización para 6 segundos.
 */
void Timeout_6sg()
{
  currentInput = Input::TIME_OUT;  
}

/**
 * @brief Función de temporización para 5 segundos.
 */
void Timeout_5sg()
{
  currentInput = Input::TIME_OUT; 
}

/**
 * @brief Función para ingresar la contraseña por el teclado.
 * @return La contraseña ingresada como String.
 */
String Escribir_Contrasenia() 
{
  lcd.print("Escribir");
  lcd.setCursor(0, 1);
  lcd.print("Contrasenia: ");
  String result = "";
  char key;
  while (true) {
    key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print("*");
      result += key;
    } else if (key == '#') {
      /** Salir del bucle cuando se presiona '#' */
      break;
    }
  }
  lcd.clear();
  return result;

}

/**
 * @brief Función para el estado de seguridad.
 */
void Seguridad() 
{
  const int maxAttempts = 3;

  for (int intentos = 0; intentos < maxAttempts; ++intentos) {

    String enteredPassword = Escribir_Contrasenia();

    if (enteredPassword.equals(password)) {      
      lcd.print("Acceso permitido");
      digitalWrite(pingreen, HIGH);
      delay(1000);
      digitalWrite(pingreen, LOW);
      currentInput = Input::CLAVECORRECTA;
      return;  /** Sale de la función si la contraseña es correcta */
    } else {
      lcd.print("Contrasenia");
      lcd.setCursor(0,1);
      lcd.print("incorrecta");
      digitalWrite(pinblue, HIGH);
      delay(1000);
      digitalWrite(pinblue, LOW);
      delay(1000);
      lcd.clear();
    }
  }
    /** Si llega aquí, se han agotado los intentos */
    lcd.print("Bloqueado");
    digitalWrite(pinred, HIGH);
    delay(1000);  
    digitalWrite(pinred, LOW);
    currentInput = Input::BLOQUEADO;
    lcd.print(currentInput);
    delay(1000);
    lcd.clear();

}

/**
 * @brief Función para el estado de monitor de luz.
 */
void MonitorL()
{
  lcd.clear();
  lcd.setCursor(0,0);
  int outputValue = analogRead(photocellPin);
  lcd.print("Photo: ");
  lcd.setCursor(0, 1);
  lcd.print(outputValue);
  Serial.println(outputValue);
  delay(1000);
  
  if (outputValue < 40)
  {
    currentInput = Input::LUZ;
  }
}

/**
 * @brief Función para el estado de alerta con LED.
 */
void Alerta_led()
{
  lcd.print("Alerta   ");
  digitalWrite(pinred, HIGH);
  delay(500);
  digitalWrite(pinred, LOW);
  delay(200);
}

/**
 * @brief Función para el estado de monitor de temperatura.
 */
void Temperatura(){
  lcd.clear();
  lcd.setCursor(0,0);
  int analogValue = analogRead(TempT);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  Serial.print("Temperature: ");
  Serial.print(celsius);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(celsius);
  delay(1000);

  if(celsius > 30)
  {
    currentInput = Input::TEMP;
  }
}

/**
 * @brief Función para el estado de alarma con buzzer.
 */
void AlarmaB(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alarma");
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    /** Calcula la duracion de cada nota */
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}
