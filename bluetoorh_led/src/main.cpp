//使用蓝牙进行LED控制
//蓝牙ID: ESP32-BLE-LED
//蓝牙UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
//蓝牙特性UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E
//输入命令：on/off 或 0x01/0x00 对应    开/关

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN 2  //LED连接的引脚

BLECharacteristic *pCharacteristic;// BLE特性对象，类似一个接口
bool deviceConnected = false;
std::string rxValue;
BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();// 创建一个广播对象

// 特性（接口）回调类
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {// 当有数据写入时触发
        rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0) {
          uint8_t cmd = rxValue[0];  // 获取第一个字节
          if (rxValue == "on" || cmd == 0x01) {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ON");
            } else if (rxValue == "off" || cmd == 0x00) {
                digitalWrite(LED_PIN, LOW);
                Serial.println("LED OFF");
            } 
        }
    }
};

// 服务器回调类
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("A client has connected!"); // 打印连接信息
        deviceConnected = true; // 标记设备已连接
    }

    void onDisconnect(BLEServer* pServer) {
        Serial.println("Client has disconnected!"); // 打印断开信息
        deviceConnected = false; // 标记设备已断开
        pAdvertising->start();// 开始广播，使设备可被发现
        Serial.println("BLE device is now advertising agian...");
    }
};
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    // 初始化 BLE
    BLEDevice::init("ESP32-BLE-LED");// 设置设备名称
    BLEServer *pServer = BLEDevice::createServer();// 创建一个服务器
    pServer->setCallbacks(new MyServerCallbacks()); // 服务器回调类，包括连接、断开等各种操作

    BLEService *pService = pServer->createService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");// 创建一个服务，服务UUID是标准的，可以define

    pCharacteristic = pService->createCharacteristic(   // 创建一个特性
        "6E400002-B5A3-F393-E0A9-E50E24DCCA9E",  // RX
        BLECharacteristic::PROPERTY_WRITE   // 特性属性：只允许写入
    );

    pCharacteristic->setCallbacks(new MyCallbacks());// 回调函数类，包括写入、读取等各种操作

    // 启动服务
    pService->start();

    // 开始广播
    pAdvertising->start();// 开始广播，使设备可被发现
    Serial.println("BLE device is now advertising...");
}

void loop() {
    // 无需重复处理，回调里已经处理了写入命令
    delay(100);
}
