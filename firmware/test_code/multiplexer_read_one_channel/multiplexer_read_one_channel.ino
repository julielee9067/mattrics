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

// Will return 6 entries in 1 row
void read_mux(float data[6]) {
 for (byte pin=0; pin<=5; pin++)
  {
    selectMuxPinRead(pin); // Select one at a time
    float inputValue = analogRead(A0)*0.0048339*100; // and read Z
//    Serial.print(String(inputValue) + " ");
    data[pin] = inputValue;
  }
//  Serial.print("\n");
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

  // Print the header:
//  Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
  Serial.println("Start of program****");
}

void loop() 
{
  // Loop over 6 rows
//  for (int i = 0; i < 6; i++){
    // Turn on row 0
    write_mux(5);
    // Loop over 6 columns
//    float data[6] = {0, 0, 0, 0, 0, 0};
//    read_mux(data);
     for (byte pin=0; pin<=5; pin++)
      {
        selectMuxPinRead(pin); // Select one at a time
        float inputValue = analogRead(A0)*0.0048339*100; // and read Z
        Serial.print(String(inputValue) + " ");
  //      data[pin] = inputValue;
      }
//    for (int j = 0; j < 6; j++){
//    }
    Serial.println();
    delay(1000);
  
//  Serial.print("--------------------------------");  
//  Serial.println();
}
