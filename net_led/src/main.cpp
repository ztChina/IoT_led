//连接wifi
//基于wifi创建mqtt客户端
//连接emqx服务器
//订阅主题ZT
//根据订阅信息回调函数
#include <WiFi.h>
#include <PubSubClient.h>//MQTT 通信 的 Arduino 库
#include "ledControl.h"

// 1. 你的 WiFi 信息
const char* ssid = "Xiaomi_zt";
const char* password = "zt15297943496";

// 2. EMQX 服务信息（注意端口是 1883，非加密）
const char* mqtt_server = "192.168.31.236";  // 你的 EMQX 服务器 IP
const int mqtt_port = 1883;
const char* mqtt_user = "zt"; // EMQX 账号
const char* mqtt_pass = "zt200103"; // EMQX 密码
const char* mqtt_client_id = "esp32client";

// 3. MQTT 客户端
WiFiClient espClient;// WiFi 客户端，相当于一个通道
PubSubClient client(espClient); // 客户端，作为 MQTT 客户端，用 上面的 Wi-Fi 通道

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
    ledOn(); // 打开 LED 灯
    bool success = client.publish("esp32", "led_on"); // 发布消息
    Serial.println(success ? "消息发布成功" : "消息发布失败");
  } else if(msg == "off") {
    ledOff(); // 关闭 LED 灯
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

void setup() {
  Serial.begin(115200);
  delay(100);

  ledInit();

  WiFi.begin(ssid, password);
  Serial.print("连接 WiFi 中");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi 连接成功");

  client.setServer(mqtt_server, mqtt_port);// 设置 MQTT 服务器信息
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
