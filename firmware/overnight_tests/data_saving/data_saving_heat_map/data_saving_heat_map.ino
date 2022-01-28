// #include math.h

/////////////////////
// Pin Definitions //
/////////////////////
const int selectPinsWrite[3] = {8, 9, 10}; 
const int selectPinsRead[3] = {2, 3, 4}; 
const int zOutput = 5; // Connect to the write mux
const int zInput = A0; // Connect to the read mux
const int r2 = 470;
const int vs = 5;
const int sample_time = 30000;
const float gravity = 9.81;
const float area = 3.14159265359 * (0.02597 / 2) * (0.02597 / 2);
const double VCC = 5.091;
unsigned long timer_sample = 0;


float calc_r1(float vo)
{
  return ((r2 * (vs - vo) / vo));
}

float convert_to_pressure(float resistance)
{
  return ((937 * exp(-0.006438 * resistance)) / area * gravity / 1000);
}

void write_mux(int pin){
  selectMuxPinWrite(pin); // Select one at a time
  analogWrite(zOutput, 255);
}

// The selectMuxPin function sets the S0, S1, and S2 pins
// accordingly, given a pin from 0-7.
void selectMuxPinRead(byte pin)
{
  for (int i = 0; i < 3; i++)
  {
    if (pin & (1 << i))
      digitalWrite(selectPinsRead[i], HIGH);
    else
      digitalWrite(selectPinsRead[i], LOW);
  }
}

void selectMuxPinWrite(byte pin)
{
  for (int i = 0; i < 3; i++)
  {
    if (pin & (1 << i))
      digitalWrite(selectPinsWrite[i], HIGH);
    else
      digitalWrite(selectPinsWrite[i], LOW);
  }
}

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i = 0; i < 3; i++)
  {
    pinMode(selectPinsWrite[i], OUTPUT);
    digitalWrite(selectPinsWrite[i], HIGH);
    pinMode(selectPinsRead[i], OUTPUT);
    digitalWrite(selectPinsRead[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input
  pinMode(zOutput, OUTPUT); // Set up Z as an input

  Serial.println("Start of program. Calibration will occur for 10 seconds ****");

  // Get the start time in millis
  // Note that the number returned by millis() overflows after about 50 days
  // So this program is not designed to be run longer than that
  // Start timer for sampling
  timer_sample = millis();
}

void loop() 
{
  unsigned long new_milli = millis();
  timer_sample = new_milli;
  // Loop over 6 rows
  for (int row = 0; row < 6; row++){
    // Turn on row
    write_mux(row);
    // Loop over 6 columns
    for (byte col = 0; col <= 5; col++)
    {
      selectMuxPinRead(col); // Select one at a time
      int inputValue = analogRead(zInput); 
      Serial.print(String(inputValue) + " ");
      if (col != 5) {
        Serial.print(String(0) + " ");
      }
    }
    if (row != 5)
    {
      for (int j = 0; j < 11; j++){
      Serial.print(String(0) + " ");
      }
    }
  }
  Serial.write(13);
  Serial.write(10);
}
