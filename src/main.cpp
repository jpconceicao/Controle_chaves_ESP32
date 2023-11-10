#include <Arduino.h>

#include <iostream>
#include <string>

#include <Wire.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>

#include <EEPROM.h>

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

void opcoes_admin();

void salvar_senha_EEPROM(int numero_chave, String senha);
String get_string(int endereco_inicial);
void liberar_chave(int numero_chave);

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
        if (!strcmp(data.c_str(), "reset"))
        {
          // Reseta os valores p/ o padrão
          resetar_EEPROM();
        }

        if (!strcmp(data.c_str(), get_string(eeprom_Address_master).c_str()))
        {
          /* Rotina de administrador */
          opcoes_admin();
        }
      }
    }
  }
}

void set_senha_master()
{
  // Converte a string em bytes
  String data = "#Diabinho";
  Serial.println(data.length());

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(0, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(i + 1, bytes[i]);
    Serial.print("Inserindo byte: ");
    Serial.println(bytes[i]);
  }

  EEPROM.commit();
}

void set_senha_padrao_carro_1()
{
  // Converte a string em bytes
  String data = "#Carro1";
  Serial.println(data.length());

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave1, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave1 + i + 1, bytes[i]);
    Serial.print("Inserindo byte: ");
    Serial.println(bytes[i]);
  }

  EEPROM.commit();
}

void set_senha_padrao_carro_2()
{
  // Converte a string em bytes
  String data = "#Carro2";
  Serial.println(data.length());

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave2, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave2 + i + 1, bytes[i]);
    Serial.print("Inserindo byte: ");
    Serial.println(bytes[i]);
  }

  EEPROM.commit();
}

void set_senha_padrao_carro_3()
{
  // Converte a string em bytes
  String data = "#Carro3";
  Serial.println(data.length());

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave3, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave3 + i + 1, bytes[i]);
    Serial.print("Inserindo byte: ");
    Serial.println(bytes[i]);
  }

  EEPROM.commit();
}

void set_senha_padrao_carro_4()
{
  // Converte a string em bytes
  String data = "#Carro4";
  Serial.println(data.length());

  byte bytes[data.length() + 1];
  data.getBytes(bytes, data.length() + 1);

  EEPROM.write(eeprom_Address_chave4, data.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < data.length(); i++)
  {
    EEPROM.write(eeprom_Address_chave4 + i + 1, bytes[i]);
    Serial.print("Inserindo byte: ");
    Serial.println(bytes[i]);
  }

  EEPROM.commit();
}

void resetar_EEPROM()
{

  set_senha_master();
  set_senha_padrao_carro_1();
  set_senha_padrao_carro_2();
  set_senha_padrao_carro_3();
  set_senha_padrao_carro_4();
}

void salvar_senha_EEPROM(int endereco_inicial, String senha)
{
  // Converte a string em bytes

  Serial.println(senha.length());

  byte bytes[senha.length() + 1];
  senha.getBytes(bytes, senha.length() + 1);

  EEPROM.write(0, senha.length());

  // Escreve os bytes da string na EEPROM
  for (int i = 0; i < senha.length(); i++)
  {
    EEPROM.write(i + 1, bytes[i]);
  }

  EEPROM.commit();

  SerialBT.println("Finalizada a inserção da nova senha!");
}

String get_string(int endereco_inicial)
{
  String string = "";

  int tamanho = EEPROM.read(endereco_inicial);
  Serial.println(tamanho);
  for (int i = 0; i < tamanho; i++)
  {
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(EEPROM.readByte(endereco_inicial + i + 1));

    char c = (char)EEPROM.readByte(endereco_inicial + i + 1);
    string += c;
    // Imprime a string.
    Serial.println(c);
  }

  return string;
}

void liberar_chave(int numero_chave)
{
  while (!SerialBT.available())
  {

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
          SerialBT.println("Liberar chave do carro 1");
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
          SerialBT.println("Liberar chave do carro 2");
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
          SerialBT.println("Liberar chave do carro 3");
        }
        else
        {
          SerialBT.println("Senha incorreta");
        }
        break;

      case 4:
        senha_salva = get_string(eeprom_Address_chave4);
        if (!strcmp(senha.c_str(), senha_salva.c_str()))
        {
          // Rotina para girar o motor e liberar a chave.
          SerialBT.println("Liberar chave do carro 4");
        }
        else
        {
          SerialBT.println("Senha incorreta");
        }
        break;

      default:
        SerialBT.println("Numero da chave inválido! ");
        break;
      }

      break;
    }
  }

  SerialBT.println("Saiu do loop");
}

void opcoes_admin()
{
}
