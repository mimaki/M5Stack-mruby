# M5Stack-mruby

<table align="right"><tr><td><a href="README.md">English</a></td></tr></table><br/>

M5Stack-mrubyは、LCD付きのマイコン[M5Stack](https://m5stack.com/)で動作する[mruby](http://mruby.org/)実行環境です。

以下のmrubyのプログラミング環境を提供します。

- mrubyアプリケーション(バイナリ)の実行
- 対話モードでのmrubyスクリプト実行

# 導入手順

## 事前準備

`M5Stack-mruby`のビルドに必要な以下をインストールしてください。

- [Ruby](https://www.ruby-lang.org/)
- [GNU Bison](https://www.gnu.org/software/bison/)
- [Git](https://git-scm.com/)
- [esp-idf](https://github.com/espressif/esp-idf)

## M5Stack-mrubyのダウンロード

`esp-idf`をインストールしたディレクトリに移動して、`M5Stack-mruby`のソースコードを取得します。

```bash
cd ~/esp
git clone --recursive https://github.com/mimaki/M5Stack-mruby.git
```

## ビルドとM5Stackへの書き込み

`M5Stack`とPCをUSBケーブルで接続し、`M5Stack-mruby`ディレクトリ内で以下のコマンドを実行します。

```bash
make flash
```

## アプリケーションの実行

`M5Stack-mruby`では、mrubyコンパイラ(mrbc)でコンパイルしたmrubyアプリケーションが実行できます。  
mrubyコンパイラ(mrbc)は``M5Stack-mruby/components/mruby_component/mruby/bin``ディレクトリに生成されます。

サンプルプログラム`japan.rb`とそれをコンパイルする手順を以下に示します。

### サンプルプログラム japan.rb

```rb
LCD.clear
LCD.fill_rect(50, 40, 219, 159, LCD::WHITE)
LCD.fill_circle(160, 120, 50, LCD::RED)
gets
```

### コンパイル

```bash
M5Stack-mruby/components/mruby_component/mruby/bin/mrbc -o autorun.mrb japan.rb
```

ルートディレクトリに`autorun.mrb`をコピーしたmicroSDカードをM5StackのSDカードスロットに装着して、M5Stackの電源を投入（またはリセット）することで、microSDカードに保存したmrubyアプリーションが実行されます。

#### 注意

- 実行するmrubyアプリケーションバイナリのファイル名は`autorun.mrb`としてください
- `autorun.mrb`はmicroSDカードのルートディレクトリにコピーしてください

---
## 対話モードの実行

[CoolTerm](http://freeware.the-meiers.org/)等のターミナルアプリケーションを使用することで、`M5Stack-mruby`の対話モードが利用できます。  
[CoolTerm](http://freeware.the-meiers.org/)を使用した場合の操作を以下に示します。

### CoolTermの初期設定

M5StackをUSBケーブルでPCに接続し、CoolTermの`Options`を開き、以下の項目を設定します。

#### Serial Port Options

|項目|設定内容|備考|
|:-:|:-:|:--|
|Port|Mac: SLAB_USBtoUART<br/>Windows: COMx|COMxの番号は環境によって異なります|
|Baudrate|115200|　|
|Data Bits|8|　|
|Parity|none|　|
|Stop Bits|1|　|

`Port`が見つからない場合は、[VCPドライバ](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)が正しくインストールされていることを確認して下さい。

#### Terminal Options

|項目|設定内容|
|:-:|:-:|
|Terminal Mode|Line Mode|
|Enter Key Emulation|CR|
|Local Echo|ON|

### 対話モードの実行

1. `CoolTerm`の`Connect`をクリックします
2. M5StackにmicroSDカードが装着されていない状態で、M5Stackの`RESETボタン`を押下します
3. CoolTermの下部の入力ボックスにRubyスクリプトと`Enter`を入力すると、Rubyスクリプトが実行できます
