
/////////////////////
// Pin Definitions //
/////////////////////
const int selectPinsCols[3] = {2, 3, 4}; // S0~2, S1~3, S2~4
const int selectPinsRows[3] = {8, 9, 10}; // S0~8, S1~9, S2~10
const int zInput = A0; // Connect col common (Z) to A0 (analog input)
const int zOutput = 5; 

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
  {
    pinMode(selectPinsCols[i], OUTPUT);
    pinMode(selectPinsRows[i], OUTPUT);
    digitalWrite(selectPinsCols[i], HIGH);
    digitalWrite(selectPinsRows[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input
  pinMode(zOutput, OUTPUT); // Set up Z as an input

  // Print the header:
//  Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
//  Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
}

void loop() 
{
  // Loop over 6 rows
  for (int i = 0; i < 6; i++){
    write_mux(i);  
    // Loop over 6 columns
    float data[6];
    read_mux(data);
    for (int j = 0; j < 6; j++){
      Serial.print(String(data[j]) + " ");
      Serial.println();
    }
  }
  Serial.print("----------------------------");  
  delay(1000);
}

void write_mux(int pin){
  selectMuxPinWrite(pin); // Select one at a time
  analogWrite(zOutput, 255);
}

// Will return 6 entries in 1 row
void read_mux(float data[6]) {
 for (byte pin=0; pin<=5; pin++)
  {
    selectMuxPinRead(pin); // Select one at a time
    int inputValue = analogRead(A0)*0.0048339; // and read Z
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
      digitalWrite(selectPinsCols[i], HIGH);
    else
      digitalWrite(selectPinsCols[i], LOW);
  }
}

void selectMuxPinWrite(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPinsRows[i], HIGH);
    else
      digitalWrite(selectPinsRows[i], LOW);
  }
}
