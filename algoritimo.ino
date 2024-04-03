String entrada,nome;
////////////////////////////////
const int trigger=10;
const int echo=9;
float dist;
// Definição dos pinos para os LEDs
const int greenLED = 8;
const int yellowLED = 11;
const int redLED = 12;
const int pedestrianGreenLED = 7;
const int pedestrianRedLED = 5;
const int pedestrianButton = 6;
const int extraLED = 4; // Novo LED para piscar quando o sinal estiver vermelho
// Definição dos tempos de cada estado do semáforo em milissegundos
 int greenTime2=0;
 unsigned long greenTime = 10000; // 10 segundos
 unsigned long yellowTime = 2000; // 2 segundos
 unsigned long redTime = 15000; // 15 segundos
 unsigned long pedestrianGreenTime = 15000; // 15 segundos
const unsigned long blinkInterval = 200; // Intervalo de piscar do LED
// Variáveis para controlar o estado atual e o tempo decorrido
int currentState = 0; // 0 - verde, 1 - amarelo, 2 - vermelho
unsigned long previousMillis = 0;
bool pedestrianButtonPressed = false;
bool blinkState = false;
unsigned long previousBlinkMillis = 0;

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
            if (entrada.indexOf("green")>0 ) {
              
      greenTime = entrada.toInt();
      greenTime2 =entrada.toInt();
      }
       else if (entrada.indexOf("yellow")>0) { // Verde

      yellowTime = entrada.toInt();
       }
      else if (entrada.indexOf("red")>0) { // Verde

      redTime = entrada.toInt();
        }
        else if (entrada.indexOf("pedestre")>0) {
          pedestrianGreenTime=entrada.toInt(); 
        }
        
        // Então, imprime a entrada e limpa a variável 'entrada' para a próxima entrada
        Serial.print("Entrada recebida: "); Serial.print("tempo aplicado e cor: "); Serial.println(entrada);
        entrada = ""; // Limpa a variável para a próxima entrada
      }
    }
  }
}
void setup() {
  Serial.begin(9600);
  Serial.println("digite o tempo em milisegundos e qual cor deseja aplicar esse tempo em seguida ; para finaliza a instrução");
  Serial.println("EX: 2500green; 2500yellow; 2500red; 500pedestre;");
  // Configura os pinos dos LEDs como saída
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(pedestrianGreenLED, OUTPUT);
  pinMode(pedestrianRedLED, OUTPUT);
  //Configura o pino de entrada do sensor
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(pedestrianButton, INPUT_PULLUP);
  pinMode(extraLED, OUTPUT); // Novo LED para piscar quando o sinal estiver vermelho
}
void loop() {
  ajuste();
  unsigned long currentMillis = millis(); // Obtém o tempo atual em milissegundos
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  dist=pulseIn(echo,HIGH);
  dist = dist/58;
  // Verifica se o botão de pedestres foi pressionado
  if (digitalRead(pedestrianButton) == LOW && currentState !=2) {
    pedestrianButtonPressed = true;
     currentState = 1; // Mudar para pedestre verde
        previousMillis = currentMillis;
  }
       
  // Atualiza o estado do semáforo conforme o tempo passa
  switch (currentState) {
    case 0: // Verde
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
      digitalWrite(pedestrianGreenLED, LOW);
      digitalWrite(pedestrianRedLED, HIGH);
      digitalWrite(extraLED, LOW);
      if (currentMillis - previousMillis >= greenTime) {
        currentState = 1; // Mudar para amarelo
        previousMillis = currentMillis;
      }

      break;
    case 1: // Amarelo
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(redLED, LOW);
      digitalWrite(pedestrianGreenLED, LOW);
      digitalWrite(pedestrianRedLED, HIGH);
      digitalWrite(extraLED, LOW);
      if (currentMillis - previousMillis >= yellowTime) {
              if(pedestrianButtonPressed == true){
          currentState = 3;
        }
        else{
            currentState = 2; // Mudar para vermelho
        }
        
        previousMillis = currentMillis;
      }
      break;
    case 2: // Vermelho
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
      digitalWrite(pedestrianRedLED, LOW);
      digitalWrite(pedestrianGreenLED, HIGH);
      digitalWrite(extraLED, LOW);
      if (currentMillis - previousMillis >= redTime) {
  
        currentState = 0; // Mudar para verde
        previousMillis = currentMillis;
      }
      // Se o botão de pedestres foi pressionado, mudar o semáforo para pedestres para verde
      
      break;
    case 3: // Pedestre verde
    
   pedestrianButtonPressed = false;
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
      digitalWrite(pedestrianGreenLED, HIGH);
      digitalWrite(pedestrianRedLED, LOW);
      digitalWrite(extraLED, LOW);
      if (currentMillis - previousMillis >= pedestrianGreenTime) {
        currentState = 4; // Mudar para pedestre vermelho
        previousMillis = currentMillis;
      }
      break;
    case 4: // Pedestre vermelho
      digitalWrite(pedestrianGreenLED, LOW);
      digitalWrite(pedestrianRedLED, HIGH);
      currentState = 0; // Mudar para verde dos carros
      previousMillis = currentMillis;
      break;
  }
  // Piscar LED extra quando o sinal estiver vermelho
  if (dist<100 && currentState == 2 && currentMillis - previousBlinkMillis >= blinkInterval) {
    previousBlinkMillis = currentMillis;
    blinkState = !blinkState; // Alterna o estado do LED
    digitalWrite(extraLED, blinkState ? HIGH : LOW);
  } else if (currentState != 2) { // Se o sinal não estiver vermelho, mantenha o LED extra desligado
    digitalWrite(extraLED, LOW);
  }
}

