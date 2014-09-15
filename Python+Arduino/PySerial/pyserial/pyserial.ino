#define READ 0
#define WRITE 1

int read_pin = 2;
int led_pin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(read_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  while (!Serial);
}

void loop() {

  int mode = digitalRead(read_pin);
  if (mode == READ) {
    if (Serial.available()) {
      int c = Serial.read();
      blinky(c);
    }
  }
  else if (mode == WRITE) {
    Serial.println("I'm here and listening");
    blinky(1);
    delay(500);
  }
}

void blinky(int n) {
  for (int i = 0; i < n; ++i) {
    digitalWrite(led_pin, HIGH);
    delay(200);
    digitalWrite(led_pin, LOW);
    delay(200);
  }
}
