
/////////////////////
// Pin Definitions //
/////////////////////
const int selectPins[3] = {2, 3, 4}; // S0~2, S1~3, S2~4
const int selectPins_2[3] = {5, 6, 7}; // S0~5, S1~6, S2~7
const int zOutput = 5; 
const int zInput = A0; // Connect common (Z) to A0 (analog input)

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    pinMode(selectPins_2[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
    digitalWrite(selectPins_2[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input
  pinMode(zOutput, OUTPUT); // Set up Z as an input

  // Print the header:
//  Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
//  Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
}

void loop() 
{
  // Loop through 6 pins
  float data[6]= read_mux();
  for (int i = 0; i < 6; i++){
    write_mux(i);  
    for (int j = 0; j < 6; j++){
      Serial.print(String(data[j] + " "));
      Serial.println();
    }
  }
  Serial.print("----------------------------");  
  delay(1000);
}

void write_mux(int pin){
  selectMuxPin(pin); // Select one at a time
  analogWrite(zOutput, 255*5/1023);
}

// Will return 6 entries in 1 row
float read_mux() {
 float data[6];
 for (byte pin=0; pin<=5; pin++)
  {
    selectMuxPin(pin); // Select one at a time
    int inputValue = analogRead(A0)*0.0048339; // and read Z
//    Serial.print(String(inputValue) + " ");
    data[pin] = inputValue;
  }
//  Serial.print("\n");
  return data;
}

// The selectMuxPin function sets the S0, S1, and S2 pins
// accordingly, given a pin from 0-7.
void selectMuxPin(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
