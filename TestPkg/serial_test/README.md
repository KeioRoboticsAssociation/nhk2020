# serial_test package

## ROS 側

---

### 起動

ノード名：**serial_test_node**

launch ファイル：**serial_test.launch**

パラメータ：

- **port**  
  ポート名  
  デフォルト：`/dev/ttyACM0`
- **baudrate**  
  シリアル通信のボーレート(マイコン側と合わせる)  
  デフォルト：`B115200`
- **looprate**  
  通信読み取りの周期[Hz]  
  デフォルト：`200`

### トピック・パブリッシュ

- **/Serial_pub_float**  
   メッセージ型：std_msgs::Float32MultiArray
- **/Serial_pub_int**  
   メッセージ型：std_msgs::Int32MultiArray
- **/Serial_pub_string**  
   メッセージ型：std_msgs::String

### トピック・サブスクライブ

- **/Serial_sub_float**  
   メッセージ型：std_msgs::Float32MultiArray
- **/Serial_sub_int**  
   メッセージ型：std_msgs::Int32MultiArray
- **/Serial_sub_string**  
   メッセージ型：std_msgs::String

※パブリッシュ/サブスクライブはは最大**200Hz**まで

### 仕様

配列のサイズはなんでもよい  
文字列の出力 (`/Serial_pub_string`) は文字列の最後に `\n` がつく

<br>

## mbed 側

---

### ソースコードの場所

platformio\\nucleo\\src の`mbedserial.cpp`と`mbedserial.h`
※`example.cpp` はサンプルコード

### 使い方

- 宣言

```c++
// 普通のシリアル通信の宣言
// ボーレートも明記するとよい
Serial pc(USBTX, USBRX, 115200);
// 今回作ったクラスの宣言
//引数は上で宣言したmbed::Serialクラス
Mbedserial Ms(pc);
```

- 送信
  - Float 型
    `Ms.float_write(float *array, int arraysize);`  
    引数 = ( Float 型の配列またはポインタ, 配列のサイズ )
  - Int 型
    `Ms.int_write(int *array, int arraysize);`  
    引数 = ( Int 型の配列またはポインタ, 配列のサイズ )
  - Char 型
    `Ms.char_write(char *array, int arraysize);`  
    引数 = ( Char 型の配列またはポインタ, 配列のサイズ )
- 受信

  - Float 型
    `float Mbedserial::getfloat[]`  
    受信した配列(Float 型のポインタ)  
    `int Mbedserial::floatarraysize`  
    受信した配列の長さ(Int)
  - Int 型
    `int Mbedserial::getint[]`  
     受信した配列(Int 型のポインタ)  
     `int Mbedserial::intarraysize`  
     受信した配列の長さ(Int)
  - Char 型
    `char Mbedserial::getchar[]`  
     受信した配列(Char 型のポインタ)  
     `int Mbedserial::chararraysize`  
     受信した配列の長さ(Int)

- 受信コールバック関数
  PC から受信した時に呼び出される関数の設定
  呼び出す関数は void 型・引数無し

  - Float 型
    `Ms.float_attach(void (*pfunc)())`
  - Int 型
    `Ms.int_attach(void (*pfunc)())`
  - Char 型
    `Ms.char_attach(void (*pfunc)())`

  **注意!**  
  コールバック関数には`wait`を入れないこと

### サンプルコード

受信した値をそのまま返す処理
<details>
<summary>example.cpp</summary>

```c++
// test code

#include "mbedserial.h"

Serial pc(USBTX, USBRX, 115200);
Mbedserial Ms(pc);

void CallBack_float();
void CallBack_int();
void CallBack_char();

int main()
{
  //受信コールバック関数の設定
  Ms.float_attach(CallBack_float);
  Ms.int_attach(CallBack_int);
  Ms.char_attach(CallBack_char);

  while (1)
  {
    wait(0.1);
  }
}

// コールバック関数の定義
void CallBack_float()
{
  // 受信
  int size = Ms.floatarraysize; // 配列サイズを取得
  float *f = Ms.getfloat;       // データを取得
  // 送信
  Ms.float_write(f, size);
}

void CallBack_int()
{
  // 受信
  int size = Ms.intarraysize; // 配列サイズを取得
  int *i = Ms.getint;         // データを取得
  // 送信
  Ms.int_write(i, size);
}

void CallBack_char()
{
  // 受信
  int size = Ms.chararraysize; // 配列サイズを取得
  char *c = Ms.getchar;        // データを取得
  // 送信
  Ms.char_write(c, size);
}
```
</details>