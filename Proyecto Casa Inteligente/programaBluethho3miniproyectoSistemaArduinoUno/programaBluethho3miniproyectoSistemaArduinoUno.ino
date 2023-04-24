 // ASIGNACION DE PINES
const int desbloqueoAire = 18;
const int desbloqueoLuces = 19;
int seguroAire = 0;
int seguroSala = 0;
int seguroPatio = 0;
int seguroCocina = 0;
int controlAsistente = LOW;
int controlAire = LOW;
int controlLuces = LOW;
int controlSala = LOW;
int controlCocina = LOW;
int controlPatio = LOW;
int controlTodas = LOW;

const int encenderAire = 13;
const int encenderSala = 12;
const int encenderPatio = 11;
const int encenderCocina = 10;
const int encenderAsistente = 9;
const int seleccionLuces = 8;
const int todaslasLuces =7;
const int apagarAire = 6;
const int apagarSala = 5;
const int apagarPatio = 4;
const int apagarCocina = 3;
char valorblutuch;


void setup() {
  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(encenderAire, INPUT);
  pinMode(encenderSala, INPUT);
  pinMode(encenderPatio, INPUT);
  pinMode(encenderCocina, INPUT);
  pinMode(encenderAsistente, INPUT);
  pinMode(seleccionLuces, INPUT);
  pinMode(todaslasLuces, INPUT);
  digitalWrite(encenderAire, LOW);
  digitalWrite(encenderSala, LOW);
  digitalWrite(encenderPatio, LOW);
  digitalWrite(encenderCocina, LOW);
  digitalWrite(encenderAsistente, LOW);
  digitalWrite(seleccionLuces, LOW);
  digitalWrite(todaslasLuces, LOW);

  
  // CONFIGURAR PIN DE LED COMO SALIDA
  pinMode(apagarAire, OUTPUT);
  pinMode(apagarSala, OUTPUT);
  pinMode(apagarPatio, OUTPUT);
  pinMode(apagarCocina, OUTPUT);
  pinMode(desbloqueoAire, OUTPUT);
  pinMode(desbloqueoLuces, OUTPUT);
  digitalWrite(apagarAire, LOW);
  digitalWrite(apagarSala, LOW);
  digitalWrite(apagarPatio, LOW);
  digitalWrite(apagarCocina, LOW);
  pinMode(desbloqueoAire, LOW);
  pinMode(desbloqueoLuces, LOW);

  // Conexion bluetuch
  Serial.begin(9600);
}

void loop() {
  //LEE DATOS BLUTUCH
  
  if( Serial.available()){
    valorblutuch = Serial.read();
    if( valorblutuch =='a'){
      digitalWrite(apagarAire, LOW);
      digitalWrite(controlAire, LOW);
      digitalWrite(desbloqueoAire, HIGH);
      delay(100);
      digitalWrite(desbloqueoAire, LOW);
      }
    if( valorblutuch =='c'){
      digitalWrite(apagarSala, LOW);
      seguroSala = 1;
      }
    if( valorblutuch =='b'){
      digitalWrite(apagarPatio, LOW);
      seguroPatio = 1;
      }
    if( valorblutuch =='d'){
      digitalWrite(apagarCocina, LOW);
      seguroCocina = 1;
      }
    }

  if(seguroCocina == 1){
  digitalWrite(desbloqueoLuces, HIGH);
  delay(100);
  digitalWrite(desbloqueoLuces, LOW);
  }
  if(seguroCocina == 1 && seguroPatio == 1){
    digitalWrite(desbloqueoLuces, HIGH);
    delay(100);
    digitalWrite(desbloqueoLuces, LOW);
    delay(100);
    digitalWrite(desbloqueoLuces, HIGH);
    delay(100);
    digitalWrite(desbloqueoLuces, LOW); 
  }
  if(seguroSala == 1 && seguroPatio == 1 && seguroCocina == 1){
    digitalWrite(desbloqueoLuces, HIGH);
    delay(100);
    digitalWrite(desbloqueoLuces, LOW);
    delay(100);
    digitalWrite(desbloqueoLuces, HIGH);
    delay(100);
    digitalWrite(desbloqueoLuces, LOW); 
    delay(100);
    digitalWrite(desbloqueoLuces, HIGH);
    delay(100);
    digitalWrite(desbloqueoLuces, LOW);
    } 
  controlAsistente = digitalRead(encenderAsistente);
  controlAire = digitalRead(encenderAire);  
  controlLuces = digitalRead(seleccionLuces);
  controlSala = digitalRead(encenderSala);
  controlCocina = digitalRead(encenderCocina);
  controlPatio = digitalRead(encenderPatio);
  controlTodas = digitalRead(todaslasLuces);
  if (controlAsistente == HIGH) {
    int i = 1;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
  }
  if (controlAire == HIGH) {
    int i = 2;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
    digitalWrite(apagarAire, HIGH);
    seguroAire = 0;
  }
  if (controlSala == HIGH) {
    int i = 3;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
    digitalWrite(apagarSala, HIGH);
    seguroSala = 0;
  }
  if (controlPatio == HIGH) {
     int i = 4;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
    digitalWrite(apagarPatio, HIGH);
    seguroPatio = 0;
  }
  if (controlCocina == HIGH) {
     int i = 5;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
    digitalWrite(apagarCocina, HIGH);
    seguroCocina = 0;
  }
  if (controlLuces == HIGH) {
    int i = 6;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
  }
  if (controlTodas == HIGH) {
    int i = 7;
    Serial.print(i);
    Serial.print("\n");
    delay(300);
    digitalWrite(apagarSala, HIGH);
    digitalWrite(apagarCocina, HIGH);
    digitalWrite(apagarPatio, HIGH);
  }

}
