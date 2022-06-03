/* EL3014 Tugas Besar - Hardware in the Loop
 * 13219001 Leonardo Gianto
 * 13219043 Aditya Anandita Dharma Putera
 * 
 * Source Code Arduino Plant
 */

// Variable untuk fungsi transfer filter digital
float y, yn1, xn1, xn;

void setup() {
	// Setup serial monitor dengan baud rate 9600
	Serial.begin(9600);
	// Setup data logging PLX-DAQ
	Serial.println("CLEARDATA");
	Serial.println("LABEL,Time,Control,Output");
	Serial.println("RESETTIMER");
}

void loop() {
	// Mendeteksi sinyal dari controller
	if(Serial.available()>0){
		// Baca kode awal frame dari controller, Z
		if(Serial.read() == 'Z'){
			// Baca jenis perintah dari controller
			char perintah = Serial.read();
			
			// Jika perintah S maka plant ambil data dari controller
			if(perintah == 'S'){
				xn = Serial.parseFloat();
				if(Serial.read() == 'F'){
					// Proses data
					y = 0.00372*xn + 0.00372*xn1 + 0.992*yn1;
					yn1 = y;
					xn1 = xn;
				}
			}
			// Jika perintah L maka plant kirim feedback ke controller
			else if(perintah == 'L'){
				if (Serial.read() == 'F'){
					// Kirim feedback dengan format frame z<data>f
					Serial.print("z");
					Serial.print(y);
					Serial.println("f");
					// Serial print untuk data logging ke PLX-DAQ
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