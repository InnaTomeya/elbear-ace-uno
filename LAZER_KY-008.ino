#define LAZER 6 // задаем имя для Pin

void setup() {
   pinMode(LAZER, OUTPUT); // инициализируем Pin выход
}

void loop() {
for (int i=0; i<=5; i++) // мигание лазерным светодиодом
   {
      digitalWrite(LAZER, HIGH);
      delay(500);
      digitalWrite(LAZER, LOW);
      delay(500);
   }
   delay(1000);
}