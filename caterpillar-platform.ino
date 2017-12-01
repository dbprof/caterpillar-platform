/**************************************************************
caterpillar-platform.ino
BlackBug Engineering
01.12.2017
https://github.com/dbprof/caterpillar-platform
***************************************************************/

///////////////////////////ШИЛД///////////////////////////
//Подключаем библиотеку для работы с I2C
#include <Wire.h>

//Подключаем библиотеку шилда моторов
#include <Adafruit_MotorShield.h>

//Подключаем библиотеку драйвера моторов Adafruit
#include "utility/Adafruit_MS_PWMServoDriver.h"

//Создаем объект шилда моторов с адресом I2C по умолчанию
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

//Создаем объекты правого и левого моторов
//Правый мотор подключен к выходу 3
Adafruit_DCMotor *RightMotor = AFMS.getMotor(3);
//Левый мотор подключен к выходу 4
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(4);

///////////////////////////REMOTEXY///////////////////////////
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>

#include <RemoteXY.h>

#define REMOTEXY_SERIAL_RX 12
#define REMOTEXY_SERIAL_TX 11
#define REMOTEXY_SERIAL_SPEED 9600 

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,0,0,17,0,6,0,0,
  5,32,13,16,30,30,2,5,32,60,
  16,30,30,2 };
  
struct {
  int8_t left_caterpillar_x; // =-100..100 x-coordinate joystick position 
  int8_t left_caterpillar_y; // =-100..100 y-coordinate joystick position 
  int8_t right_caterpillar_x; // =-100..100 x-coordinate joystick position 
  int8_t right_caterpillar_y; // =-100..100 y-coordinate joystick position 

  uint8_t connect_flag;  // =1 if wire connected, else =0 
} RemoteXY;
#pragma pack(pop) 




//Создаем ограничение максимальной скорости для каждого мотора
int iMaxSpeedRM = 50; //50
int iMaxSpeedLM = 50; //50

void setup() {
  //Открываем последовательный порт для отладки
  Serial.begin(9600);

  //Создаем объект шилда мотор моторов на частоте по умолчанию 1.6KHz
  AFMS.begin();

  //Инициируем интерфейс управления
  RemoteXY_Init ();
}

void loop() 
{
  RemoteXY_Handler ();
  
  int8_t right_val_cur = RemoteXY.right_caterpillar_y;
  int8_t left_val_cur = RemoteXY.left_caterpillar_y;
  
  RightMotor->setSpeed(abs(right_val_cur)*5);
  LeftMotor->setSpeed(abs(left_val_cur)*5);
  
  if (right_val_cur > 0){
    RightMotor->run(FORWARD);
  }
  else if (right_val_cur < 0){
    RightMotor->run(BACKWARD);
  }
  else{
    RightMotor->run(RELEASE);
  }

  if (left_val_cur > 0){
    LeftMotor->run(FORWARD);
  }
  else if (left_val_cur < 0){
    LeftMotor->run(BACKWARD);
  }
  else{
    LeftMotor->run(RELEASE);
  }
}
