#define EN_PIN 10    // LOW: Driver enabled, HIGH: Driver disabled
#define STEP_PIN 12  // Step on the rising edge
#define DIR_PIN 11  // Set stepping direction
#define endStopPin 13 // концевик

int Pval = 0; // Прошлое значение
int NVal = 0; // Нынешнее значение
int NtoHorizont = 500;          // Количество микрошагов необходимые для выхода в горизонт
int microSecondsDelay = 2000;  // скорость вращения

void setup() {
 // Configure pin modes
 pinMode(EN_PIN, OUTPUT);  // EN_PIN Включает выключает драйвер
 pinMode(STEP_PIN, OUTPUT); // STEP_PIN работает в микрошагах у нашего tmc микрошаг 1/8
 pinMode(DIR_PIN, OUTPUT); // DIR_PIN отвечает за направление вращения
 pinMode(endStopPin, INPUT); // концевик приведёт мотор в абсолютный 0
 // Initialize pin states
 digitalWrite(EN_PIN, LOW);   // Включили драйвер
 digitalWrite(DIR_PIN, LOW);  // Указали направления

// Идём к концевику
  while(digitalRead(endStopPin) != HIGH) {    // Пока концевик на зажат мотор идет на 4 микрошага
    moveSteps(4);
  }
 delay(100);

 digitalWrite(DIR_PIN, HIGH);   // поменяли направление
 moveSteps(NtoHorizont);        // двигаемся в горизонт
 
}

void loop() {
NVal = map(analogRead(A0),0,1024,0,500); // значение с потенциометра благодаря map  от 0 до 500
if (NVal>Pval)
  digitalWrite(DIR_PIN, HIGH);
  moveSteps(4);
if (NVal<Pval)
  digitalWrite(DIR_PIN, LOW);
  moveSteps(4);
Pval = NVal;
}

// Функция для перемещения. Туда не лезем:)
void moveSteps(int steps) {
 for (int i = 0; i < steps; i++) {
   digitalWrite(STEP_PIN, HIGH);
   delayMicroseconds(microSecondsDelay);
   digitalWrite(STEP_PIN, LOW);
   delayMicroseconds(microSecondsDelay);
 }
}