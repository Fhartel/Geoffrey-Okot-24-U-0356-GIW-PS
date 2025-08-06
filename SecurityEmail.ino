#include <WiFi.h>
#include <ESP_Mail_Client.h>

// Wi-Fi
const char* ssid = "Godyneyo";
const char* password = "Geoffrey";

// Gmail SMTP
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "Godyneyo@gmail.com"         // Your Gmail
#define AUTHOR_PASSWORD "qifnhvtnmgmsyjhc"        // App Password from step 2
#define RECIPIENT_EMAIL "fharteljeff@gmail.com"      // Where the alert goes

SMTPSession smtp;

void sendEmail() {
  SMTP_Message message;

  message.sender.name = "ESP32 Security";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "🚨 Motion Detected!";
  message.addRecipient("User", RECIPIENT_EMAIL);

  message.text.content = "Alert: Motion was detected by your ESP32 system.";
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  smtp.debug(1);
  smtp.callback([](SMTP_Status status) {
    Serial.println(status.info());
  });

  if (!smtp.connect(&session)) {
    Serial.println("SMTP connection failed: " + smtp.errorReason());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Failed to send mail: " + smtp.errorReason());
  } else {
    Serial.println("✅ Mail sent!");
  }

  smtp.closeSession();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  sendEmail();
}

void loop() {
  int motionDetected = digitalRead(pirPin);
  
  if (motionDetected == HIGH) {
    Serial.println("Motion detected!");
  } else {
    Serial.println("No motion.");
  }
  delay(500);
  
}