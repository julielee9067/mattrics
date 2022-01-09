// 1 multiplexer only write to 1 channel
// Other multiplexer reads from all channels in order
// #include math.h

/////////////////////
// Pin Definitions //
/////////////////////
const int selectPinsWrite[3] = {8, 9, 10}; 
const int selectPinsRead[3] = {2, 3, 4}; 
const int zOutput = 5; 
const int zInput = A0; // Connect common (Z) to A0 (analog input)
const int r2 = 470;
const int vs = 5;
const float gravity = 9.81;
const float area = 3.14159265359 * (0.02597 / 2) * (0.02597 / 2);
unsigned long curr_milli = 0;

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
  // Get the start time in millis
  // Note that the number returned by millis() overflows after about 50 days
  // So this program is not designed to be run longer than that
  curr_milli = millis();
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

  Serial.println("Start of program****");
}

void loop() 
{
  unsigned long new_milli = millis();
  if (new_milli - curr_milli >= 30000)
  {
    curr_milli = new_milli;
    // Loop over 6 rows
    for (int i = 0; i < 6; i++){
      // Turn on row i
      write_mux(i);
      // Loop over 6 columns
      for (byte pin = 0; pin <= 5; pin++)
      {
        selectMuxPinRead(pin); // Select one at a time
        float inputValue = analogRead(A0) * 0.0048339; // And read Z
        float data = convert_to_pressure(calc_r1(inputValue));
        Serial.print(String(data));
        if (i != 5 || pin != 5)
        { 
          Serial.print(",");   
        }
      }
    }
      Serial.println();
      Serial.write(13);
      Serial.write(10);
  }
}
