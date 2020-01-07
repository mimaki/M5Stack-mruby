# M5Stack-mruby

<table align="right"><tr><td><a href="README_ja.md">Japanese</a></td></tr></table><br/>

M5Stack-mruby is [mruby](http://mruby.org/) runtime environment on [M5Stack](https://m5stack.com/) micro computer.

M5Stack-mruby provide mruby programming platform in below.

- Execution of mruby application (binary).
- Execution of mruby script in interactive mode.

# How to setup

## Preparation

Please install the following software required to build `M5Stack-mruby`.

- [Ruby](https://www.ruby-lang.org/)
- [GNU Bison](https://www.gnu.org/software/bison/)
- [Git](https://git-scm.com/)
- [esp-idf](https://github.com/espressif/esp-idf)

## Download M5Stack-mruby

Change to the directory where you installed `esp-idf` and get the source code of `M5Stack-mruby`.

```bash
cd ~/esp
git clone --recursive https://github.com/mimaki/M5Stack-mruby.git
```

## Build and write to M5Stack

Connect M5Stack and PC with USB cable.  
Type below command at `M5Stack-mruby` directory.

```bash
make flash
```

## Execution of mruby application

`M5Stack-mruby` can execute mruby application (binary) compiled with mruby compiler (mrbc).  
The mruby compiler (mrbc) is generated in the ``M5Stack-mruby/components/mruby_component/mruby/bin`` directory.

The sample program `japan.rb` and the procedure to compile it are shown below.

### Sample program (japan.rb)

```rb
LCD.clear
LCD.fill_rect(50, 40, 219, 159, LCD::WHITE)
LCD.fill_circle(160, 120, 50, LCD::RED)
gets
```

### Compile

```bash
M5Stack-mruby/components/mruby_component/mruby/bin/mrbc -o autorun.mrb japan.rb
```

Insert the microSD card whose `autorun.mrb` is copied to the root directory into the M5Stack's SD card slot and power on (or reset) the M5Stack to execute the mruby application saved on the microSD card.


#### Note

- The file name of the mruby application binary to execute should be `autorun.mrb`.
- Copy `autorun.mrb` to the microSD card root directory.

---
## Execution of mruby script in interactive mode

By using a terminal application such as [CoolTerm](http://freeware.the-meiers.org/), `M5Stack-mruby` interactive mode can be used.  
The operation when using [CoolTerm](http://freeware.the-meiers.org/) is shown below.

### Setting of CoolTerm

Connect M5Stack to PC with USB cable, open CoolTerm `Options`, and set the following items.

#### Serial Port Options

|Item|Value|Note|
|:-:|:-:|:--|
|Port|Mac: SLAB_USBtoUART<br/>Windows: COMx|`COMx` number is different depending on the environment.|
|Baudrate|115200|　|
|Data Bits|8|　|
|Parity|none|　|
|Stop Bits|1|　|

If you can not find `Port`, confirm the [VCP driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) is correctly installed.

#### Terminal Options

|Item|Value|
|:-:|:-:|
|Terminal Mode|Line Mode|
|Enter Key Emulation|CR|
|Local Echo|ON|

### Run interactive mode

1. Click `Connect` on `CoolTerm`.
2. Remove the microSD card and press the M5Stack `RESET button`.
3. Ruby script is executed when Ruby script and `Enter` are input in the input box at the bottom of `CoolTerm`.
