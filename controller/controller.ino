
// A simple program to turn ON/OFF pins on the arduino over 
// the serial port via the command string like:
//
//   zone,minutes,debug,
//
// zone is 0 to 8 # zero turns all pins off
// minutes is number of minutes to keep pin HIGH
// if debug is 1 it will print back status messages on the serial port
// 
// If training arguments are missing, they will receive zero.
//
// Hint: use a non-numeric character at the end to get a faster response.


int relay_channels[8]={39,41,43,45,47,49,51,53};  // These pins can be controlled via zone 1 to 8

int zone = 0;   // zero is for all zones off
unsigned long seconds = 0;
unsigned long minutes = 0;
int pin = 0;
int debug = 0;

unsigned long time1;
unsigned long time;
unsigned long min;

void turn_on(int zone, unsigned long minutes) {
  int i;
  int pin;
  if (zone == 0) {
    if (debug) {
      Serial.println("Turning off all zones.\n");
    }
  }
  // First turn all others off to prevent two from being on at the same time.
  // If pin is already on, don't turn it off to turn it immediately back on again.
  for (i=1; i <= 8; i=i+1) {
    if (zone == i) {
      pin = relay_channels[i-1];
      if (debug) {
        Serial.print("Turning off all zones but zone ");
        Serial.print(zone);
        Serial.print(".\n");
      }
    } else {
      digitalWrite(relay_channels[i-1], LOW);
    }
    delay(50);
  }
  // Then turn on zone:
  for (i=1; i <= 8; i=i+1) {
    if (zone == i) {
      pin = relay_channels[i-1];
      if (debug) {
        Serial.print("Turn on zone ");
        Serial.print(zone);
        Serial.print(" for ");
        Serial.print(minutes);
        Serial.print(" minutes (Arduino pin ");
        Serial.print(pin);
        Serial.print(").\n");
      }
      digitalWrite(pin, HIGH);
    }
    delay(80);
  }
}

void turn_off() {
  int i;
  int pin;
  if (debug) {
    Serial.println("Turning off all zones.\n");
  }
  for (i=1; i <= 8; i=i+1) {
      digitalWrite(relay_channels[i-1], LOW);
  }
  delay(80);
}

void setup() {
  int i;
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  delay(100);
  for (i=0; i < 8; i=i+1) {
    pinMode(relay_channels[i], OUTPUT);
  }
  turn_off(); // turn all off;
  pinMode(13, OUTPUT); // LED pin
  time1 = millis();
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    zone = Serial.parseInt();
    minutes = Serial.parseInt();
    debug = Serial.parseInt();
    
    time1 = millis();
        
    turn_on(zone, minutes);
    if (zone > 8) {
        if (debug) {
          Serial.print("ERROR: doing nothing, no such zone: ");
          Serial.print(zone);
          Serial.println();
        }
        return;
    }
  }
  
  time = millis() - time1;
  seconds = time/1000;
  min = seconds/60;
  if (zone != 0) {
    if (min >= minutes) {
      zone = 0;
      turn_off();  // Turn all off by passing bogus zone
    }
  }
  // Blink LED
  if (seconds % 2) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  delay(300);
}


