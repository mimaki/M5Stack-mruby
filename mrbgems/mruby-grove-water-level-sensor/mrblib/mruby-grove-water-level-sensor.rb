# Water Level Sensor
# https://wiki.seeedstudio.com/Grove-Water-Level-Sensor/
# I2C Address 0x78,0x77 (High 12, Low 8)
# https://wiki.seeedstudio.com/Grove-Water-Level-Sensor/#software
class GroveWaterLevelSensor < I2CDevice
  THRESHOLD = 100

  def initialize
    super(0x78)
    if @i2c
      # init - nothing to do
    end
  end

  def read_raw
    if @i2c
      lo = @i2c.read @addr-1,8
      hi = @i2c.read @addr,12
      touch_lo = []
      touch_hi = []
      lo.each_char {|c| touch_lo.push( c.ord ) }
      hi.each_char {|c| touch_hi.push( c.ord ) }
    else
      touch_lo = [255,255,255,255,255,255,255,255]
      touch_hi = [255,255,0,0,0,0,0,0,0,0]
    end
    raw = touch_lo + touch_hi
    return raw
  end

  # read 
  def read
    # count touches over THRESHOLD
    raw = read_raw
    touch = 0
    raw.each do |v|
      if v > THRESHOLD
        touch += 1
      end
    end
    # 20 sensors on board, so 5% / touch
    return touch*5
  end
end
