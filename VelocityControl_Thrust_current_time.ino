#include <ESP32Servo.h>
#include <HX711.h>

// --- CONFIGURAÇÕES DO HARDWARE ---
const int escPin = 25;      
const int currentPin = 34;  
const int balancaDT  = 18;
const int balancaSCK = 19;

// --- CONFIGURAÇÕES ACS712 ---
const float VREF = 3.3;          
const float ADC_MAX = 4095.0;    
const int numAmostras = 250;     
float zeroCurrentOffset_V = 0.0; 

// Sensibilidade: 5A = 0.185 | 20A = 0.100 | 30A = 0.066
const float sensibilidade = 0.100; // Mantenha 0.100 para o modelo de 20A

// --- CONFIGURAÇÕES ESC ---
const int usMin = 1000;
const int usMax = 2000;

Servo esc;
float currentPercent = 0;    
float rampPercent = 0;       
const float rampStep = 0.5; 

// --- HX711 ---
HX711 escala;
const float FATOR_CALIBRACAO = -189.3; 

// =================================================================
// FUNÇÕES AUXILIARES
// =================================================================

float getTensaoEstabilizada() {
    long somaLeituras = 0;
    for (int i = 0; i < numAmostras; i++) {
        somaLeituras += analogRead(currentPin);
        delayMicroseconds(100);
    }
    return ((float)somaLeituras / numAmostras / ADC_MAX) * VREF;
}

float getCorrente() {
    float tensaoMedida = getTensaoEstabilizada();
    
    // Diferença absoluta para evitar problemas de inversão de fios
    float diferenca = abs(tensaoMedida - zeroCurrentOffset_V); 
    
    // Cálculo base
    float correnteBruta = diferenca / sensibilidade;

    // --- FATOR DE CORREÇÃO (Ajuste conforme seu multímetro) ---
    // Se o código lê 0.40 e o real é 0.24, multiplicamos por 0.6
    float correnteCorrigida = correnteBruta * 0.67; 

    if (correnteCorrigida < 0.12) return 0.0; // Filtro de ruído
    return correnteCorrigida; 
}

float lerPesoFiltrado() {
    float peso = escala.get_units(5); 
    if (abs(peso) < 2.0) peso = 0.0; 
    return peso;
}

// =================================================================
// SETUP
// =================================================================

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);

    Serial.println("\n--- INICIALIZANDO BANCADA DE TESTE ---");
    
    // Calibração de corrente no início
    Serial.println("Calibrando Corrente (Motor parado)...");
    delay(2000); 
    zeroCurrentOffset_V = getTensaoEstabilizada();
    
    Serial.print("Offset de Zero: "); Serial.print(zeroCurrentOffset_V, 4); Serial.println(" V");

    // HX711
    escala.begin(balancaDT, balancaSCK);
    escala.set_scale();
    escala.tare(); 
    escala.set_scale(FATOR_CALIBRACAO);
    Serial.println("Célula de carga tarada.");
    
    // ESC
    esc.attach(escPin, usMin, usMax);
    esc.writeMicroseconds(usMin);
    delay(2000);
    Serial.println("Sistema Pronto! Digite de 0 a 100.");
}

// =================================================================
// LOOP
// =================================================================

void loop() {
    // 1. Rampa do ESC
    if (rampPercent < currentPercent) rampPercent = min(rampPercent + rampStep, currentPercent);
    else if (rampPercent > currentPercent) rampPercent = max(rampPercent - rampStep, currentPercent);

    int escUs = (int)(rampPercent * (usMax - usMin) / 100.0 + usMin);
    esc.writeMicroseconds(escUs);

    // 2. Leituras
    float corrente = getCorrente();
    float peso     = lerPesoFiltrado();

    // 3. Serial Monitor
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) {
        lastPrint = millis();
        Serial.print("Alvo: "); Serial.print(currentPercent, 0);
        Serial.print("% | Real: "); Serial.print(rampPercent, 1);
        Serial.print("% | Corrente: "); Serial.print(corrente, 2);
        Serial.print(" A | Empuxo: "); Serial.print(peso, 1);
        Serial.println(" g");
    }

    // 4. Input Serial
    if (Serial.available() > 0) {
        float p = Serial.parseFloat();
        if (p >= 0.0 && p <= 100.0) currentPercent = p;
        while(Serial.available() > 0) Serial.read();
    }
    delay(10); 
}
