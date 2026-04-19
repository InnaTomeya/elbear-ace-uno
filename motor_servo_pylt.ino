#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>          

// Пины nRF24L01+
#define NRF_CE_PIN  9
#define NRF_CSN_PIN 10

// Пины моторов (КР1128КТ - два цифровых пина на мотор)
#define MOTOR1_IN1 4
#define MOTOR1_IN2 5

#define MOTOR2_IN1 6
#define MOTOR2_IN2 3

#define SERVO_PIN 8          // Пин для сервопривода

#define ACK_MSG_LEN 7

const byte address[][ACK_MSG_LEN] = { "EMech", "ElCar" };

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);
Servo servo;                

struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte joy2_X;
  byte joy2_Y;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};
Data_Package data;

struct PayloadStruct {
  char message[ACK_MSG_LEN];
  uint8_t counter;
};
PayloadStruct payload;

char ACKmsg[ACK_MSG_LEN] = "RecACK";

unsigned long lastReceiveTime = 0;
#define TIME_NO_CON 500
bool conFlag = false;

int currentServoAngle = 90;  // Текущий угол сервопривода
const int servoCenter = 90;  // Центр (ровно)
const int servoStep = 5;     // Шаг изменения угла за цикл (чем меньше — тем плавнее)

void setup() {
  Serial.begin(9600);
  Serial.println("Receiver started");

  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);

  stopMotor(MOTOR1_IN1, MOTOR1_IN2);
  stopMotor(MOTOR2_IN1, MOTOR2_IN2);

  servo.attach(SERVO_PIN);     
  servo.write(servoCenter);    // Начальный угол (центр)

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();

  radio.openReadingPipe(1, address[1]);
  radio.startListening();

  memcpy(payload.message, ACKmsg, ACK_MSG_LEN);
  payload.counter = 0;
}

void stopMotor(uint8_t in1, uint8_t in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// val - значение оси Y джойстика (0..255)
void controlMotor(byte val, uint8_t in1, uint8_t in2) {
  int16_t joy = (int16_t)val - 127; // центрируем
  if (joy > 10) {
    // Вперёд
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (joy < -10) {
    // Назад
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    // Стоп (мёртвая зона)
    stopMotor(in1, in2);
  }
}

// Функция плавного перехода currentAngle к targetAngle с шагом step
int smoothMove(int currentAngle, int targetAngle, int step) {
  if (currentAngle < targetAngle) {
    currentAngle += step;
    if (currentAngle > targetAngle) currentAngle = targetAngle;
  } else if (currentAngle > targetAngle) {
    currentAngle -= step;
    if (currentAngle < targetAngle) currentAngle = targetAngle;
  }
  return currentAngle;
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));

    payload.counter++;
    radio.writeAckPayload(1, &payload, sizeof(PayloadStruct));

    lastReceiveTime = millis();
    conFlag = true;

    controlMotor(data.joy1_Y, MOTOR1_IN1, MOTOR1_IN2);
    controlMotor(data.joy2_Y, MOTOR2_IN1, MOTOR2_IN2);

    // Целевой угол сервопривода по оси X второго джойстика (0..255 -> 0..180)
    int targetServoAngle = map(data.joy2_X, 0, 255, 0, 180);

    // Плавно меняем угол
    currentServoAngle = smoothMove(currentServoAngle, targetServoAngle, servoStep);
    servo.write(currentServoAngle);

    Serial.print("joy1_Y: ");
    Serial.print(data.joy1_Y);
    Serial.print(" | joy2_Y: ");
    Serial.print(data.joy2_Y);
    Serial.print(" | servo angle: ");
    Serial.println(currentServoAngle);

  } else {
    if (millis() - lastReceiveTime > TIME_NO_CON) {
      if (conFlag) {
        Serial.println("Потеря связи!");
        conFlag = false;
        stopMotor(MOTOR1_IN1, MOTOR1_IN2);
        stopMotor(MOTOR2_IN1, MOTOR2_IN2);

        // Плавно возвращаем сервопривод в центр
        currentServoAngle = smoothMove(currentServoAngle, servoCenter, servoStep);
        servo.write(currentServoAngle);
      }
    }
  }

  delay(10);
}
