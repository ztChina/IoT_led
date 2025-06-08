Page({
  data: {
    deviceList: [],
    selectedDevice: {},
    deviceId: '',
    serviceId: '',
    characteristicId: '',
    connected: false,
    inputValue: ''
  },


  //*****************初始化蓝牙按键回调函数******************//
  //初始化主函数
  initBluetooth() {
    wx.openBluetoothAdapter({   //打开蓝牙适配器
      success: () => {
        this.searchBLE();
      },
      fail: () => {
        wx.showToast({ title: '请开启蓝牙', icon: 'none' });
      }
    });
  },
  //搜索蓝牙函数，搜索时间为1秒
  searchBLE() {
    wx.startBluetoothDevicesDiscovery({
      allowDuplicatesKey: false,
      success: () => {
        setTimeout(() => {
          wx.getBluetoothDevices({//1秒后获取搜索到的蓝牙设备
            success: res => {
              this.setData({ deviceList: res.devices });//更新设备列表
            }
          });
        }, 1000);
      }
    });
  },

  //*****************选择蓝牙函数******************//
  onDeviceSelect(e) {
    const index = e.detail.value;
    const selected = this.data.deviceList[index];
    this.setData({ selectedDevice: selected });
  },

  //*****************连接蓝牙******************//
  startBluetooth() {
    const { selectedDevice } = this.data;
    if (!selectedDevice.deviceId) {
      wx.showToast({ title: '请先选择设备', icon: 'none' });
      return;
    }
    //连接蓝牙设备
    wx.createBLEConnection({
      deviceId: selectedDevice.deviceId,

      success: () => {
        this.setData({ connected: true, deviceId: selectedDevice.deviceId });
        wx.getBLEDeviceServices({
          deviceId: selectedDevice.deviceId,
          success: res => {
            const service = res.services.find(s => s.uuid.includes('6E400001'));
            if (service) {
              this.setData({ serviceId: service.uuid });
              this.getCharacteristics(selectedDevice.deviceId, service.uuid);
            }
          }
        });
      }

    });
  },
  //获取蓝牙特征值函数
  getCharacteristics(deviceId, serviceId) {
    wx.getBLEDeviceCharacteristics({
      deviceId,
      serviceId,
      success: res => {
        const char = res.characteristics.find(c => c.properties.write);
        if (char) {
          this.setData({ characteristicId: char.uuid });
        }
      }
    });
  },

  //*****************发送数据******************//
  sendData() {
    const str = this.data.inputValue;
    const buffer = new ArrayBuffer(str.length);
    const dataView = new DataView(buffer);
    for (let i = 0; i < str.length; i++) {
      dataView.setUint8(i, str.charCodeAt(i));
    }

    wx.writeBLECharacteristicValue({
      deviceId: this.data.deviceId,
      serviceId: this.data.serviceId,
      characteristicId: this.data.characteristicId,
      value: buffer,
      success: () => {
        wx.showToast({ title: '发送成功' });
      },
      fail: (err) => {
        console.error("发送失败：", err);
        wx.showToast({ title: '发送失败', icon: 'none' });
      }
    });
  },
    /*********LED开关按键******** */
    led_on(e)
    {
        const str = "on";
        const buffer = new ArrayBuffer(str.length);
        const dataView = new DataView(buffer);
        for (let i = 0; i < str.length; i++) {
        dataView.setUint8(i, str.charCodeAt(i));
        }

        wx.writeBLECharacteristicValue({
        deviceId: this.data.deviceId,
        serviceId: this.data.serviceId,
        characteristicId: this.data.characteristicId,
        value: buffer,
        success: () => {
            wx.showToast({ title: '发送成功' });
        },
        fail: (err) => {
            console.error("发送失败：", err);
            wx.showToast({ title: '发送失败', icon: 'none' });
        }
        });
    },
    led_off(e)
    {
        const str = "off";
        const buffer = new ArrayBuffer(str.length);
        const dataView = new DataView(buffer);
        for (let i = 0; i < str.length; i++) {
        dataView.setUint8(i, str.charCodeAt(i));
        }

        wx.writeBLECharacteristicValue({
        deviceId: this.data.deviceId,
        serviceId: this.data.serviceId,
        characteristicId: this.data.characteristicId,
        value: buffer,
        success: () => {
            wx.showToast({ title: '发送成功' });
        },
        fail: (err) => {
            console.error("发送失败：", err);
            wx.showToast({ title: '发送失败', icon: 'none' });
        }
        });
    }
});
