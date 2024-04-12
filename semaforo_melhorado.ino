// C++ code
//
const int trigger=3;
const int echo=4;
int dist;
String entrada;
const int led[]={9,10,12,11,8,5};
bool x[]={0,0,0,0,0};
int t[]={10000,2000,15000,200,0};
int estado=1;
const int botao =6;
unsigned long tempo,tempo2,tempo3;
void setup()
{
  Serial.begin(9600);
  for(int i=0;i<5;i++){
    pinMode(led[i], OUTPUT);
  }
  pinMode(botao,INPUT_PULLUP);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
}

void loop()
{
  ajuste();

  digitalWrite(trigger,LOW);
  delayMicroseconds(5);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  dist=pulseIn(echo,HIGH);
  dist = dist/58;

  switch(estado){
   case 1:
    x[2]=0; x[0]=1; 
    if(millis()-tempo>t[0]){
     estado=2;
     tempo=millis();
    }
       else if(digitalRead(botao)==0 && millis()-tempo3>t[4]){
       tempo3=millis();
       estado=2;
    }
    break;
    case 2:
    x[0]=0; x[2]=0; x[1]=1;
     if(millis()-tempo>t[1]){
     estado=3;
     tempo=millis();
    }
   
    break;
    case 3:
    x[1]=0; x[2]=1;
    if(dist<100 && millis()-tempo2>t[3]){
        x[3]=!digitalRead(led[5]);
        tempo2=millis();
    }
     else{x[3]=0;}
     if(millis()-tempo>t[2]){
     estado=1;
     tempo=millis();
    }
    break;
  }
  digitalWrite(led[0], x[0]);
  digitalWrite(led[1], x[1]);
  digitalWrite(led[2], x[2]);
  digitalWrite(led[3], x[0] );
  digitalWrite(led[4],x[2] || x[1]);
  digitalWrite(led[5], x[3]);
  
}

void ajuste() {
  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {
    // Lê a entrada do usuário até que um caractere de nova linha ('\n') seja recebido
    while (Serial.available()) {
      char c = Serial.read();
      
      // Adiciona o caractere à variável 'entrada' se não for um caractere de nova linha
      if (c != ';') {
         entrada += c;
        

       
       } 
      
else {
            if (entrada.indexOf("g")>0 ) {       
           t[0] = entrada.toInt();
            }
       else if (entrada.indexOf("y")>0) { // Verde
      t[1] = entrada.toInt();
       }
      else if (entrada.indexOf("r")>0) { // Verde
      t[2] = entrada.toInt();
        }
        else if (entrada.indexOf("p")>0) {
          t[4]=entrada.toInt(); 
        }
        
        // Então, imprime a entrada e limpa a variável 'entrada' para a próxima entrada
        Serial.print("Entrada recebida: "); Serial.print("tempo aplicado e cor: "); Serial.println(entrada);
        entrada = ""; // Limpa a variável para a próxima entrada
      }
    }
  }
}