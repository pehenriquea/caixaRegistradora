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
int calc = 0;

float resultado = 0.0;
float operando = 0.0;
char operador = '\0';
bool novaOperacao = true;

float totalCompra = 0.0;
float auxSaldo;
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
      escolherOpcaoVenda(0);
      myServo.write(180);
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
}

// - - - - - - - - - - - Funções
void escolherOpcaoVenda(char key) {
  int flag = 0, troco, v1;
  char valor1, pressionado;
  String equation1 = "", change = "";

  lcd.setCursor(0, 0);
  resultado = escolherOpcaoCalculadora(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valor recebido: ");

  do{
    valor1 = meuteclado.waitForKey(); 

    if(valor1 != 'A' && valor1 != 'B' && valor1 != 'C' && valor1 != 'D' && valor1 != '#' && valor1 != '*'){
      equation1 += valor1;
      lcd.setCursor(0, 1);
      lcd.print("               ");
      lcd.setCursor(0, 1);
      lcd.print(equation1);
    }else{
      flag = 1;
    }
  }while(flag == 0);

  v1 = equation1.toInt();
  saldo += v1;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Troco: ");
  troco = v1 - calc;
  change = String(troco);

  if(troco > 0){
    lcd.print(change);
    saldo -= troco;
  }else{
    lcd.print("Erro!!");
    return ;
  }
  
  pressionado = meuteclado.waitForKey();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Caixa liberado!");
  myServo.write(50);
  pressionado = meuteclado.waitForKey();
}

int escolherOpcaoCalculadora(int resultado) {
  char valor1;
  int v1;
  char operando;
  char valor2;
  int v2;
  char igual = '0';
  int flag = 0;
  char aux;
  String equation1 = "";
  String equation2 = "";
  String eqFinal = "";
  String result = "";
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(resultado);

if(resultado == 0){
  do{
    valor1 = meuteclado.waitForKey(); 

    if(valor1 != 'A' && valor1 != 'B' && valor1 != 'C' && valor1 != 'D' && valor1 != '#' && valor1 != '*'){
      equation1 += valor1;
      lcd.clear();
      lcd.print(equation1);
    }else{
      flag = 1;
    }
  }while(flag == 0);
  
  operando = valor1;
  v1 = equation1.toInt();
  lcd.setCursor(15, 0);

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

  flag = 0;
  lcd.setCursor(0, 1);
  do{
      valor2 = meuteclado.waitForKey(); 

      if(valor2 != 'A' && valor2 != 'B' && valor2 != 'C' && valor2 != 'D' && valor2 != '#' && valor2 != '*'){
        equation2 += valor2;
        lcd.setCursor(0, 1);
        lcd.print(equation2);
      }else{
        flag = 1;
      }
    }while(flag == 0);

  lcd.setCursor(0,0);

  switch(operando){
    case 'A':
    aux = '+';
    break;

    case 'B':
    aux = '-';
    break;

    case 'C':
    aux = '*';
    break; 
  }

  lcd.clear();
  eqFinal = equation1 + aux + equation2 + "=";
  lcd.print(eqFinal);
  
  v2 = equation2.toInt();

  lcd.setCursor(0, 1);

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
  operando = meuteclado.waitForKey();

  v1 = resultado;
  lcd.setCursor(15, 0);

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

  flag = 0;
  lcd.setCursor(0, 1);
  do{
      valor2 = meuteclado.waitForKey(); 

      if(valor2 != 'A' && valor2 != 'B' && valor2 != 'C' && valor2 != 'D' && valor2 != '#' && valor2 != '*'){
        equation2 += valor2;
        lcd.setCursor(0, 1);
        lcd.print(equation2);
      }else{
        flag = 1;
      }
    }while(flag == 0);

  lcd.setCursor(0,0);

  switch(operando){
    case 'A':
    aux = '+';
    break;

    case 'B':
    aux = '-';
    break;

    case 'C':
    aux = '*';
    break; 
  }

  lcd.clear();
  result = String(resultado);
  eqFinal = result + aux + equation2 + "=";
  lcd.print(eqFinal);

  v2 = equation2.toInt();

  lcd.setCursor(0, 1);

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
}
  operando = meuteclado.waitForKey();

  switch(operando){
    case 'D':
      lcd.clear();
      escolherOpcaoCalculadora(resultado);
    break;

    case '#':
    lcd.clear();
    calc = resultado;
    break;
  }

  
}

void escolherOpcaoSaldo() {
  // Lógica para a opção SALDO
  float limite = 1000.0;

  if (saldo > limite){
    digitalWrite(pinLed, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("R$");
    lcd.setCursor(3, 0);
    lcd.print(saldo);
    lcd.setCursor(0, 1);
    lcd.print("Esvaziar Caixa!!");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("R$");
    lcd.setCursor(3, 0);
    lcd.print(saldo);
    lcd.setCursor(0, 1);
    lcd.print("Caixa bom!!");
  }

  char pressionado = meuteclado.waitForKey();
  digitalWrite(pinLed, LOW);
}
