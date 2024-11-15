#include <Wire.h> //I2C library
#include <MPU6050.h> //sensor library

MPU6050 mpu; //creating object

// Thresholds
const float coastingThreshold = 0.2;           // Threshold for low acceleration (G)
const float corneringThreshold = 40.0;         // Threshold for angular velocity (degrees/second)
const float corneringExitThreshold = 25.0;     // Lower threshold to exit cornering
const float downhillTiltThreshold = 20.0;      // Tilt angle threshold for downhill detection

// Number of stable readings needed to exit cornering
const int corneringStableReadingsNeeded = 3;

// Smoothing factor
const float alpha = 0.8;

// Variables to hold smoothed values
float smoothAccelX = 0.0;
float smoothGyroZ = 0.0;

// Flags and counters
bool isCoasting = true;
bool isCornering = false;
bool isDownhill = false;

int corneringStableReadingsCount = 0;

// Relay pins
const int coasting_corner_RelayPin = 8;
//const int corneringRelayPin = 8;
const int downhillRelayPin = 9;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize(); //intializing MPU6050

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  } else {
    Serial.println("MPU6050 connected.");
  }

  // Initialize relay pins as outputs
  pinMode(coasting_corner_RelayPin, OUTPUT);
  //pinMode(corneringRelayPin, OUTPUT);
  pinMode(downhillRelayPin, OUTPUT);
  
  // Default to coasting mode
  setRelayState(true, false, false);
}

void loop() {
  // Read accelerometer and gyroscope values
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //read the accelerometer and gyroscope values

  // Convert accelerometer readings to 'G' units
  float accelX = ax / 16384.0;
  float accelY = ay / 16384.0;
  float accelZ = az / 16384.0;

  // Convert gyroscope readings to degrees per second
  float gyroZ = gz / 131.0;

  // Apply smoothing (EMA equation) 
  smoothAccelX = (alpha * smoothAccelX) + ((1 - alpha) * accelX);
  smoothGyroZ = (alpha * smoothGyroZ) + ((1 - alpha) * gyroZ);

  // Coasting detection
  if (!isCoasting && abs(smoothAccelX) < coastingThreshold) { //if not coasting and x is less than coasting threshold
    setRelayState(true, false, false); // Default to coasting
  } else if (isCoasting && abs(smoothAccelX) >= coastingThreshold + 0.1) {
    isCoasting = false;
  }

  // Cornering detection
  if (!isCornering && abs(smoothGyroZ) > corneringThreshold) {
    setRelayState(false, true, false); // Activate cornering relay
    corneringStableReadingsCount = 0;
  } else if (isCornering && abs(smoothGyroZ) < corneringExitThreshold) {
    corneringStableReadingsCount++;
    if ((corneringStableReadingsCount >= corneringStableReadingsNeeded)&&(isCoasting!=0)) {
      setRelayState(true, false, false); // Return to coasting
    }
  } else if (isCornering && abs(smoothGyroZ) >= corneringExitThreshold) {//prevents false exiting
    corneringStableReadingsCount = 0;
  }

  // Downhill detection
  float tiltAngle = atan2(accelY, accelZ) * 180 / PI;//inverse tangent and conversion from radian to angle
  if (!isDownhill && tiltAngle > downhillTiltThreshold) {
    setRelayState(false, false, true); // Activate downhill relay
  } else if (isDownhill && tiltAngle <= downhillTiltThreshold - 2) {
    setRelayState(true, false, false); // Return to coasting
  }

  // Print the state of flags
  Serial.print("Coasting: "); Serial.print(isCoasting);
  Serial.print(", Cornering: "); Serial.print(isCornering);
  Serial.print(", Downhill: "); Serial.println(isDownhill);

  delay(100);
}

// Function to set relay states based on mode
void setRelayState(bool coasting, bool cornering, bool downhill) {
  isCoasting = coasting;
  isCornering = cornering;
  isDownhill = downhill;

  digitalWrite(coasting_corner_RelayPin, coasting || cornering ? HIGH : LOW);
  //digitalWrite(corneringRelayPin, cornering ? HIGH : LOW);
  digitalWrite(downhillRelayPin, downhill ? HIGH : LOW);
}