#include <TFT_eSPI.h>      // Biblioteca para TFT (suporta touch se for capacitivo)
#include <SPI.h>

// --- Pinos do sensor ---
#define TRIG_PIN 9
#define ECHO_PIN 10

// --- Configuração da tela ---
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite listSprite = TFT_eSprite(&tft); // sprite para desenhar lista

// --- Variáveis do sensor ---
float distancia = 0;
float altura_tanque = 30.0; // em cm, ajuste para seu tanque

// --- Lista de leituras ---
#define MAX_ITENS 5
float nivelList[MAX_ITENS];
int indexList = 0;

// --- Intervalo de medição ---
unsigned long lastUpdate = 0;
const unsigned long interval = 5000; // 5 segundos

// --- Função para medir distância ---
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms (~5m)
  float distancia = (duracao / 2.0) * 0.0343; // cm
  return distancia;
}

// --- Função para atualizar lista ---
void atualizarLista(float nivel) {
  nivelList[indexList] = nivel;
  indexList = (indexList + 1) % MAX_ITENS;
}

// --- Função para desenhar lista ---
void desenharLista() {
  listSprite.fillSprite(TFT_BLACK);
  listSprite.setTextColor(TFT_WHITE);
  listSprite.setTextSize(2);

  for (int i = 0; i < MAX_ITENS; i++) {
    int pos = (indexList + i) % MAX_ITENS;
    listSprite.drawString("Nivel: " + String(nivelList[pos], 1) + " cm", 0, i*20);
  }

  listSprite.pushSprite(0, 0);
}

// --- Setup ---
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  listSprite.createSprite(160, 120); // ajuste ao tamanho do display
}

// --- Loop principal ---
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= interval) {
    lastUpdate = currentTime;

    float distanciaMedida = medirDistancia();
    float nivelAgua = altura_tanque - distanciaMedida; // nível relativo
    nivelAgua = max(0.0, nivelAgua); // evita negativo

    // Atualiza lista e desenha na tela
    atualizarLista(nivelAgua);
    desenharLista();

    // Envia evento via serial se houver mudança >1cm
    if (abs(nivelAgua - nivelList[(indexList-1 + MAX_ITENS)%MAX_ITENS]) > 1.0) {
      Serial.println("Evento: Mudança significativa! Nivel = " + String(nivelAgua,1) + " cm");
    }

    Serial.println("Nivel atual: " + String(nivelAgua,1) + " cm");
  }
}