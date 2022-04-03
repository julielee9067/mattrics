// #include math.h

////////////////////////
// MACROS and Globals //
////////////////////////
#define NUM_NODES_MAX_SIZE    36 * 5
#define NUM_NODES       36
#define PACKET_SIZE     6

// https://forum.arduino.cc/t/serial-input-basics-updated/382007/2
char start_char = '<';
char end_char = '>';
bool receiving = false;

char values[NUM_NODES];
                                                                                                                                                                
void setup() 
{
  Serial.begin(115200); // Initialize the serial port sending to the PC
  Serial.println("***Start of program ~~ Streaming to MATLAB***"); 

  Serial1.begin(115200); // Initialize the serial port receiving from the MCU
}

void loop() {
  // Wait for serial data to begin streaming in
  if(Serial1.available() > 0)
  {
    char c = Serial1.read();
    while(c != start_char)
    {
        c = Serial1.read();
    }
    for (int count = 0; count < NUM_NODES; ++count)
    {
      int num = Serial1.readBytesUntil(end_char, values, PACKET_SIZE + 1);
      Serial.write(values, num);
    }
  
    // Write CR and LF
    Serial.write(13);
    Serial.write(10);
  }
}
 
bool receivedStartChar()
{
  bool start_of_packet = false;
  char c = Serial1.read();
  while(c != start_char)
  {
      c = Serial1.read();
  }

  start_of_packet = true;

  return start_of_packet;
}
