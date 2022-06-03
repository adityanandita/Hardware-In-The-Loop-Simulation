/* EL3014 Tugas Besar - Hardware in the Loop
 * 13219001 Leonardo Gianto
 * 13219043 Aditya Anandita Dharma Putera
 * 
 * Source Code Arduino Pengendali
 */

// Konstanta pengendali PID
float Kp = 90;
float Ki = 0;
float Kd = 70;

// Set point, target 
float set_point = 10;

// Error handler
float err_both, err_new, err_new2, err_ratio;
float controlsig = 0;

void setup() {
  Serial.begin(9600);
  /*
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,ControlSig,OutputSig");
  Serial.println("RESETTIMER");
  */
}

void loop() {
  // Ambil data feedback dari plant
  if (Serial.available()){
    if (Serial.find('z')){
      float data = Serial.parseFloat();
      /*
      Serial.print("DATA,");
      Serial.print("TIME,");
      Serial.print(data);
      Serial.print(",");
      Serial.println(controlsig);
      */
      if (Serial.read() == 'f'){
        float err = set_point - data;
        err_both = (err + err_new)/2;
        err_ratio = (err - 2*err_new + err_new2);
        err_new2 = err_new;
        err_new = err;
        controlsig = set_point + Kp*err + Ki*err_both + Kd*err_ratio;
      }
    }
  }
  // Kirim data ke plant
  Serial.print("Z");
  Serial.print("S");
  Serial.print(controlsig);
  Serial.print("F");
  delay(100);

  // Trigger plant untuk mengirimkan data
  Serial.print("Z");
  Serial.print("L");
  Serial.print("F");
  delay(100);
}
