// Определяем пины
const int buttonPin = 4;  // Пин для кнопки
const int motorForwardPin = 3; // Пин для движения вперед
const int motorBackwardPin = 5; // Пин для движения назад

void setup() {
  // Инициализация пинов
  pinMode(buttonPin, INPUT_PULLUP); // Устанавливаем кнопку как вход с подтяжкой
  pinMode(motorForwardPin, OUTPUT);  // Устанавливаем пин для движения вперед как выход
  pinMode(motorBackwardPin, OUTPUT); // Устанавливаем пин для движения назад как выход
}

void loop() {
  // Читаем состояние кнопки
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // Если кнопка не нажата, мотор едет вперед
    digitalWrite(motorForwardPin, HIGH);
    digitalWrite(motorBackwardPin, LOW);
  } else {
    // Если кнопка нажата, мотор едет назад
    digitalWrite(motorForwardPin, LOW);
    digitalWrite(motorBackwardPin, HIGH);
  }
}
