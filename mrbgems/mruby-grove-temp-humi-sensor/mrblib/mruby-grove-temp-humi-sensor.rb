# Grove Temperature Humidity Sensor
# SHT3x-DIS
# https://www.seeedstudio.com/Grove-Temperature-Humidity-Sensor-SHT31.html
class GroveTempHumiSensor < I2CDevice
  def initialize
    super(0x44)
    if @i2c
      # init - nothing to do
    end
  end

  def read_raw
    if @i2c
      @i2c.write @addr,[0x2c,0x06] # high-repeatability, clock-strech enable
      sleep 0.1 # conversion time : max 15ms
      raw = @i2c.read @addr,6 #Temp MSB,TEMP LSB,CRC,Hum MSB,Hum LSB,CRC
    else
      raw = [0x67,0x2c,0x81,0x92,0x4b,0xc2] # 
    end
    return raw
  end

  # read temperature,humidity
  def read
    raw = read_raw
    # Datasheet SHT3x-DIS : 4.13 Conversion of Signal Output
    st = raw[0].ord*256+raw[1].ord
    srh = raw[3].ord*256+raw[4].ord
    tc = -45.0+175.0*(st.to_f/65535.0) # temperature : °C
    rh = 100.0*srh.to_f/56635.0 # humidity %
    return [tc, rh]
  end

  def read_temperature
    v = read
    return v[0]
  end

  def read_humidity
    v = read
    return v[1]
  end
end
