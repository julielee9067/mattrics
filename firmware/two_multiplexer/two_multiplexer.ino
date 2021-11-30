// 1 multiplexer only write to 1 channel
// Other multiplexer reads from all channels in order


/////////////////////
// Pin Definitions //
/////////////////////
const int selectPinsWrite[3] = {8, 9, 10}; 
const int selectPinsRead[3] = {2, 3, 4}; 
const int zOutput = 5; 
const int zInput = A0; // Connect common (Z) to A0 (analog input)


void write_mux(int pin){
  selectMuxPinWrite(pin); // Select one at a time
  analogWrite(zOutput, 255);
}

// The selectMuxPin function sets the S0, S1, and S2 pins
// accordingly, given a pin from 0-7.
void selectMuxPinRead(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPinsRead[i], HIGH);
    else
      digitalWrite(selectPinsRead[i], LOW);
  }
}

void selectMuxPinWrite(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPinsWrite[i], HIGH);
    else
      digitalWrite(selectPinsWrite[i], LOW);
  }
}

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
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
  // Loop over 6 rows
  for (int i = 0; i < 6; i++){
    // Turn on row i
    write_mux(i);
    // Loop over 6 columns
    for (byte pin=0; pin<=5; pin++){
      selectMuxPinRead(pin); // Select one at a time
      float inputValue = analogRead(A0)*0.0048339; // and read Z
      Serial.print(String(inputValue) + " ");
    }
      Serial.println();
    }
  delay(1000);
  Serial.print("---------------------------------");  
  Serial.println();
}
