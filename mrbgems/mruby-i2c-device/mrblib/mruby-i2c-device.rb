class I2CDevice
  @i2c = nil
  @addr = -1

  def I2CDevice.mruby?
    return RUBY_ENGINE=="mruby"
  end

  def initialize(addr)
    @addr = addr
    if I2CDevice.mruby?
      @i2c = I2C.new @addr,nil
    end
  end
end
