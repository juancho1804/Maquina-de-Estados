/**
 * @file statemachine.ino
 * @brief Implementación de una máquina de estados para un sistema de seguridad.
 * @author Juan Manuel Cerón Certuche
 * @author Jorge Ignacio Astudillo
 * @author Guillermo Segura
 */


 /**
 * @brief Configura la máquina de estados y sus transiciones.
 */
void setup_State_Machine()
{
  /**
   *Configurar transiciones 
  */
  stateMachine.AddTransition(INIT, BLOQUEO, []() { return currentInput == BLOQUEADO; });
  stateMachine.AddTransition(INIT, MONITOR_LUZ, []() { return currentInput == CLAVECORRECTA; });
  stateMachine.AddTransition(BLOQUEO, INIT, []() { return currentInput == TIME_OUT; });

  stateMachine.AddTransition(MONITOR_LUZ, ALERTA, []() { return currentInput == LUZ; });
  stateMachine.AddTransition(ALERTA, MONITOR_LUZ, []() { return currentInput == TIME_OUT; });
  stateMachine.AddTransition(MONITOR_LUZ, MONITOR_TEMP, []() { return currentInput == TIME_OUT; });
  stateMachine.AddTransition(MONITOR_TEMP, MONITOR_LUZ, []() { return currentInput == TIME_OUT; });

  stateMachine.AddTransition(MONITOR_TEMP, ALARMA, []() { return currentInput == TEMP; });
  stateMachine.AddTransition(ALARMA, MONITOR_TEMP, []() { return currentInput == TIME_OUT; });


  /**
  *configurar eventos de estado(entrada)
  */
  stateMachine.SetOnEntering(INIT, inputI);
	stateMachine.SetOnEntering(BLOQUEO, inputB);
  stateMachine.SetOnEntering(MONITOR_LUZ, inputM);
	stateMachine.SetOnEntering(ALERTA, inputA);
  stateMachine.SetOnEntering(MONITOR_TEMP, inputMo);
  stateMachine.SetOnEntering(ALARMA, inputAla);


/**
 * configurar eventos de estado(salida)
*/
  stateMachine.SetOnLeaving(INIT, outputI);
  stateMachine.SetOnLeaving(BLOQUEO, outputB);
  stateMachine.SetOnLeaving(MONITOR_LUZ, outputM);
	stateMachine.SetOnLeaving(ALERTA, outputA);
  stateMachine.SetOnLeaving(MONITOR_TEMP, outputMo);
  stateMachine.SetOnLeaving(ALARMA, outputAla);
}

/**
 * @brief Entrada para el estado INIT.
 */
void inputI()
{
  currentInput = Input::Unknown;
  Serial.println("Estado INIT");
  Serial.println("A   B   C   D   E   F");
  Serial.println("X                     ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTask_Seguridad.Start();
}
/**
 * @brief Salida para el estado INIT.
 */
void outputI()
{
  asyncTask_Seguridad.Stop();
}
/**
 * @brief Entrada para el estado BLOQUEO.
 */
void inputB()
{ 
  currentInput = Input::Unknown;
  Serial.println("Estado BLOQUEADO");
  Serial.println("A   B   C   D   E   F");
  Serial.println("    X                 ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTaskTime4.Start();
}
/**
 * @brief Salida para el estado BLOQUEO.
 */
void outputB()
{  
  asyncTaskTime4.Stop();
}
/**
 * @brief Entrada para el estado Monitor de Luz.
 */
void inputM()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Monitor de Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("        X             ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  asyncTask2MonitorL.Start();
  asyncTaskTime1.Start();  
}
/**
 * @brief Salida para el estado Monitor de Luz.
 */
void outputM()
{
  asyncTask2MonitorL.Stop();
  asyncTaskTime1.Stop();
}
/**
 * @brief Entrada para el estado Alerta Luz.
 */
void inputA()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Alerta Luz");
  Serial.println("A   B   C   D   E   F");
  Serial.println("            X         ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask3Alerta.Start();
  asyncTaskTime1.Start();
}
/**
 * @brief Salida para el estado Alerta Luz.
 */
void outputA()
{
  asyncTask3Alerta.Stop();
  asyncTaskTime1.Stop();
}

/**
 * @brief Entrada para el estado Temperatura.
 */
void inputMo()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Temperatura");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                X     ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask4Temperatura.Start();
  asyncTaskTime3.Start();
}

/**
 * @brief Salida para el estado Temperatura.
 */
void outputMo()
{
  asyncTask4Temperatura.Stop();
  asyncTaskTime3.Stop();
}

/**
 * @brief Entrada para el estado Alarma.
 */
void inputAla()
{
  currentInput = Input::Unknown;
  Serial.println("Estado Alarma");
  Serial.println("A   B   C   D   E   F");
  Serial.println("                    X ");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0); 
  asyncTask4Alarma.Start();
  asyncTaskTime4.Start();
}

/**
 * @brief Salida para el estado Alarma.
 */
void outputAla()
{
  asyncTask4Alarma.Stop();
  asyncTaskTime4.Stop();
}
