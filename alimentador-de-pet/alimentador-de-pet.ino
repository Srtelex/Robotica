#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>

// Definições do display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD (pode ser 0x27 ou 0x3F)

// Definições do RTC
RTC_DS3231 rtc;

// Definições do servo
Servo myServo;
const int servoPin = 9;

// Botões
const int buttonUpPin = 2;   // Botão para aumentar a hora
const int buttonDownPin = 3; // Botão para diminuir a hora

// Variáveis
int hourToFeed = 12; // Hora padrão para alimentar
int minuteToFeed = 0; // Minuto padrão para alimentar
bool feeding = false; // Estado do alimentador

void setup() {
    Serial.begin(9600);
    lcd.begin();
    rtc.begin();
    myServo.attach(servoPin);
    
    pinMode(buttonUpPin, INPUT_PULLUP);
    pinMode(buttonDownPin, INPUT_PULLUP);

    // Verifica se o RTC está funcionando
    if (!rtc.isrunning()) {
        Serial.println("RTC não está funcionando!");
        // Inicializa o RTC com a data e hora atual
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    lcd.print("Alimentador de Pets");
    delay(2000);
}

void loop() {
    // Atualiza a hora atual
    DateTime now = rtc.now();

    // Exibe a hora e os minutos no LCD
    lcd.setCursor(0, 1);
    lcd.print("Hora: ");
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());

    // Verifica se é hora de alimentar
    if (now.hour() == hourToFeed && now.minute() == minuteToFeed && !feeding) {
        myServo.write(90); // Abre o compartimento
        delay(5000);       // Mantém aberto por 5 segundos
        myServo.write(0);  // Fecha o compartimento
        feeding = true;    // Marca como alimentado
    }

    // Verifica os botões para ajustar a hora
    if (digitalRead(buttonUpPin) == LOW) {
        hourToFeed = (hourToFeed + 1) % 24; // Aumenta a hora
        delay(200); // Debounce
    }
    if (digitalRead(buttonDownPin) == LOW) {
        hourToFeed = (hourToFeed - 1 + 24) % 24; // Diminui a hora
        delay(200); // Debounce
    }

    // Reset de alimentação
    if (now.minute() != minuteToFeed) {
        feeding = false; // Reseta o estado de alimentação
    }

    delay(1000); // Atualiza a cada segundo
}
