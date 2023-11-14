/* Sistema Bluetooth para controle de chaves dos carros da Eletrônica Embarcada
 *  Desenvolvido por:    JORGE PALMA CONCEIÇÃO
 *  Data:                14/11/2023
 */

#include <Arduino.h>

#include <iostream>
#include <string>

#include <Wire.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>
#include <EEPROM.h>

// DECLARAÇÃO DE VARIÁVEL DE TEMPO DO LED
unsigned long tempo_atual;
unsigned long tempo_anterior;
const long intervalo = 45000; // Intervalo desejado em milissegundos (45 segundos)

/* Variáveis e funções relacionadas à EEPROM */
const int eeprom_Address_master = 0;
const int eeprom_Address_chave1 = 10;
const int eeprom_Address_chave2 = 20;
const int eeprom_Address_chave3 = 30;
const int eeprom_Address_chave4 = 40;

void set_senha_master();
void set_senha_padrao_carro_1();
void set_senha_padrao_carro_2();
void set_senha_padrao_carro_3();
void set_senha_padrao_carro_4();

void resetar_EEPROM();

bool ctrl_loop_liberar_chaves = false;
bool ctrl_loop_admin = false;
bool ctrl_loop_admin_salvar_senha = false;
/* ------------------------------------------ */

void opcoes_admin();

void salvar_senha_EEPROM(int numero_chave, int n_carro);
String get_string(int endereco_inicial);
void liberar_chave(int numero_chave);

#define TEMPO_ESPERA 50

BluetoothSerial SerialBT;

Servo myservo; // create servo object to control a servo

int servoPin = 26; // GPIO pin used to connect the servo control (digital out)

int ADC_Max = 4096; // This is the default ADC max value on the ESP32 (12 bit ADC width);
                    // this width can be set (in low-level oode) from 9-12 bits, for a
                    // a range of max values of 512-4096

int val; // variable to read the value from the analog pin

#define LED_PIN 2

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  SerialBT.begin("Controle_Chaves");
  Serial.println("O dispositivo foi iniciado. Pode parear agora! :)");

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);          // Standard 50hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
                                       // using SG90 servo min/max of 500us and 2400us
                                       // for MG995 large servo, use 1000us and 2000us,
                                       // which are the defaults, so this line could be
                                       // "myservo.attach(servoPin);"

  EEPROM.begin(512);
}

void loop()
{
  // Verifica se há dados recebidos via Bluetooth
  if (SerialBT.available())
  {
    // Lê os dados recebidos
    String data = SerialBT.readString();
    data.remove(data.length() - 1);
    data.remove(data.length() - 1);

    if (data.length() > 9)
    {
      SerialBT.println("A string é muito longa");
    }
    else
    {
      if (data.length() == 1)
      {
        int chave = atoi(data.c_str());
        liberar_chave(chave);
      }
      else
      {
        if (!strcmp(data.c_str(), "#reset"))
        {
          // Reseta os valores p/ o padrão
          resetar_EEPROM();
        }
        else if (!strcmp(data.c_str(), get_string(eeprom_Address_master).c_str()))
        {
          /* Rotina de administrador */
          opcoes_admin();
        }
        else
        {
          delay(TEMPO_ESPERA);
          SerialBT.println("Informação inválida!");
        }
      }
    }
  }
}

void set_senha_master()
{
  // Converte a string em bytes
  String data = "#Diabinho";

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(0, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(i + 1, bytes[i]);
  }

  EEPROM.commit();
  delay(TEMPO_ESPERA);
  SerialBT.println("Resetada a senha padrão do Master");
}

void set_senha_padrao_carro_1()
{
  // Converte a string em bytes
  String data = "#Carro1";

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave1, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave1 + i + 1, bytes[i]);
  }

  EEPROM.commit();
  delay(TEMPO_ESPERA);
  SerialBT.println("Resetada a senha padrão do carro 1");
}

void set_senha_padrao_carro_2()
{
  // Converte a string em bytes
  String data = "#Carro2";

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave2, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave2 + i + 1, bytes[i]);
  }

  EEPROM.commit();
  delay(TEMPO_ESPERA);
  SerialBT.println("Resetada a senha padrão do carro 2");
}

void set_senha_padrao_carro_3()
{
  // Converte a string em bytes
  String data = "#Carro3";

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave3, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave3 + i + 1, bytes[i]);
  }

  EEPROM.commit();
  delay(TEMPO_ESPERA);
  SerialBT.println("Resetada a senha padrão do carro 3");
}

void set_senha_padrao_carro_4()
{
  // Converte a string em bytes
  String data = "#Carro4";

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave4, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave4 + i + 1, bytes[i]);
  }

  EEPROM.commit();

  delay(TEMPO_ESPERA);
  SerialBT.println("Resetada a senha padrão do carro 4");
}

void resetar_EEPROM()
{

  set_senha_master();
  set_senha_padrao_carro_1();
  set_senha_padrao_carro_2();
  set_senha_padrao_carro_3();
  set_senha_padrao_carro_4();
}

String get_string(int endereco_inicial)
{
  String string = "";

  int tamanho = EEPROM.read(endereco_inicial);
  for (int i = 0; i < tamanho; i++)
  {
    char c = (char)EEPROM.readByte(endereco_inicial + i + 1);
    string += c;
  }

  return string;
}

void liberar_chave(int numero_chave)
{
  delay(TEMPO_ESPERA);
  SerialBT.println("Insira a senha: ");

  // Start no contador
  tempo_atual = millis();
  tempo_anterior = tempo_atual;

  while (!ctrl_loop_liberar_chaves)
  {
    tempo_atual = millis();
    if (tempo_atual - tempo_anterior >= intervalo)
    { // Atraso de 1 segundo (1000 milissegundos)
      ctrl_loop_liberar_chaves = true;

      tempo_anterior = tempo_atual; // Atualiza o tempo anterior para o próximo atraso
      SerialBT.println("Tempo excedido! Aplicação reiniciada");
    }

    // Implementar contador para não ficar aqui pra sempre
    if (SerialBT.available())
    {
      // Lê a senha
      String senha = SerialBT.readString();
      senha.remove(senha.length() - 1);
      senha.remove(senha.length() - 1);
      String senha_salva = "";

      switch (numero_chave)
      {
      case 1:
        senha_salva = get_string(eeprom_Address_chave1);
        if (!strcmp(senha.c_str(), senha_salva.c_str()))
        {
          // Rotina para girar o motor e liberar a chave.
          delay(TEMPO_ESPERA);
          SerialBT.println("Liberando chave do carro 1...");
          for (int pos = 0; pos <= 90; pos += 1)
          {
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          delay(3000);

          for (int pos = 90; pos >= 0; pos -= 1)
          {                     // goes from 180 degrees to 0 degrees
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          ctrl_loop_liberar_chaves = true;
        }
        else
        {
          SerialBT.println("Senha incorreta!");
        }

        break;

      case 2:
        senha_salva = get_string(eeprom_Address_chave2);
        if (!strcmp(senha.c_str(), senha_salva.c_str()))
        {
          // Rotina para girar o motor e liberar a chave.
          delay(TEMPO_ESPERA);
          SerialBT.println("Liberando chave do carro 2...");
          for (int pos = 0; pos <= 90; pos += 1)
          {
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          delay(3000);

          for (int pos = 90; pos >= 0; pos -= 1)
          {                     // goes from 180 degrees to 0 degrees
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }
          ctrl_loop_liberar_chaves = true;
        }
        else
        {
          SerialBT.println("Senha incorreta!");
        }

        break;

      case 3:
        senha_salva = get_string(eeprom_Address_chave3);
        if (!strcmp(senha.c_str(), senha_salva.c_str()))
        {
          // Rotina para girar o motor e liberar a chave.
          delay(TEMPO_ESPERA);
          SerialBT.println("Liberando chave do carro 3...");
          for (int pos = 0; pos <= 90; pos += 1)
          {
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          delay(3000);

          for (int pos = 90; pos >= 0; pos -= 1)
          {                     // goes from 180 degrees to 0 degrees
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }
          ctrl_loop_liberar_chaves = true;
        }
        else
        {
          SerialBT.println("Senha incorreta!");
        }

        break;

      case 4:
        senha_salva = get_string(eeprom_Address_chave4);
        if (!strcmp(senha.c_str(), senha_salva.c_str()))
        {
          // Rotina para girar o motor e liberar a chave.
          delay(TEMPO_ESPERA);
          SerialBT.println("Liberando chave do carro 4...");
          for (int pos = 0; pos <= 90; pos += 1)
          {
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          delay(3000);

          for (int pos = 90; pos >= 0; pos -= 1)
          {                     // goes from 180 degrees to 0 degrees
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(1);           // waits 15ms for the servo to reach the position
          }

          ctrl_loop_liberar_chaves = true;
        }
        else
        {
          SerialBT.println("Senha incorreta!");
        }

        break;

      default:
        SerialBT.println("Numero da chave inválido! Tente novamente");
        break;
      }

      delay(TEMPO_ESPERA);
      if (ctrl_loop_liberar_chaves)
        break;
    }
  }

  ctrl_loop_liberar_chaves = false;
  SerialBT.println("Saiu do loop");
}

void opcoes_admin()
{
  delay(TEMPO_ESPERA);
  SerialBT.println("Entrou nas opções de admin");

  SerialBT.println("Menu do admin: ");
  SerialBT.println("- Digite de 1 a 4 p/ alterar as senhas dos carros");
  SerialBT.println("- Digite 5 p/ alterar a senha master");

  // Start no contador
  tempo_atual = millis();
  tempo_anterior = tempo_atual;

  while (!ctrl_loop_admin)
  {
    tempo_atual = millis();
    if (tempo_atual - tempo_anterior >= intervalo)
    { // Atraso de 1 segundo (1000 milissegundos)
      ctrl_loop_admin = true;

      tempo_anterior = tempo_atual; // Atualiza o tempo anterior para o próximo atraso
      SerialBT.println("Tempo excedido! Aplicação reiniciada");
    }

    if (SerialBT.available())
    {
      // Lê a senha
      String valor = SerialBT.readString();
      valor.remove(valor.length() - 1);
      valor.remove(valor.length() - 1);

      int endereco_inicial = 0;

      switch (atoi(valor.c_str()))
      {
      case 1:
        endereco_inicial = eeprom_Address_chave1;
        salvar_senha_EEPROM(endereco_inicial, atoi(valor.c_str()));
        ctrl_loop_admin = true;
        break;

      case 2:
        endereco_inicial = eeprom_Address_chave2;
        salvar_senha_EEPROM(endereco_inicial, atoi(valor.c_str()));
        ctrl_loop_admin = true;
        break;

      case 3:
        endereco_inicial = eeprom_Address_chave3;
        salvar_senha_EEPROM(endereco_inicial, atoi(valor.c_str()));
        ctrl_loop_admin = true;
        break;

      case 4:
        endereco_inicial = eeprom_Address_chave4;
        salvar_senha_EEPROM(endereco_inicial, atoi(valor.c_str()));
        ctrl_loop_admin = true;
        break;

      case 5:
        endereco_inicial = eeprom_Address_master;
        salvar_senha_EEPROM(endereco_inicial, atoi(valor.c_str()));
        ctrl_loop_admin = true;
        break;

      default:
        SerialBT.println("Valor inválido! Reiniciando sistema.");
        ctrl_loop_admin = true;
        break;
      }

      delay(TEMPO_ESPERA);
      if (ctrl_loop_admin)
        break;
    }
  }

  ctrl_loop_admin = false;
  delay(TEMPO_ESPERA);
  SerialBT.println("Saiu do loop de admin!");
}

void salvar_senha_EEPROM(int endereco_inicial, int n_carro)
{
  delay(TEMPO_ESPERA);
  SerialBT.println("Entrou em salvar senha");

  SerialBT.println("Insira a senha desejada: ");

  // Start no contador
  tempo_atual = millis();
  tempo_anterior = tempo_atual;

  while (!ctrl_loop_admin_salvar_senha)
  {

    tempo_atual = millis();
    if (tempo_atual - tempo_anterior >= intervalo)
    { // Atraso de 1 segundo (1000 milissegundos)
      ctrl_loop_admin_salvar_senha = true;

      tempo_anterior = tempo_atual; // Atualiza o tempo anterior para o próximo atraso
      SerialBT.println("Tempo excedido! Aplicação reiniciada");
    }

    if (SerialBT.available())
    {
      // Lê a senha
      String senha = SerialBT.readString();
      senha.remove(senha.length() - 1);
      senha.remove(senha.length() - 1);

      if (senha.length() < 10)
      {
        byte bytes[senha.length() + 1];
        senha.getBytes(bytes, senha.length() + 1);

        EEPROM.write(endereco_inicial, senha.length());

        // Escreve os bytes da string na EEPROM
        for (int i = 0; i < senha.length(); i++)
        {
          EEPROM.write(endereco_inicial + i + 1, bytes[i]);
        }

        EEPROM.commit();

        delay(TEMPO_ESPERA);

        if (n_carro == 5)
        {
          SerialBT.println("Finalizada a inserção da nova senha master!");
        }
        else
        {
          SerialBT.print("Finalizada a inserção da nova senha p/ carro ");
          SerialBT.println(n_carro);
        }
      }
      else
      {
        delay(TEMPO_ESPERA);
        SerialBT.println("Senha muito longa! Reiniciando o sistema.");
      }

      ctrl_loop_admin_salvar_senha = true;
    }
  }
}