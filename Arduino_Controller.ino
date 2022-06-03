/* EL3014 Tugas Besar - Hardware in the Loop
 * 13219001 Leonardo Gianto
 * 13219043 Aditya Anandita Dharma Putera
 * 
 * Source Code Arduino Pengendali
 */

// Deklarasi untuk timer interrupt
#define TCNT_HIGH 0xF9
#define TCNT_LOW 0xE6

// Konstanta pengendali PID
float Kp = 90;
float Ki = 20;
float Kd = 70;

// Set point, target
float set_point = 10;

// Error handler
float err_both, err_new, err_new2, err_ratio;

// Sinyal control ke plant
float controlsig = 0;

// Interrupt timer
void init_int(void){
	// set prescaler 1024
	TIMSK1 = 0b000001;
	TCCR1B = 0b00001101;
	TCNT1H = TCNT_HIGH;
	TCNT1L = TCNT_LOW;
	// Enable interrupt
	sei();
}

ISR (TIMER1_OVF_vect){
	// Kirim data ke plant dengan perintah S
	Serial.print("Z"); 			// Z adalah kode awal menandakan awal frame
	Serial.print("S");			// S adalah perintah untuk plant menerima data
	Serial.print(controlsig);	// Data control yang dikirim
	Serial.print("F");			// F adalah kode akhir frame

	// Trigger plant untuk mengirimkan data dengan perintah L
	Serial.print("Z");
	Serial.print("L");
	Serial.print("F");
	
	// Reset timer
	TCNT1H = TCNT_HIGH;
	TCNT1L = TCNT_LOW;
}


void setup() {
	Serial.begin(9600);
	init_int();
}

void loop() {
	// Ambil data feedback dari plant
	if (Serial.available()){
		// Deteksi kode awal frame plant, yaitu z
		if (Serial.find('z')){
			// Ambil data feedback dari plant
			float data = Serial.parseFloat();
			// Jika frame kode plant selesai, maka proses data
			if (Serial.read() == 'f'){
				// Hitung error
				float err = set_point - data;
				err_both = (err + err_new)/2;
				err_ratio = (err - 2*err_new + err_new2);
				err_new2 = err_new;
				err_new = err;
				// Berdasarkan nilai error, hitung sinyal kendali baru
				controlsig = set_point + Kp*err + Ki*err_both + Kd*err_ratio;
			}
		}
	}
}