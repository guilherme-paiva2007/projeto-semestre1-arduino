#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>

#define L_Red 11
#define L_Green 10
#define L_Blue 13

String entrada = "";
String senha = "";
String modo = "entrada";

LiquidCrystal LCD(A0, A1, A2, A3, A4, A5);

char Teclas[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte Coluna_PINS[3] = {5, 4, 3};
byte Linha_PINS[4] = {9, 8, 7, 6};
byte Colunas = 3;
byte Linhas = 4;

Keypad Teclado = Keypad(makeKeymap(Teclas), Linha_PINS, Coluna_PINS, Linhas, Colunas);

void setup()
{
  Serial.begin(9600);
  pinMode(L_Red, OUTPUT);
  pinMode(L_Green, OUTPUT);
  pinMode(L_Blue, OUTPUT);
  senha.concat(char(EEPROM.read(0)));
  senha.concat(char(EEPROM.read(1)));
  senha.concat(char(EEPROM.read(2)));
  senha.concat(char(EEPROM.read(3)));
  LCD.begin(16, 2);
}

void loop()
{
  String comando = ler();
  char tecla = Teclado.getKey();

  if (comando == "redefinir") { modo = "redefinir"; }
  
  if (tecla) {
  	entrada.concat(tecla);
  }
  
  if (modo == "entrada") {
    LCD.setCursor(0, 0);
    LCD.print("Insira a senha:");
    LCD.setCursor(0, 1);
    rgb(255, 0, 0);
  }
  if (modo == "ocioso") {
    LCD.setCursor(0, 0);
    LCD.print("Inativo        ");
    LCD.setCursor(0, 1);
    rgb(0, 120, 255);
  }
  if (modo == "redefinir") {
    LCD.setCursor(0, 0);
    LCD.print("Defina a senha:");
    LCD.setCursor(0, 1);
    rgb(180, 100, 0);
  }
  
  if (entrada == "##*" && modo == "ocioso") {
  	entrada = "";
    modo = "redefinir";
  }
  
  if (entrada.length() >= 4 && modo == "entrada") {
    if (entrada == senha) {
      modo = "ocioso";
      Serial.println("ocioso");
    }
    entrada = "";
  }
  
  if (entrada == "*#" && modo == "ocioso") {
  	entrada = "";
    modo = "entrada";
    Serial.println("vigia");
  }
  
  if (modo == "entradaaa") {
    String texto_oculto = "";
    for (int i = 0; i < entrada.length(); i++) {
      texto_oculto.concat("*");
    }
    LCD.print(texto_oculto + "    ");
  } else {
    String texto_vazio = "";
    for (int i = 4; i > entrada.length(); i--) {
      texto_vazio.concat(" ");
    }
    LCD.print(entrada + texto_vazio);
  }
  
  if (entrada.length() >= 4 && modo == "ocioso") {
  	entrada = "";
  }
  
  if (entrada.length() >= 4 && modo == "redefinir") {
    EEPROM.write(0, entrada[0]);
    EEPROM.write(1, entrada[1]);
    EEPROM.write(2, entrada[2]);
    EEPROM.write(3, entrada[3]);
    senha = "";
    senha.concat(char(EEPROM.read(0)));
  	senha.concat(char(EEPROM.read(1)));
  	senha.concat(char(EEPROM.read(2)));
  	senha.concat(char(EEPROM.read(3)));
    entrada = "";
    modo = "ocioso";
  }
}

void rgb(int red, int green, int blue) {
  analogWrite(L_Red, red);
  analogWrite(L_Green, green);
  analogWrite(L_Blue, blue);
}

String ler() {
  String texto = "";
  if (Serial.available()) {
    texto = Serial.readStringUntil('\n');
    Serial.println("> " + texto);
  }
  return texto;
}