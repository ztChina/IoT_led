//esp信息配置界面
Page({
    data: {
        ESP32SSID: '',
        ESP32PWD: '',
        WIFI_SSID: '',
        WIFI_PWD: '',
        ESP32_IP: '192.168.4.1',//ESPAP模式默认的IP
    },
    //wifi SSID和PWD 输入
    SSIDInput(e)
    {
        this.setData({
            WIFI_SSID: e.detail.value
        })
    },
    PWDInput(e)
    {
        this.setData({
            WIFI_PWD: e.detail.value
        })
    },
    //上传wifi信息
    to_esp()
    {
        wx.request({
            url: 'http://192.168.4.1/wifi-config', // ESP32 的 IP + 接口路径
            method: 'POST',
            header: {
              'content-type': 'application/json' // 注意要设置为 JSON
            },
            data: {
              ssid: this.data.WIFI_SSID,
              password: this.data.WIFI_PWD
            },
            success: function (res) {
              console.log('发送成功:', res);
            },
            fail: function (err) {
              console.error('发送失败:', err);
            }
        });
        const app = getApp(); // 获取全局 app 实例
        // 将局部变量赋值给全局变量
        app.globalData.WIFI_SSID = this.data.WIFI_SSID;
        app.globalData.WIFI_PWD = this.data.WIFI_PWD;
        app.globalData.ESP32_IP = this.data.ESP32_IP;
        console.log('全局变量已设置:', app.globalData.ESP32SSID, app.globalData.ESP32PWD,app.globalData.ESP32_IP);
        //跳转到esp信息配置界面
        wx.navigateTo({
            url: '../index/index',
        })
        
    },
    onLoad() {
        //获取全局变量
        const app = getApp(); // 获取全局 app 实例
        this.setData({
            ESP32SSID: app.globalData.ESP32SSID,
            ESP32PWD: app.globalData.ESP32PWD,
        })
    },
})