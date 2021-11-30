
// Code to test that 1 multiplexer can turn on 1 channel one by one
// Nothing being read from arduino, use multimeter

/////////////////////
// Pin Definitions //
/////////////////////
const int selectPins[3] = {8, 9, 10}; // S0~2, S1~3, S2~4
const int zOutput = 5; 
//const int zInput = A0; // Connect common (Z) to A0 (analog input)

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
//  pinMode(zInput, INPUT); // Set up Z as an input
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
    Serial.print(String(i));
    // Loop over 6 columns
//    float data[6] = {0, 0, 0, 0, 0, 0};
//    read_mux(data);
//    for (int j = 0; j < 6; j++){
//      Serial.print(String(data[j]) + " ");
//    }
    Serial.println();
    Serial.println();

    delay(10000);
  }
//  Serial.print("--------------------------------");  
//  Serial.println();
}

void write_mux(int pin){
  selectMuxPinWrite(pin); // Select one at a time
  analogWrite(zOutput, 255);
}

void select_y_four() {

  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[2], LOW);
  digitalWrite(selectPins[1], LOW);
}


// The selectMuxPin function sets the S0, S1, and S2 pins
// accordingly, given a pin from 0-7.
void selectMuxPinWrite(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
