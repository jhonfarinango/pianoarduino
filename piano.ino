
//Código Piano:

#define SPK 13  //Conexión a la bocina 
#define PLAYBACK_BTN 12  // Grabar o reproducir 
#define BUFFERSIZE 160   // Memoria de almacenamiento temporal   
int keys[] = {2,3,4,5,6,7,8,9};  //pines de entrada para los pulsadores  
int tones[] = {2093,1976,1760,1568,1397,1319,1175,1047}; // Tonos 
//int tones[] = {1047,1175,1319,1397,1568,1760,1976,2093};  //Tonos secundarios 
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
//int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 }; //Tonos secundarios

int length = sizeof keys/sizeof *keys; // Ingresa los valores a los números de pulsadores (keys)

// Para grabar
int count = 0;
int recorder[BUFFERSIZE]; // Variable que tiene como parámetro la memoria temporal 
unsigned long interval[BUFFERSIZE];  //Variable  que almacena los tonos precionados por los pulsadores
unsigned long pauseInterval[BUFFERSIZE];

void setup() {
  pinMode(SPK,OUTPUT); // El pin 13 la bocina se configura como salida
  pinMode(PLAYBACK_BTN,INPUT); // Se configura el botón de grabar y reproducir como entrada
  for(int i=0; i<length; i++){
    pinMode(keys[i],INPUT);
  }
}

void loop() {
  unsigned long startTime;
  static unsigned long pauseTime;
  static int playback_btn=0;
  int lastState = LOW;
  
  // Para evitar que el buffer se sature 
  if(count>=BUFFERSIZE) count=0;

  // Para reproducir la secuencia 
  if(digitalRead(PLAYBACK_BTN)==HIGH){
    playback();
    playback_btn=1;
  }
  
  // Para hacer que cada pulsador funcione 
  for(int i=0; i<length; i++){
    //startTime = micros();
    startTime = millis();
    while(digitalRead(keys[i])==HIGH){  //lee el estado de la llave, nuestras llaves son  los pulsadores 
      if(playback_btn){
        // Restablece   las llaves después de haberlas pulsado 
        playback_btn = 0;
        count=0;
      }
//Para que la grabación se detenga o se inicie 
      recorder[count]=i;
      tone(SPK,tones[i]);
      delay(200); // debounce
      lastState=HIGH;  // Ultimo estado del botón 
    }
    if(lastState==HIGH){  //si esta en alto 
      lastState=LOW; //Pasa a estar en bajo 
      noTone(SPK);  //No sonara nada 
      interval[count]=millis()-startTime;
      if(count)
        pauseInterval[count-1]=millis()-pauseTime;
      pauseTime=millis();
      count++;
    }
  }
}
//Para que suene el teclado 
void playback(){
  int index, key;
  for(int i=0; i<length; i++){
    pinMode(keys[i],OUTPUT);  //se configura cada llave 0 la presionada como salida 
  }
  for(int i=0; i<count; i++){
    index = recorder[i];
    key = keys[index];
    digitalWrite(key,HIGH); // Le da al pulsador un valor de alto (para que produzca el sonido cuando se precione)
    tone(SPK,tones[index]); //Graba ese sonido 
    delay(interval[i]); //Hace la pausa 
    digitalWrite(key,LOW); //Le da al pulsador presionado un valor de bajo (se detiene el sonido)
    noTone(SPK); //No suena 
    delay(pauseInterval[i]); // Se pausa 
  }
  for(int i=0; i<length; i++){
    pinMode(keys[i],INPUT);  // Se configura la llave 0  pulsador como entrada 
  }
}
