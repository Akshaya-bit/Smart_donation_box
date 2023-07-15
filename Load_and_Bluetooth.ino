#include <HX711_ADC.h>
#include <SoftwareSerial.h>
#include <HX711.h>

#define LOADCELL_DOUT_PIN  7
#define LOADCELL_SCK_PIN  6

HX711 scale;
SoftwareSerial bluetooth(0, 1);
float calibration_factor = -465.82; // Adjust this variable according to the weight sensor
float previousWeight = 0.0;

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from the scale");
  Serial.println("After readings begin, place a known weight on the scale");
  Serial.println("Press + or a to increase the calibration factor");
  Serial.println("Press - or z to decrease the calibration factor");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(); // Reset the scale to 0

  long zero_factor = scale.read_average(); // Get a baseline reading
  Serial.print("Zero factor: "); // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
  scale.set_scale(calibration_factor); // Adjust to this calibration factor
  float weight = scale.get_units(); // Get the weight in grams

  if (fabs(weight - previousWeight) > 2) {
    Serial.print("Reading: ");
    Serial.print(weight, 1);

    // Send the message to the Bluetooth device
    int pointsAdded = static_cast<int>((weight - previousWeight) * 10); // Convert the weight difference to points
    String message = String(pointsAdded) + " points added to your account";
    bluetooth.println(message);

    Serial.print(" g");
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.print(" Message: ");
    Serial.println(message);

    previousWeight = weight;
  }

  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
  delay(1000);
}