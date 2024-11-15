//Team: Low Level Brawlers
//Track: Electric Vehicle and Future Mobility
//Team: Arun V M, Hemanth R, Hrudayesh B Y, Lokesh M


//Variables for Relay
int relay1Pin = 8;
int relay2Pin = 9;
int motorRelayPin = 10;

//Variables for button
int throttlePin = 3;

//Variables for States
int previousState = 0;
int presentState = 0;


//Variables for regen mode
int mildModeStatus = 0;
int aggressiveModeStatus = 0;
//Variables for regen modes
int aggressiveModePin = 5;
int mildModePin = 6;

//Variable for brakeLight
int brakeLightStatus = 0;
int brakeLightPin = 2;

void setup() 
{

  Serial.begin(9600);//Enabling the serial communication
  //Setting pins connected to relays as output
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(motorRelayPin, OUTPUT);

  //Setting pins connected to button as input
  pinMode(throttlePin, INPUT);

  //Turning all relays OFF
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(motorRelayPin, HIGH);//Relays are Active Low

  //Setting pin of Brake light
  pinMode(brakeLightPin, OUTPUT);

  //Turning the brake LightRelay OFF
  digitalWrite(brakeLightPin, HIGH);
}

void loop()
{
  presentState = digitalRead(throttlePin);
  Serial.println(presentState);

  if(presentState == 1)
  {
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH); //Disabling Regen while accelrating
    digitalWrite(motorRelayPin, !presentState);//relay is active LOW
    digitalWrite(brakeLightPin,  HIGH);
    Serial.println("Vehicle Accelerating...");
    previousState = presentState;
  }
  

  presentState = digitalRead(throttlePin);
  if(previousState ==  1 && presentState == 0)
  {
    Serial.println(presentState);
    digitalWrite(motorRelayPin, !presentState);
    Serial.println("Throttle Released...");
    Serial.println("Activating Regen");
    //previousState = 0;

    mildModeStatus = digitalRead(mildModePin);
    aggressiveModeStatus = digitalRead(aggressiveModePin);

    digitalWrite(relay1Pin, !mildModeStatus);
    digitalWrite(relay2Pin, !aggressiveModeStatus);

  
    brakeLightStatus = aggressiveModeStatus? LOW:HIGH;
    digitalWrite(brakeLightPin,  brakeLightStatus);
    // delay(2000);
    // digitalWrite(brakeLightPin,  HIGH);
    // delay(2000);


  }

  delay(50);


}
