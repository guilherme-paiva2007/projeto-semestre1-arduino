#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>

#define L_Red 11
#define L_Green 10
#define L_Blue 13

String entrada = "";
String senha = "";
String modo = "entrada";
bool trava_teclado = false;

LiquidCrystal LCD(A0, A1, A2, A3, A4, A5);

char Teclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte Coluna_PINS[4] = {9, 8, 7, 6};
byte Linha_PINS[4] = {5, 4, 3, 2};
byte Colunas = 4;
byte Linhas = 4;

Keypad Teclado = Keypad(makeKeymap(Teclas), Linha_PINS, Coluna_PINS, Linhas, Colunas);

void setup()
{
  Serial.begin(9600);
  pinMode(L_Red, OUTPUT);
  pinMode(L_Green, OUTPUT);
  pinMode(L_Blue, OUTPUT);
  senha.concat(EEPROM.read(0));
  senha.concat(EEPROM.read(1));
  senha.concat(EEPROM.read(2));
  senha.concat(EEPROM.read(3));
  LCD.begin(16, 2);
}

void loop()
{
  char tecla = Teclado.getKey();
  
  if (tecla && tecla != 'A' && tecla != 'B' && tecla != 'C' && tecla != 'D') { // Se pegar uma tecla nova, armazena ela
    if (trava_teclado == false) {
      entrada.concat(tecla);
    }
  } else if (tecla == 'A') { // Apaga o ultimo caractere
    if (entrada.length() > 0) {
      entrada = entrada.substring(0, entrada.length() - 1);
    }
  } else if (tecla == 'B') {
    entrada = "";
  } else if (tecla == 'C') {
    trava_teclado = !trava_teclado;
  } else if (tecla == 'D' && modo == "ocioso") {
    modo = "entrada";
    Serial.println("vigia");
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
    rgb(220, 100, 0);
  }
  
  if (entrada == "##*" && modo == "ocioso") { // Entra em modo de redefinir senha
  	entrada = "";
    modo = "redefinir";
  }
  
  if (entrada == "*#" && modo == "ocioso") { // Manda de volta ao modo de vigia
  	entrada = "";
    modo = "entrada";
    Serial.println("vigia");
  }
  
  if (entrada.length() >= 4 && modo == "entrada") { // Se a senha for correta, entra em modo ocioso
    if (entrada == senha) {
      modo = "ocioso";
      Serial.println("ocioso");
    }
    entrada = ""; // Limpa a entrada automaticamente
  }
  
  if (modo == "entrada") { // Mostrar parte na inferior ao digitar
    String texto_oculto = "";
    for (int i = 0; i < entrada.length(); i++) {
      texto_oculto.concat("*");
    }
    LCD.print(texto_oculto + "    ");
  } else {
    LCD.print(entrada + "    ");
  }
  
  if (entrada.length() >= 4 && modo == "ocioso") { // Reseta ao chegar em 4 caracteres
  	entrada = "";
  }
  
  if (entrada.length() >= 4 && modo == "redefinir") { // Mesmo do anterior, mas armazena a senha
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