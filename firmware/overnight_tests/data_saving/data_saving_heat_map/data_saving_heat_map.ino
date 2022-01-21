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
const int calibration_time = 10000;
const float gravity = 9.81;
const float area = 3.14159265359 * (0.02597 / 2) * (0.02597 / 2);
const double VCC = 5.091;
unsigned long timer_calibration = 0;
unsigned long timer_sample = 0;
const int mat_dimension = 6; // number of nodes per row/ column
const int weighted_ave = 0.8; // give 80% reliance on existing data

int calibration_array [mat_dimension][mat_dimension];


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


  // Set start time for calibration
  timer_calibration = millis();  
  // Populate calibration array from calibration
  calibration();

  // Get the start time in millis
  // Note that the number returned by millis() overflows after about 50 days
  // So this program is not designed to be run longer than that
  // Start timer for sampling
  timer_sample = millis();
}

void calibration() {
  bool first_iteration = true;
  while (millis() - timer_calibration < calibration_time) { // calibration still ongoing
    int raw_data [mat_dimension][mat_dimension];
    // Loop over 6 rows
    int sum = 0;
    for (int row = 0; row < 6; row++) {
      // Turn on row row
      write_mux(row);
      // Loop over 6 columns
      for (byte col = 0; col < 6; col++) {
        selectMuxPinRead(col); // Select one at a time
        int input_value = analogRead(zInput); // * 0.0048339; // And read Z
        //float data = convert_to_pressure(calc_r1(inputValue));
        raw_data[row][col] = input_value;
        sum = sum + input_value;
      }
    }
    int average_of_all_points = sum / (mat_dimension*mat_dimension);
    // Take difference of each point and put into 2d array
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        if (first_iteration) {
          calibration_array[i][j] = average_of_all_points - raw_data[i][j]; // calibration difference is average - value at each node         
        } else {
          int diff_calc = average_of_all_points - raw_data[i][j];
          calibration_array[i][j] = (calibration_array[i][j]* weighted_ave) + (diff_calc * (1-weighted_ave)); // take the weighted average
        }
      }
    }
    first_iteration = false;

    // For debugging purposes
//    for (int k = 0; k < 6; k++) {
//      for (int l = 0; l < 6; l++) {
//        Serial.print(String(calibration_array[k][l]) + ", ");
//      }
//    }
//
//    Serial.print("\n\n");
  }
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
      int inputValue = analogRead(zInput) + calibration_array[row][col]; // calibration
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
