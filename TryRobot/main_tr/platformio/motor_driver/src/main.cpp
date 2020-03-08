/*/////////////////////////////////変更する必要のある定数///////////////////////////////////////

#define STYLE1 MotorA:Table MotorB:Wheel, else MotorA:Wheel MotorB:Table

#define MAX_WHEEL_SPEED 6000 //ジョイスティック入力での最高速度(°/s)←多分これは高めの設定だと思う
#define MAX_TABLE_SPEED 2000 //ジョイスティック入力での最高速度(°/s)←多分これは高めの設定だと思う
PIDで速度が出ないときは　比例定数上げる or MAX_SPEEDを下げる

#define I_SET_t 50
#define I_SET_w 50
#define D_SET_t 50
#define D_SET_w 50 //I制御とD制御の影響の具合の上限値。50は適当な値です。(単位は°/s)

226行目
diff_t[1] = -1 * (transform_gear_into_encoder(target_angle, 1) - en_count_t); //エンコーダーの増減の符号とモーターの回転向きを考慮し全体に-1を乗じている
↑-1かけているのは前のMDでの話で、今はどうなっているかわからないのでモーターを正転させたときにエンコーダ―がどの方向に増えるかチェック

////////////////////////////////////////////////////////////////////////////////////////////*/

#include <mbed.h>
#include <math.h>
#include "motor.h"
#include "mbedserial.h"

#define MAXDUTY_W 0.8F
#define MAXDUTY_T 0.4F

Serial pc(USBTX, USBRX, 115200);
Ticker ticker;
#define SUMPLING_TIME_US 10000 //タイマー割り込み周期 kHzオーダーつまり1000が理想

Mbedserial Ms(pc); /////////////////////////////////

DigitalIn SW1(PB_7);
DigitalIn SW2(PB_6);
DigitalOut LED(LED3);
DigitalOut OE(PF_1, 0); // レベルシフタを有効化

//----------------------------------------Motor---------------------------------------------------
#define STYLE_RF //MotorA:Table MotorB:Wheel, else MotorA:Wheel MotorB:Table

#ifdef STYLE_RF
//MotorA
Motor Wheel(PB_5, PA_10, PA_11_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_W PA_8
#define ENB_W PA_9
//MotorB
Motor Table(PB_4, PB_0_ALT0, PB_1_ALT0); //EN_GD, PWM_p, PWM_n

#define ENA_T PA_7
#define ENB_T PA_6
#define EN_Z PA_5

#define INITSTEP -50
#endif

#ifdef STYLE_LF
//MotorA
Motor Wheel(PB_5, PA_10, PA_11_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_W PA_8
#define ENB_W PA_9
//MotorB
Motor Table(PB_4, PB_1_ALT0, PB_0_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_T PA_7
#define ENB_T PA_6
#define EN_Z PA_5

#define INITSTEP -280
#endif

#ifdef STYLE_LB
//MotorA
Motor Wheel(PB_5, PA_10, PA_11_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_W PA_8
#define ENB_W PA_9
//MotorB
Motor Table(PB_4, PB_1_ALT0, PB_0_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_T PA_7
#define ENB_T PA_6
#define EN_Z PA_5

#define INITSTEP -50
#endif

#ifdef STYLE_RB
//MotorA
Motor Wheel(PB_5, PA_10, PA_11_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_W PA_8
#define ENB_W PA_9
//MotorB
Motor Table(PB_4, PB_1_ALT0, PB_0_ALT0); //EN_GD, PWM_p, PWM_n
#define ENA_T PA_7
#define ENB_T PA_6
#define EN_Z PA_5

#define INITSTEP -50
#endif

#define MAX_WHEEL_SPEED 6000 //ジョイスティック入力での最高速度(°/s)
#define MAX_TABLE_SPEED 2000 //ジョイスティック入力での最高速度(°/s)
/*
float target_angle = 0; //目標角(エンコーダーの値)
float target_speed = 0; //目標角速度.1秒間に進む角
float table_speed = 0;
float wheel_speed = 0;
*/
//リセット時の暴走を防ぐため後で初期化する

//------------------------------------------------------------------------------------------------

//----------------------------------------Encoder---------------------------------------------------

//EncoderA == Wheel
InterruptIn enA_w(ENA_W);
DigitalIn enB_w(ENB_W);
//EncoderB == Table
InterruptIn enA_t(ENA_T);
DigitalIn enB_t(ENB_T);
InterruptIn enZ(EN_Z);

int en_count_t = 0; //現在の角度

float en_count_w = 0;     //現在の角度
float old_en_count_w = 0; //過去の角度(一定周期でタイマー割り込みがなされる)
float d_en_count_w = 0;   //角度の差分

#ifdef STYLE_LB
bool initflag = true;
#endif

#ifdef STYLE_RB
bool initflag = true;
#endif

#ifdef STYLE_RF
bool initflag = true; //Z相による初期化が済んでいるか否か
#endif

#ifdef STYLE_LF
bool initflag = true; //Z相による初期化が済んでいるか否か
#endif

bool ROSflag = false; //ROSのターマー割り込みを許可するか
//------------------------------------------------------------------------------------------------

//------------------------------------------PID-----------------------------------------------------
//table
float Kp_t = 1.0, Ki_t = 0, Kd_t = 0;
float diff_t[2] = {}; //差分を管理
//wheel
float Kp_w = 1.0, Ki_w = 0, Kd_w = 0;
float diff_w[2] = {};           //差分を管理
int Delta_T = SUMPLING_TIME_US; //周期

//変える
#define I_SET_t 10000
#define I_SET_w 10000
#define D_SET_t 10000
#define D_SET_w 3600
//--------------------------------------------------------------------------------------------------

float target_angle = 0; //目標角(エンコーダーの値)
float target_speed = 0; //目標角速度.1秒間に進む角
float table_speed = 0;
float wheel_speed = 0;

//プロトタイプ宣言
void init();
void init_motor();
void init_encoder();
void init_PID();
void encoder_t();
void encoder_w();
void init_wheel_pos_by_z(void);
void init_wheel_pos();
void move_table(float);
void move_wheel(float);
void Timer_interrupt();
void speed_calc_w(void);
void PID_t(void);
void PID_w(void);
void set_PID_t(float _Kp, float _Ki, float _Kd);
void set_PID_w(float _Kp, float _Ki, float _Kd);
void setter(float &value, float set);
float transform_gear_into_encoder(float a, int b);
float transform_encoder_into_gear(float a, int b);
void send_data();

void init()
{
  init_motor();
  init_encoder();
  init_PID(); //ここに置くと暴走しない
}

void init_motor()
{
  Wheel.setPWMPeriod(50);
  Wheel.setPWMPulsewidth(10);
  Wheel.enableGateDriver();

  Table.setPWMPeriod(50);
  Table.setPWMPulsewidth(10);
  Table.enableGateDriver();

  /*モーター回し方
  motorA.setSpeed(0.0);//-1~1のfloat
  wait_us(1000000);
  */
}

void init_encoder()
{
  enA_t.rise(encoder_t);         //エンコーダーの割り込み
  enA_w.rise(encoder_w);         //エンコーダーの割り込み
  enZ.rise(init_wheel_pos_by_z); //エンコーダーの割り込み テーブルのen_countの初期化
}

void init_PID()
{
  ticker.attach_us(&Timer_interrupt, SUMPLING_TIME_US); //タイマー割り込みの設定(speed_calc()を SUMPLING_TIME_US ごとに実行)

  target_angle = 0; //目標角
  target_speed = 0; //目標角速度.1秒間に進む角

  set_PID_t(40.0F, 30.0F*0, 0.1F); //比例定数の設定
  set_PID_w(0.5F, 0.5F, 0.010F); //比例定数の設定
}

int main(void)
{
  init();
  //ROSの送信設定
  Ms.float_attach(send_data);
  //SW1押されるまで待機
  while (1)
  {
    if (SW1.read() == 1)
    {
      //pc.printf("a\n");
      break;
    }
  }

  init_wheel_pos(); //z相で車輪の位置初期化
  wait_us(100000);

  //SW1押されるまで待機
  while (1)
  {
    if (SW1.read() == 1)
    {
      break;
    }
  }

  //ROSからの受付部分　testがうまく行ったら実行
  //ROSflag=true;とするとROSの割り込みが開始する
  ROSflag = true;
  while (1) //今は無限ループでやっているが実際はROSから送られてくる周期と同じ周期でタイマー割り込みするとか?
  {
    //move_table(Ms.getfloat[1]*180/M_PI); //ROSから目標角を受け付ける//radから°に変換
    //move_wheel(Ms.getfloat[0]); //ROSから目標角を受け付ける//値適当、後でやる
    if (!SW1)
      break; //回路についてあるデップスイッチ
  }
}

void encoder_t(void)
{
  if (enB_t == 1)
    en_count_t += 1;
  else
    en_count_t -= 1;
  //pc.printf("Angle_t:%d\n", en_count_t);
}

void encoder_w(void) //特性確認
{
  if (enB_w == 1)
    en_count_w += 7.5F; //7.5
  else
    en_count_w -= 7.5F; //7.5F
  //pc.printf("Angle_w:%d\n", (int)en_count_w);
}

void init_wheel_pos_by_z(void)
{
  if (!initflag)
  {
    en_count_t = INITSTEP; //現在の角度
    Table.setSpeed(0);
    //pc.printf("Z_demension was found\n");
    initflag = true;
  }
}

void init_wheel_pos()
{
  if (initflag)
    return;
  en_count_t = 0;
  Table.setSpeed(0.2F);
  wait_us(500000);
  if (initflag)
  {
    return;
  }
  Table.setSpeed(0);
  en_count_t = 0;
  wait_us(100000);
  Table.setSpeed(-0.2F);
  wait_us(1000000);
  if (initflag)
    return;
  Table.setSpeed(0);
  en_count_t = 0;
  return;
}

void Timer_interrupt()
{
  LED = 1;
  if (!initflag)
    ;
  else
  {
    speed_calc_w(); //車輪の速度計測
    PID_t();
    PID_w();
  }

  if (!SW1)
    return; //回路についてあるデップスイッチ

  //ROSはいったんオフ
  if (!ROSflag)
    ;
  else
  {
    if (!SW1)
      return;
    //ROSから目標角を受け付ける//radから°に変換
    move_table(Ms.getfloat[1] * 180 / 3.141592);
    move_wheel(Ms.getfloat[0] * 180 * 1000 / 35 / 3.141592);
  }
}

void move_table(float _target_angle)
{
  target_angle = _target_angle;
  //速度制御はPID_angleでやっています
}

void move_wheel(float _target_speed)
{
  target_speed = _target_speed;
  //速度制御はPID_speedでやっています
}

void speed_calc_w(void) //wheel
{
  d_en_count_w = en_count_w - old_en_count_w;
  old_en_count_w = en_count_w; //en_countは古くなったので保管しておく
  d_en_count_w *= 1000000 / SUMPLING_TIME_US;
  //pc.printf("Anglular Velocity(°/s):%d\n", (int)d_en_count_w);//d_en_countは現在値
}

void PID_t(void)
{
  static float v_t = 0;
  static float delta_v = 0;
  diff_t[0] = diff_t[1];
  diff_t[1] = -1 * (transform_gear_into_encoder(target_angle, 1) - en_count_t); //エンコーダーの増減の符号とモーターの回転向きを考慮し全体に-1を乗じている
  //diff[1]は目標角と現在の角度の差分
  float P = diff_t[1];
  static float I = 0;
  I += (diff_t[1] + diff_t[0]) / 2.0f * (SUMPLING_TIME_US / 1000000.0F);
  static float D = 0;
  D = (diff_t[1] - diff_t[0]) / (SUMPLING_TIME_US / 1000000.0F);
  float I_value = Ki_t * I;
  float D_value = Kd_t * D;
  setter(I_value, I_SET_t);
  setter(D_value, D_SET_t);
  delta_v = Kp_t * P + I_value + D_value;
  v_t = transform_encoder_into_gear(delta_v, 1); //エンコーダーの差分から回転テーブルの差分に変換する
  table_speed = v_t / MAX_TABLE_SPEED;           //-1~1のfloat値に変換(abs0.9以上 or abs0.02未満の場合はBrakeになるが)
  if (table_speed >= MAXDUTY_T)
  {
    table_speed = MAXDUTY_T;
    I = 0;/////
  }
  else if (table_speed <= -MAXDUTY_T)
  {
    table_speed = -MAXDUTY_T;
    I = 0;/////
  }
  Table.setSpeed(table_speed);
}

void PID_w(void)
{
  static float v_w = 0;
  static float delta_v = 0;
  diff_w[0] = diff_w[1];
  diff_w[1] = transform_gear_into_encoder(target_speed, 0) - d_en_count_w;
  //diff[1]は目標速度と現在の速度の差分
  float P = diff_w[1];
  static float I = 0;
  I += (diff_w[1] + diff_w[0]) / 2.0f * (SUMPLING_TIME_US / 1000000.0F);
  if (target_speed == 0)
    I = 0;
  static float D = 0;
  D = (diff_w[1] - diff_w[0]) / (SUMPLING_TIME_US / 1000000.0F);
  float I_value = Ki_w * I;
  float D_value = Kd_w * D;
  setter(I_value, I_SET_w);
  setter(D_value, D_SET_w);
  delta_v = Kp_w * P + I_value + D_value;
  v_w += transform_encoder_into_gear(delta_v, 0); //エンコーダーの差分から車輪の差分に変換し、足す
  setter(v_w, 0.88F * MAX_WHEEL_SPEED);
  wheel_speed = v_w / MAX_WHEEL_SPEED; //-1~1のfloat値に変換(abs0.9以上 or abs0.02未満の場合はBrakeになるが)
  // とりあえずエンコーダは読まないでduty比を入力
  //wheel_speed = target_speed * 0.1F;ここでPIDをあきらめてる
  if (wheel_speed >= MAXDUTY_W)
    wheel_speed = MAXDUTY_W;
  else if (wheel_speed <= -MAXDUTY_W)
    wheel_speed = -MAXDUTY_W;
  Wheel.setSpeed(wheel_speed);
  //pc.printf("%d\n", (int)(wheel_speed * 100)); //割る100するとwheel_speedが得られる
  //pc.printf("%f\n", wheel_speed);
}

void set_PID_t(float _Kp, float _Ki, float _Kd)
{
  Kp_t = _Kp;
  Ki_t = _Ki;
  Kd_t = _Kd;
  diff_t[0] = 0, diff_t[1] = 0;
}

void set_PID_w(float _Kp, float _Ki, float _Kd)
{
  Kp_w = _Kp;
  Ki_w = _Ki;
  Kd_w = _Kd;
  diff_w[0] = 0, diff_w[1] = 0;
}

void setter(float &value, float set)
{ //valueがset以上にならないようにする関数
  bool sign = true;
  if (value < 0)
    sign = false;
  if (abs(value) > set)
  {
    if (sign)
    {
      value = set;
    }
    else
    {
      value = -1 * set;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
//ギア変換///////////////////////////////////////////////////////////////
float transform_gear_into_encoder(float a, int b)
{ //b==0なら車輪の角速度をencoderの角速度に変換 b==1なら回転テーブルの角速度をencoderのに変換
  if (b == 0)
    return a * 60.0f / 23.0f; //ギア比
  else
    return a * 122.0f / 40.0f * 500.0f / 360.0f;
}

float transform_encoder_into_gear(float a, int b)
{
  if (b == 0)
    return a * 23.0f / 60.0f;
  else
    return a * 40.0f / 122.0f;
}

void send_data()
{
  //車輪の速度(m/s)、テーブル角度(rad)を送る
  //d_en_count_w、en_count_t
  float data[2] = {0};
  data[0] = transform_encoder_into_gear(d_en_count_w, 0) / (180.0f * 1000.0f / 35.0f / 3.141592f);
  data[1] = transform_encoder_into_gear(en_count_t, 1) *2.0f * 3.141592f / 500.0f;
  Ms.float_write(data, 2);
}
