#define LED_alarme 3
#define LED_ocioso 2
#define Som 13
#define PIR 9

String estado = "VIGIA";
int contador = 0;
int contador_max = 0;
int ciclo = 0;
int estados[] = { 0, 0, 0, 0 };
/*
0: Led Alarme
1: Led Ocioso
2: Som
3: PIR
*/

void setup()
{
  Serial.begin(9600);
  pinMode(LED_alarme, OUTPUT);
  pinMode(LED_ocioso, OUTPUT);
  pinMode(Som, OUTPUT);
  pinMode(PIR, INPUT);
}

void loop()
{
  String comando = ler();
  estados[3] = digitalRead(PIR);
  
  if (comando == "ocioso") { novo_estado("OCIOSO"); Serial.println("estado alterado"); }
  if (comando == "vigia") { novo_estado("VIGIA"); Serial.println("estado alterado"); }
  
  if (estado == "VIGIA") {
    novo_contador(10000);
    if (contador < contador_max) {
      contador++;
    } else if (contador >= contador_max) {
      contador = 0;
      digitalWrite(LED_alarme, estados[0]);
      estados[0] = !estados[0];
      ciclo++;
    }
    
    if (estados[3] == HIGH) { novo_estado("ALARME"); }
  }
  
  
  if (estado == "ALARME") {
    novo_contador(3000);
    if (contador < contador_max) {
      contador++;
    } else if (contador >= contador_max) {
      contador = 0;
      digitalWrite(LED_alarme, estados[0]);
      estados[0] = !estados[0];
      digitalWrite(Som, estados[2]);
      estados[2] = !estados[2];
      ciclo++;
    }
  }
  
  
  if (estado == "OCIOSO") {
    novo_contador(8000);
    if (contador < contador_max) {
      contador++;
    } else if (contador >= contador_max) {
      contador = 0;
      estados[1] = 1;
      digitalWrite(LED_ocioso, estados[1]);
    }
  }
}

String ler() {
  String texto = "";
  if (Serial.available()) {
    texto = Serial.readStringUntil('\n');
    Serial.println("> " + texto);
  }
  return texto;
}

void novo_contador(int max) {
  if (contador_max != max) {
    contador = 0;
    contador_max = max;
    ciclo = 0;
  }
}

void novo_estado(String estado_novo) {
  contador = 0;
  ciclo = 0;
  estado = estado_novo;
  estados[0] = 0;
  estados[1] = 0;
  estados[2] = 0;
  digitalWrite(LED_alarme, estados[0]);
  digitalWrite(LED_ocioso, estados[1]);
  digitalWrite(Som, estados[2]);
}