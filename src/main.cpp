#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

const int chipSelectPin = 10;                 // Chip select pin for the SD card module
RTC_DS3231 rtc;                               // Real-time clock object

// Pin definitions
int channel1 = 2;                             // Input pin for button 1
int channel2 = 3;                             // Input pin for button 2
int channel3 = 4;                             // Input pin for button 3
int buzzer   = 9;                             // Output pin for the buzzer
int bunce    = 100;                           // Debounce time in milliseconds for CH 1-3

String generate_timestamp() {
  DateTime now = rtc.now();                   // Get the current timestamp from DS3231
  
  String timestamp = String(now.year(), DEC) + "-" +  // Format the timestamp as a string with leading zeros
                     String(now.month(), DEC) + "-" + 
                     String(now.day(), DEC) + " " +
                     String(now.hour(), DEC) + ":" + 
                     String(now.minute(), DEC) + ":" + 
                     String(now.second(), DEC);
  
  return timestamp;                           // Return the formatted timestamp
}

void error_chime() {
  for (int i = 0; i < 5; i++) {               // Loop to play the error chime 5 times
    tone(buzzer, 2500, 300); delay(300);      // Play a 2500 Hz tone for 300 ms
    tone(buzzer, 2000, 300); delay(300);      // Play a 2000 Hz tone for 300 ms
  }
}

void setup() {
  SPI.begin();                                // Initialize SPI communication
  Serial.begin(9600);                         // Initialize serial communication at 9600 baud

  pinMode(channel1, INPUT_PULLUP);            // Set channel1 pin as input with internal pull-up resistor
  pinMode(channel2, INPUT_PULLUP);            // Set channel2 pin as input with internal pull-up resistor
  pinMode(channel3, INPUT_PULLUP);            // Set channel3 pin as input with internal pull-up resistor
  pinMode(buzzer, OUTPUT);                    // Set buzzer pin as output
  
  if (!SD.begin(chipSelectPin)) {             // Initialize SD card
    Serial.println("SD card initialization failed!"); // Print error message if SD card initialization fails
    return;                                   // Exit setup function
  }

  Wire.begin();                               // Initialize I2C communication
  
  if (!rtc.begin()) {                         // Check if the DS3231 is connected
    Serial.println("DS3231 initialization failed!"); // Print error message if DS3231 initialization fails
    error_chime();                            // Play error chime
    return;                                   // Exit setup function
  }

  File file = SD.open("/test.txt", FILE_WRITE); // Open a file for writing
  
  if (file) {                                 // Check if the file opened successfully
    file.close();                             // Close the file
    Serial.println("File initialized successfully!"); // Print success message
  } else {
    Serial.println("Error initializing file!"); // Print error message if file initialization fails
    error_chime();                            // Play error chime
  }

  // Play startup chime
  tone(buzzer, 2200, 200); delay(200);        // Play a 2200 Hz tone for 200 ms
  tone(buzzer, 2500, 200); delay(200);        // Play a 2500 Hz tone for 200 ms
  tone(buzzer, 2800, 200); delay(200);        // Play a 2800 Hz tone for 200 ms
}

void loop() {
  static int previousState1 = HIGH;           // Previous state of button 1
  int currentState1 = digitalRead(channel1);  // Current state of button 1
  static unsigned long previousTime1 = 0;     // Previous time button 1 was pressed
  unsigned long currentTime1 = millis();      // Current time

  if (currentState1 == LOW && previousState1 == HIGH && currentTime1 - previousTime1 >= bunce) {
    previousTime1 = currentTime1;             // Update previous time
    File file1 = SD.open("/test.txt", FILE_WRITE); // Open the file for writing

    if (file1) {                              // Check if the file opened successfully
      String timestamp1 = generate_timestamp(); // Get the current timestamp
      file1.println(timestamp1 + ",1");       // Write the timestamp and the value to the file
      Serial.println(timestamp1 + ",1");      // Print the timestamp and the value to the serial monitor
      tone(buzzer, 2000, 100);                // Play a 2000 Hz tone for 100 ms

      file1.close();                          // Close the file
    } else {
      Serial.println("Error opening file!");  // Print error message if file opening fails
      error_chime();                          // Play error chime
    }
  }
  previousState1 = currentState1;             // Update previous state

  static int previousState2 = HIGH;           // Previous state of button 2
  int currentState2 = digitalRead(channel2);  // Current state of button 2
  static unsigned long previousTime2 = 0;     // Previous time button 2 was pressed
  unsigned long currentTime2 = millis();      // Current time

  if (currentState2 == LOW && previousState2 == HIGH && currentTime2 - previousTime2 >= bunce) {
    previousTime2 = currentTime2;             // Update previous time
    File file2 = SD.open("/test.txt", FILE_WRITE); // Open the file for writing

    if (file2) {                              // Check if the file opened successfully
      String timestamp2 = generate_timestamp(); // Get the current timestamp
      file2.println(timestamp2 + ",2");       // Write the timestamp and the value to the file
      Serial.println(timestamp2 + ",2");      // Print the timestamp and the value to the serial monitor
      tone(buzzer, 2200, 100);                // Play a 2200 Hz tone for 100 ms

      file2.close();                          // Close the file
    } else {
      Serial.println("Error opening file!");  // Print error message if file opening fails
      error_chime();                          // Play error chime
    }
  }
  previousState2 = currentState2;             // Update previous state

  static int previousState3 = HIGH;           // Previous state of button 3
  int currentState3 = digitalRead(channel3);  // Current state of button 3
  static unsigned long previousTime3 = 0;     // Previous time button 3 was pressed
  unsigned long currentTime3 = millis();      // Current time

  if (currentState3 == LOW && previousState3 == HIGH && currentTime3 - previousTime3 >= bunce) {
    previousTime3 = currentTime3;             // Update previous time
    File file3 = SD.open("/test.txt", FILE_WRITE); // Open the file for writing

    if (file3) {                              // Check if the file opened successfully
      String timestamp3 = generate_timestamp(); // Get the current timestamp
      file3.println(timestamp3 + ",3");       // Write the timestamp and the value to the file
      Serial.println(timestamp3 + ",3");      // Print the timestamp and the value to the serial monitor
      tone(buzzer, 2500, 100);                // Play a 2500 Hz tone for 100 ms

      file3.close();                          // Close the file
    } else {
      Serial.println("Error opening file!");  // Print error message if file opening fails
      error_chime();                          // Play error chime
    }
  }
  previousState3 = currentState3;             // Update previous state
}