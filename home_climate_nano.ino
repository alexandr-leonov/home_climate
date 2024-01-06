#include <microDS18B20.h>
#include <TFT.h>
#include <MQ2.h>


#define TFT_CS   11
#define TFT_DC   7
#define TFT_RST  8
#define PIN_MQ2  4

MicroDS18B20<2> sensor;
MQ2 mq2(PIN_MQ2);
TFT screen = TFT(TFT_CS, TFT_DC, TFT_RST);
char temperaturePrintout[9];
char co2Printout[9];
String sensorValue;
int co2Value;



void setup() {
  // Ожидание запуска датчиков
  Serial.begin(9600);
  // Настройки дотчика CO2
  mq2.begin();

  // Настройки TFT дисплея
  screen.begin();
  screen.background(255, 0, 0);
  screen.stroke(255, 255, 255);
  screen.setTextSize(2);
  screen.text("\n HOME CLIMAT :\n ", 0, 0);
  screen.setTextSize(3);
}


void loop() {

  // запрос температуры
  sensor.requestTemp();

  //запрос CO2
  co2Value = analogRead(PIN_MQ2);

  delay(250);

  // проверяем успешность чтения и выводим
  if (sensor.readTemp()) {

    renderScreen();

    delay(3000);

    clearScreen();
+
  }
  else Serial.println("error");
}

/**
   Отображение на части экрана изменяемых показателей:
   - температура
   - значение CO2
*/
void renderScreen() {
  sensorValue = String(sensor.getTemp()) + "'C";
  sensorValue.toCharArray(temperaturePrintout, 9);

  screen.stroke(255, 255, 255);
  screen.text(temperaturePrintout, 0, 50);

  sensorValue = String(co2Value) + " CO2";
  sensorValue.toCharArray(co2Printout, 9);

  // если значение превышает допустимую норму
  // меняем цвет фона на красный
  if (co2Value > 1000) {
    screen.stroke(0, 0, 255);
  }
  screen.text(co2Printout, 0, 80);
}

/**
   Очистка части экрана (меняем цвет текста на фоновый)
*/
void clearScreen() {
  screen.stroke(255, 0, 0);
  screen.text(temperaturePrintout, 0, 50);
  screen.text(co2Printout, 0, 80);
}
