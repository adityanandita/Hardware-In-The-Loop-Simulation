/* EL3014 Tugas Besar - Hardware in the Loop
 * 13219001 Leonardo Gianto
 * 13219043 Aditya Anandita Dharma Putera
 * 
 * Source Code Arduino Plant
 */

float y, yn1, xn1, xn;

void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,Control,Output");
  Serial.println("RESETTIMER");
}

void loop() {
  // Ambil data dari plant
  if(Serial.available()>0){
    if(Serial.read() == 'Z'){
      char perintah = Serial.read();
      if(perintah == 'S'){
        xn = Serial.parseFloat();
        if(Serial.read() == 'F'){
          // Proses data
          y = 0.00372*xn + 0.00372*xn1 + 0.992*yn1;
          yn1 = y;
          xn1 = xn;
        }
      }
      else if(perintah == 'L'){
        // Kirim feedback ke controller
        if (Serial.read() == 'F'){
          Serial.print("z");
          Serial.print(y);
          Serial.println("f");
          Serial.print("DATA,");
          Serial.print("TIME");
          Serial.print(",");
          Serial.print(xn); Serial.print(","); Serial.println(y);
        }
      }
    }
  }
  delay(100);
}
