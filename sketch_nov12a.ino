int TRIG =7;
int ECO=6;
int ledPin = 12;
int piezoPin = 5;
int puntoDelay = 90;
int sosButtonPin = 2; // Pin del botón SOS
int holaButtonPin = 3; // Pin del botón Hola
int DURACION;
int DISTANCIA;


bool sosPressed = false; // Bandera para indicar si se ha pulsado el botón SOS
bool holaPressed = false; // Bandera para indicar si se ha pulsado el botón Hola
bool enableMorseCode = false; // Bandera para habilitar/deshabilitar el código Morse

// Definición de letras en morse
String letras[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // a-i
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // j-r
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // s-z
};

// Definición de números en morse
String numeros[] = {
  "-----", ".----", "..---", "...--", "....-", // 0-4
  ".....", "-....", "--...", "---..", "----." // 5-9
};

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(sosButtonPin, INPUT_PULLUP); // Botón SOS con pull-up
  pinMode(holaButtonPin, INPUT_PULLUP); // Botón Hola con pull-up
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  Serial.begin(9600);

  Serial.begin(9600);
  noTone(piezoPin); // Asegurarse de que el piezo esté en silencio al inicio
  
  // Inicializar las banderas como false
  sosPressed = true;
  holaPressed = true;
  digitalWrite(piezoPin,HIGH); 
  digitalWrite(piezoPin,LOW);
  //delay(DISTANCIA*10); //Espero (operacion) en segundos
  //digitalWrite(LED,LOW); //Apago Led
}

void loop()
{
  
  const int MAX_INTENTOS = 5;  // Número máximo de intentos
  int intentoActual = 0;  // Contador de intentos

  for (int intentoActual = 0; intentoActual < MAX_INTENTOS; ++intentoActual) {
    digitalWrite(TRIG, HIGH); // Enciendo el Trigger
    delayMicroseconds(10); // Espero 10 microsegundos
    digitalWrite(TRIG, LOW); // Apago el Trigger
    DURACION = pulseIn(ECO, HIGH); // Devuelve un valor de tiempo en microsegundos que lo asignamos a la variable DURACION. Es decir, espera por un pulso alto
    DISTANCIA = DURACION / 58.2; // Divido la DURACION en 58.2 para obtenerla en centímetros
    Serial.println(DISTANCIA); // Imprimo texto o muestro la línea Distancia
    delay(200); // Espero 200 microsegundos

    if (DISTANCIA <= 50 && DISTANCIA >= 10) // Si DISTANCIA es mayor a 50 y menor a 10
    {
      Serial.println("Hay gente"); // Imprimo mensaje indicando que hay gente
      break; // Salir del bucle
    }
    else
    {
      Serial.println("No hay nadie cerca"); // Imprimo mensaje indicando que no hay nadie cerca
    }

      // Puedes agregar más código aquí para otras acciones que quieras realizar en cada iteración
   }
  
 
  noTone(piezoPin);
  char ch;
  if (Serial.available()) // ¿Hay algo que leer del USB?
  {
    ch = Serial.read(); // Leer una letra
    Serial.print(ch); // Imprimir letra
    Serial.print(" = "); // Imprimir signo =

    if (ch >= 'a' && ch <= 'z')
    {
      flashSecuencia(letras[ch - 'a']);
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
      flashSecuencia(letras[ch - 'A']);
    }
    else if (ch >= '0' && ch <= '9')
    {
      flashSecuencia(numeros[ch - '0']);
    }
    else if (ch == ' ')
    {
      Serial.println("");
      delay(puntoDelay * 4); // Espacio entre palabras
    }
  }
  
  // Comprobar si se presionó alguno de los botones
  if (digitalRead(sosButtonPin) == LOW && !sosPressed)
  {
    flashSecuencia("...---..."); // Envía SOS
    sosPressed = true;
  }
  else if (digitalRead(holaButtonPin) == LOW && !holaPressed)
  {
    flashSecuencia("....---.-...-"); // Envía Hola
    holaPressed = true;
  }

  // Reiniciar las banderas cuando se sueltan los botones
  if (digitalRead(sosButtonPin) == HIGH)
  {
    sosPressed = false;
  }
  if (digitalRead(holaButtonPin) == HIGH)
  {
    holaPressed = false;
  }
}

void flashSecuencia(String secuencia)
{
  Serial.println(secuencia); // Imprimir el valor en morse
  int i = 0;
  while (secuencia[i] != NULL)
  {
    flashDotOrDash(secuencia[i]);
    i++;
  }
  delay(puntoDelay * 3); // Espacio entre letras
}

void flashDotOrDash(char dotOrDash)
{
  noTone(piezoPin);
  digitalWrite(ledPin, HIGH);
  tone(piezoPin, 600);
  if (dotOrDash == '.') //si es punto
  {
    delay(puntoDelay);
    noTone(piezoPin);

  }
  else // debe ser una - 
  {
    delay(puntoDelay * 3);
    noTone(piezoPin);

  }
  noTone(piezoPin);
  digitalWrite(ledPin, LOW);
  delay(puntoDelay); // espacio entre destellos
}
