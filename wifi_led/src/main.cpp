#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define LED_PIN 2 // LED 灯引脚
//web的http服务默认端口
WebServer server(80);
//EMQX 服务信息（注意端口是 1883，非加密）
const char* mqtt_server = "192.168.31.236";  // 你的 EMQX 服务器 IP
const int mqtt_port = 1883;
const char* mqtt_user = "zt"; // EMQX 账号
const char* mqtt_pass = "zt200103"; // EMQX 密码
const char* mqtt_client_id = "esp32client";
// MQTT 客户端
WiFiClient espClient;
PubSubClient client(espClient);
// 路由器 WiFi 连接状态
bool wifiConnected = false;

//***************************************************创建接收MQTT客户端
// 接收到 MQTT 消息后的回调函数
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("收到来自话题 [");
  Serial.print(topic);
  Serial.print("] 的消息: ");
  String msg = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg = msg + (char)payload[i];
  }
  Serial.println();
  if(msg == "on") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
    bool success = client.publish("esp32", "led_on"); // 发布消息
    Serial.println(success ? "消息发布成功" : "消息发布失败");
  } else if(msg == "off") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
    bool success = client.publish("esp32", "led_off"); // 发布消息
    Serial.println(success ? "消息发布成功" : "消息发布失败");
  } else {
    Serial.println("未知命令！");
  }
}
// 连接和尝试重新连接
void reconnect() {
  while (!client.connected()) {
    Serial.print("尝试连接 MQTT ... ");
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
      Serial.println("连接成功！");
      client.subscribe("ZT"); // 订阅话题
    } else {
      Serial.print("连接失败，状态码: ");
      Serial.print(client.state());
      Serial.println(" 5 秒后重试");
      delay(5000);
    }
  }
}

//***********************************************接收来自网页的 WiFi 配置
void handleWifiConfig() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  // 解析 JSON
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Bad JSON\"}");
    return;
  }

  String ssid = doc["ssid"];
  String password = doc["password"];
  Serial.println("gat_SSID: " + ssid);
  Serial.println("get_密码: " + password);

  server.send(200, "application/json", "{\"status\":\"ok\"}");

  // TODO: 保存配置到 NVS / SPIFFS，然后重启连接
  WiFi.softAPdisconnect(true); // 关闭 AP
  WiFi.begin(ssid.c_str(), password.c_str());
  int retries = 20;
  while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
    delay(500);
    Serial.print(".");
  }
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("连接失败！");
  } else {
    Serial.println("连接路由器成功！");
    wifiConnected = 1;

    client.setServer(mqtt_server, mqtt_port);// 设置 MQTT 服务器信息
    client.setCallback(callback);
  }
}
//*************************************************** */
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // 启动 AP 模式
  WiFi.softAP("ESP32", "12345678");//wifi默认IP是
  Serial.println("AP started");

  // 启动 Web 服务器
  server.on("/wifi-config", HTTP_POST, handleWifiConfig);//参数：网页路径，HTTP请求方式POST，处理函数
  server.begin();
  //等待连接路由器
  while (!wifiConnected)
  {
    server.handleClient();
  }
  server.stop();
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
