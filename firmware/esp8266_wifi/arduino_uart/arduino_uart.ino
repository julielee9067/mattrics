#define FILE_LINE_SIZE 9129

char received_str[FILE_LINE_SIZE]; 
volatile uint16_t indx; // 0-65535
volatile bool start_communication;

void setup() {
  Serial.begin(115200);
  memset(received_str, 0, sizeof received_str);
  indx = 0;
//  start_communication = false;
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (indx < FILE_LINE_SIZE) { // avoid overflowing the array
      received_str [indx] = c; // save data in the next index in the array buff
      indx++;
//      if (c == 'S' || start_communication) {
//        received_str [indx] = c; // save data in the next index in the array buff
//        indx++;
//      }
//        
//      if (c == 'S') { // check for start of the word
//        Serial.println("\nSTART");
//        start_communication = true;
//      } else if (c == 'E') { //check for the end of the word
      if (c == '\n') {
        Serial.println(String(received_str));
        indx = 0; //reset button to zero
        memset(&received_str[0], 0, sizeof(received_str));
//        start_communication = false;
        Serial.println("END\n");
      } 
    }
  }
}
