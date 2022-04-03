// #include math.h

////////////////////////
// MACROS and Globals //
////////////////////////
#define NUM_NODES    36

// https://forum.arduino.cc/t/serial-input-basics-updated/382007/2
char start_char = '<';
char end_char = '>';
bool receiving = false;

int values[NUM_NODES] = {1, 0, 1, 0, 1, 0,
                         0, 1, 0, 1, 0, 1,
                         1, 0, 1, 0, 1, 0,
                         0, 1, 0, 1, 0, 1,
                         1, 0, 1, 0, 1, 0,
                         0, 1, 0, 1, 0, 1};

/////////////////////
// Pin Definitions //
/////////////////////

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  Serial.println("***Start of program ~~ Streaming to MEGA***"); 
}

void loop() 
{
  Serial.write(start_char);
  for (int n = 0; n < NUM_NODES; ++n)
  {
    Serial.print(values[n]);
    Serial.write(' ');
  }
  
  // Write CR and LF
  Serial.write(13);
  Serial.write(10);
}
