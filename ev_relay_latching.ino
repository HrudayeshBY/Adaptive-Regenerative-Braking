int relay1pin = 2;
int relay2pin = 3;
int relay3pin = 4;

int button1Pin = 8;
int button2Pin = 9;
int button3Pin = 10;

int button1Status = 0;
int button2Status = 0;
int button3Status = 0;

void setup() {
  pinMode(relay1pin, OUTPUT);
  pinMode(relay2pin, OUTPUT);
  pinMode(relay3pin, OUTPUT);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);


//setting all relays off
  digitalWrite(relay1pin, HIGH);
  digitalWrite(relay2pin, HIGH);
  digitalWrite(relay3pin, HIGH);

}

void loop() {
  
  button1Status = digitalRead(relay1Pin);
  button2Status = digitalRead(relay2Pin);
  button3Status = digitalRead(relay3Pin);

  digitalWrite(relay1pin, !button1Status);
  digitalWrite(relay2pin, !button2Status);
  digitalWrite(relay3pin, !button3Status);

}
