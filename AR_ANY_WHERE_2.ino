#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// ================= WIFI =================
#define WIFI_SSID "JioFiber-Suni"
#define WIFI_PASSWORD "3521@KiiT"

// ================= FIREBASE =================
#define API_KEY "AIzaSyDB9A6IHFgpGm4k6EZklfRBrDyOz-NFtiA"
#define DATABASE_URL "https://esp-ar-default-rtdb.firebaseio.com/"

// ================= LED PINS =================
#define LED1_PIN 4
#define LED2_PIN 19

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

void setup() {

  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Anonymous authentication
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("SignUp error: %s\n", config.signer.signupError.message.c_str());
  }
}

void loop() {

  if (Firebase.ready() && signupOK) {

    // ===== LED1 =====
    if (Firebase.RTDB.getInt(&fbdo, "/led1")) {

      int value1 = fbdo.intData();
      Serial.print("LED1: ");
      Serial.println(value1);

      digitalWrite(LED1_PIN, value1 ? HIGH : LOW);

      // If LED works reverse (Active LOW), use this instead:
      // digitalWrite(LED1_PIN, value1 ? LOW : HIGH);
    }

    // ===== LED2 =====
    if (Firebase.RTDB.getInt(&fbdo, "/led2")) {

      int value2 = fbdo.intData();
      Serial.print("LED2: ");
      Serial.println(value2);

      digitalWrite(LED2_PIN, value2 ? HIGH : LOW);

      // For Active LOW:
      // digitalWrite(LED2_PIN, value2 ? LOW : HIGH);
    }
  }

  delay(1000);
}