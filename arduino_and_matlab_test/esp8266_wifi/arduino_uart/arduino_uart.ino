#define FILE_LINE_SIZE 180

char received_bytes[FILE_LINE_SIZE]; 
volatile byte indx; // 0-65535

void setup() {
  Serial.begin(115200);
  indx = 0;
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();

    if (indx < FILE_LINE_SIZE) {
       received_bytes[indx] = c; // save data in the next index in the array buff
       indx++;
    } else {
      Serial.println(String(received_bytes));
      indx = 0; //reset button to zero
      Serial.println("END\n");
    }
  }
}
