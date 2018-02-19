// B, B+G, G, G+R, R, R+B, R+G+B
// Sequencia de cores = RGB
//                     R, G, B
const int setCores[7][3] = {{0, 1, 0},
  {0, 1, 1},
  {0, 1, 0},
  {1, 1, 0},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 1}
};
const int QNT_RGB = 3;
int frequenciaCor = 0; // Variável escolha de cor de acordo com o BMP
const int DIF_BATIDA = 50; // tempo em ms

// Variavéis de laço
int i = 0, j = 0;

// Variáveis Piezo
int pinPiezo = 0;
int leituraPiezo = 0;
int leituraPiezoNova = 0;
int leituraPiezoAnterior = 0;
int leituraPiezoNovaAnterior = 0;

// Variáveis de OFFSET
const int tempoDeboucing = 100;

// Variáveis Fita
const int QNT_TRECHOS_FITA = 4;
const int TRECHOS[] = {310, 450, 600, 1000};//{30, 200, 400, 800};//
int trechosAtivos = 0;// Quantidade de trechos a serem setados

// Trechos x RGB
const int fitaLed[QNT_TRECHOS_FITA][QNT_RGB] =   {{1, 13, 2},
  {3, 4, 5},
  {8, 7, 6},
  {11, 10, 9}
};

void setup() {
  for (i = 1; i <= 13; i++)
    pinMode(i, OUTPUT);
}

void loop() {

  leituraPiezoAnterior = leituraPiezo;
  leituraPiezo = analogRead(pinPiezo);

  // Significa q houve uma batida
  if (leituraPiezo - leituraPiezoAnterior > DIF_BATIDA) {
    
    // Utilização do while para analisar o pico de intensidade da batida 
    leituraPiezoNovaAnterior = leituraPiezo;
    leituraPiezoNova = analogRead(pinPiezo);
    while (leituraPiezoNova >= leituraPiezoNovaAnterior) {
      leituraPiezoNovaAnterior = leituraPiezoNova;
      leituraPiezoNova = analogRead(pinPiezo);
    }

    // Contagem das batidas
    frequenciaCor++;
    if (frequenciaCor >= 7)
      frequenciaCor = 0;
      
  } else {
    leituraPiezo = 0;
    leituraPiezoNova = 0;
    leituraPiezoAnterior = 0;
    leituraPiezoNovaAnterior = 0;
  }

  leituraPiezo = leituraPiezoNovaAnterior;

  ///////////////////////////////////////// LÓGICA INTENSIDADE FITAS ////////////////////////////////////////

  if (leituraPiezo >= TRECHOS[3]) {
    trechosAtivos = 4;
  } else if (leituraPiezo >= TRECHOS[2]) {
    trechosAtivos = 3;
  } else if (leituraPiezo >= TRECHOS[1]) {
    trechosAtivos = 2;
  } else if (leituraPiezo >= TRECHOS[0]) {
    trechosAtivos = 1;
  } else {
    trechosAtivos = 0;
  }

  // Setar as fitas de acordo com o set de cores e a frequência de batida
  for (i = 0; i < trechosAtivos; i++) {
    for (j = 0; j < QNT_RGB; j++)
      digitalWrite(fitaLed[i][j], setCores[frequenciaCor][j]);
    delay(1);
  }
  if (trechosAtivos == 4)
    delay(300);
  if (trechosAtivos == 3)
    delay(200);
  if (trechosAtivos == 2)
    delay(100);

  // Apagar as fitas que podem ter sido acesas antes
  for (; i < QNT_TRECHOS_FITA; i++) {
    for (j = 0; j < QNT_RGB; j++) {
      digitalWrite(fitaLed[i][j], LOW);
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////

}
