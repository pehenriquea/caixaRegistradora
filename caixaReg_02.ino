#include <Keypad.h> //Biblioteca para uso de teclados matriciais 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <FreeRTOS.h>
#include <ESP32Servo.h>
//#include <task.h>

// - - - - - - - - FreeRTOS
// Atribuicao das tarefas
/*TaskHandle_t taskVendaHandle;
TaskHandle_t taskCalculadoraHandle;
TaskHandle_t taskSaldoHandle;
TaskHandle_t taskValorMaximoHandle;

// Funções das tarefas
void taskVenda(void *pvParameters);
void taskCalculadora(void *pvParameters);
void taskSaldo(void *pvParameters);
void taskValorMaximo(void pvParameters);*/

// - - - - - - - - Display LCD com módulo I2C

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2); //Conectado SDA no D21 e e SCL no RXD

int option = 0; // Variável para rastrear a opção do menu
int vendaOption = 0; // Variável para rastrear a opção dentro do menu de VENDA
int calculadoraOption = 0; // Variável para rastrear a opção dentro do menu de CALCULADORA

float resultado = 0.0;
float operando = 0.0;
char operador = '\0';
bool novaOperacao = true;

float totalCompra = 0.0;
float saldo = 0.0; // Inicializar com o saldo correto

// - - - - - - - - Teclado Matricial
const byte numLinhas = 4; //Quantidade de linhas do teclado
const byte numColunas = 4; //Quantidade de colunas do teclado

//Matriz de caracteres referente aos botões do teclado
char teclasMatriz[numLinhas][numColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinosLinhas[numLinhas] = {23, 4, 15, 3}; //Pinos das linhas
byte pinosColunas[numColunas] = {2, 19, 18, 5}; //Pinos das Colunas

//Cria um objeto do tipo Keypad
Keypad meuteclado = Keypad(makeKeymap(teclasMatriz), pinosLinhas, pinosColunas, numLinhas, numColunas);

// - - - - - - - - Servor Motor
Servo myServo;
int servoPin = 13;  //Define o pino do servo
int liberado = 0;   //Variavel que define a liberação do servo

// - - - - - - - - LED
int pinLed = 27;

// - - - - - - - - Setup
void setup() {
  Serial.begin(9600); //Inicia a comunicação serial
  Serial.println("Aperte um botão..."); //Imprime mensagem no monitor serial

  myServo.attach(servoPin);     //Inicializa o pino do servo
  myServo.write(180);           //Inicializa a posição do servo

  lcd.init();

  pinMode(pinLed, OUTPUT);
}

void loop() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("1-VENDA");
  lcd.setCursor(9, 0);
  lcd.print("2-SALDO");
  lcd.setCursor(0, 1);
  lcd.print("3-CALCULADORA");
  char pressionado = meuteclado.waitForKey(); ///Verifica se alguma tecla foi pressionada

  if (pressionado) { //Se alguma tecla foi pressionada
    Serial.print("Botão pressionado: "); //Imprime mensagem no monitor serial
    Serial.println(pressionado); //Imprime tecla pressionada
  }

  switch(pressionado){
    case '1':
      
    break;
    case '2':
      escolherOpcaoSaldo();
    break;

    case '3':
      escolherOpcaoCalculadora(0);
    break;
  }

  if(pressionado == '*'){
    liberado = !liberado;
  }
  
  if (liberado == 0){

    myServo.write(140);

  } else {

  myServo.write(50);
  
  }

  checkSaldo();

}

// - - - - - - - - - - - Funções
void escolherOpcaoVenda(char key) {
  /*switch (vendaOption) {
    case 0:
      // Menu principal de VENDA
      switch (key) {
        case '1':
          vendaOption = 1;
          // Lógica para a opção 1 - Inserir valor da compra
          if (novaOperacao) {
            resultado = operando;
            operando = 0.0;
            novaOperacao = false;
          }
          if (isCalculadora) {
            lcd.print("1 - Inserir valor da compra");
            lcd.setCursor(0, 1);
            lcd.print("2 - Inserir código de produtos");
          } else {
            // Lógica para a opção 1 - Inserir valor da compra
            // ...
          }
          break;
        case '2':
          vendaOption = 2;
          // Lógica para a opção 2 - Inserir código de produtos
          lcd.print("Digite o código do produto:");
          break;
        case '0':
          option = 0; // Voltar para o menu principal
          break;
        default:
          // Opção inválida
          break;
      }
      break;
    case 1:
      // Lógica para a opção 1 - Inserir valor da compra
      if (key == '0') {
        // Voltar ao menu principal de VENDA
        vendaOption = 0;
      } else if (key == '1') {
        // Inserir valor da compra
        lcd.print("Digite o valor recebido pelo cliente:");
      } else if (key == '*') {
        // Copia o resultado final da operação (Conta)
        // ...
      } else if (key == 'C') {
        // Multiplica o número de produtos a serem vendidos
        // ...
      }
      break;
    case 2:
      // Lógica para a opção 2 - Inserir código de produtos
      if (key == '0') {
        // Voltar ao menu principal de VENDA
        vendaOption = 0;
      } else if (key >= '0' && key <= '9') {
        // Lógica para inserir código de produtos
        // ...
      }
      break;
  }

  */
}

void escolherOpcaoCalculadora(int resultado) {
  char valor1;
  int v1;
  char operando;
  char valor2;
  char v2;
  char igual = '0';
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(resultado);

  if(resultado == 0){
    valor1 = meuteclado.waitForKey();
    lcd.setCursor(0, 0);
    lcd.print(valor1);
    v1 = valor1 - '0';
    operando = meuteclado.waitForKey();
    lcd.setCursor(2, 0);

    switch(operando){
      case 'A':
        lcd.print("+");
      break;
      case 'B':
        lcd.print("-");
      break;
      case 'C':
        lcd.print("*");
      break;
    }

    valor2 = meuteclado.waitForKey();
  lcd.setCursor(4, 0);
  lcd.print(valor2);
  v2 = valor2 - '0';
  igual = meuteclado.waitForKey();
  lcd.setCursor(6, 0);
  lcd.print("=");

  lcd.setCursor(8, 0);

  switch(operando){
    case 'A':
      resultado = 0;
      resultado = v1 + v2;
      lcd.print(resultado);
    break;
    case 'B':
      resultado = 0;
      resultado = v1 - v2;
      lcd.print(resultado);
    break;
    case 'C':
      resultado = 0;
      resultado = v1 * v2;
      lcd.print(resultado);
    break;
  }

  }else{
    if(resultado >= 10){
       lcd.setCursor(3, 0);
    }else{
       lcd.setCursor(2, 0);
    }
   
    operando = meuteclado.waitForKey();

    switch(operando){
      case 'A':
        lcd.print("+");
      break;
      case 'B':
        lcd.print("-");
      break;
      case 'C':
        lcd.print("*");
      break;
    }

    valor2 = meuteclado.waitForKey();
    lcd.setCursor(4, 0);
    lcd.print(valor2);
    v2 = valor2 - '0';
    igual = meuteclado.waitForKey();
    lcd.setCursor(6, 0);
    lcd.print("=");

    lcd.setCursor(8, 0);

    switch(operando){
      case 'A':
        resultado += v2;
        lcd.print(resultado);
      break;
      case 'B':
        resultado -= v2;
        lcd.print(resultado);
      break;
      case 'C':
        resultado *= v2;
        lcd.print(resultado);
      break;
    }
  }

  operando = meuteclado.waitForKey();

  switch(operando){
    case 'D':
      lcd.clear();
      escolherOpcaoCalculadora(resultado);
    break;

    case '#':
    lcd.clear();
    break;
  }
}

void escolherOpcaoSaldo() {
  // Lógica para a opção SALDO
  float saldo = 1500;
  digitalWrite(pinLed, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R$");
  lcd.setCursor(3, 0);
  lcd.print(saldo);

  char pressionado = meuteclado.waitForKey();
  digitalWrite(pinLed, LOW);
}

void somarSaldo(float valor){

  saldo += valor;  

}

void checkSaldo(){

  if (saldo > 1500.0){
    digitalWrite(pinLed, HIGH); 
  }
}
