#define FILE_LINE_SIZE 180

char received_str[FILE_LINE_SIZE]; 
volatile uint16_t indx; // 0-65535
volatile bool start_communication;

void setup() {
  Serial.begin(115200);
  indx = 0;
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (indx < FILE_LINE_SIZE) { // avoid overflowing the array
      received_str [indx] = c; // save data in the next index in the array buff
      indx++;
      if (c == '\n') {
        Serial.println(String(received_str));
        indx = 0; //reset button to zero
        Serial.println("END\n");
      } 
    }
  }
}
