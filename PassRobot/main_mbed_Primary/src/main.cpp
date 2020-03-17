#include <mbed.h>
#include <math.h>
#include "motordriver.h"
#include "PID.h"
#include "controller.h"
#include "main.h"
#include "mbedserial.h"

#include "RDS5160_Servo.h"
#include "PickUp.h"

double pi = 3.14159265358979;

Serial pc(USBTX, USBRX, 115200);
InterruptIn switch1(USER_BUTTON); //青ボタン
CAN can1(PA_11, PA_12, 500000);
Mbedserial Ms(pc);
Controller controller(can1, 0x334);

//タイマー割り込み周期 kHzオーダーつまり1000が理想
#define SUMPLING_TIME_US 100000

//モーターのMAX値
#define PERIOD 1000

//モーター
PwmOut PWM_FR(PA_8);
PwmOut PWM_FL(PA_0);
PwmOut PWM_RR(PA_6);
PwmOut PWM_RL(PB_6);
DigitalOut PHASE_FR(PC_11);
DigitalOut PHASE_FL(PD_2);
DigitalOut PHASE_RR(PC_9);
DigitalOut PHASE_RL(PB_9);
Motor motor_a(PWM_FR, PHASE_FR, PERIOD, true);
Motor motor_b(PWM_FL, PHASE_FL, PERIOD, true);
Motor motor_c(PWM_RR, PHASE_RR, PERIOD, true);
Motor motor_d(PWM_RL, PHASE_RL, PERIOD, true);

////ピックアップ
PwmOut ServoLout(PB_5);
PwmOut ServoRout(PB_4);

RDS5160Servo ServoL(ServoLout, 1370, true);
RDS5160Servo ServoR(ServoRout, 1620, false);

PickUp pickup(ServoL, ServoR);

DigitalOut PickUpClose(PC_4);
DigitalOut pass_down(PB_12);
DigitalOut pass_up(PB_14);
////

Ticker sumpling;
//InterruptIn switch1(USER_BUTTON);

double theta, r;

//この速度で回れ
double v[2]={250, 250}; // これから出てくる[2]は全て[0]がmotor_a, [1]がmotor_bに対応している
double target_speed[2];

//現在の角度
/*int en_count[2] = {0, 0};

//過去の角度(一定周期でタイマー割り込みがなされる)
int old_en_count[2] = {0, 0};

//角度の差分
int d_en_count[2] = {0, 0};*/

//PID
double Kp = 1.0, Ki = 0, Kd = 0;
double diff[4];
static double integral[2];
double delta_v=0;

//エンコーダ
/*
InterruptIn enA_a(PA_9);
DigitalIn enB_a(PB_4);

InterruptIn enA_b(PA_9);
DigitalIn enB_b(PB_4);
*/

//角速度を計測する
/*void speed_calc(void) {
  //pc.printf("encount;%d  oldencount;%d\n", en_count,old_en_count);//d_en_countは現在値
  d_en_count[0] = en_count[0] - old_en_count[0];
  old_en_count[0] = en_count[0]; //en_countは古くなったので保管しておく
  d_en_count[0] *= 1000000 / SUMPLING_TIME_US;
  pc.printf("Anglular Velocity(°/s):%d\n", d_en_count[0]);//d_en_countは現在値

  //pc.printf("encount;%d  oldencount;%d\n", en_count,old_en_count);//d_en_countは現在値
  d_en_count[1] = en_count[1] - old_en_count[1];
  old_en_count[1] = en_count[1]; //en_countは古くなったので保管しておく
  d_en_count[1] *= 1000000 / SUMPLING_TIME_US;
  pc.printf("Anglular Velocity(°/s):%d\n", d_en_count[1]);//d_en_countは現在値
}

//エンコーダ
void encoder(void) {
  if(enB_a == 1) en_count[0] += 1;
  else en_count[0] -= 1;

  if(enB_b == 1) en_count[1] += 1;
  else en_count[1] -= 1;
  //pc.printf("Angle:%d\n", en_count);
}

void limit(double &a){//セッター
  if(a>=PERIOD*0.8) a=PERIOD*0.8;
  else if(a<=-1*PERIOD*0.8) a=-1*PERIOD*0.8;
}

//PID
void PID(void) {
  double p[2], i[2], d[2];
  diff[0] = diff[1];
  diff[1] = target_speed[0] - (double)d_en_count[0];
  //diff[1]は目標速度と現在の速度の差分
  integral[0] += (diff[1] + diff[0]) / 2.0 * (SUMPLING_TIME_US/(double)1000000);
  p[0] = Kp * diff[1];
  i[0] = Ki * integral[0];
  d[0] = (diff[0] - diff[1]) / (SUMPLING_TIME_US/(double)1000000);
  v[0] += p[0] + i[0] + d[0];//エンコーダーの差分から車輪の差分に変換し、足す
  limit(v[0]);

  diff[2] = diff[3];
  diff[3] = target_speed[1] - (double)d_en_count[1];
  //diff[1]は目標速度と現在の速度の差分
  integral[1] += (diff[3] + diff[2]) / 2.0 * (SUMPLING_TIME_US/(double)1000000);
  p[1] = Kp * diff[3];
  i[1] = Ki * integral[1];
  d[1] = (diff[2] - diff[3]) / (SUMPLING_TIME_US/(double)1000000);
  v[1] += p[1] + i[1] + d[1];//エンコーダーの差分から車輪の差分に変換し、足す
  limit(v[1]);
  //vの値が増え(減り)すぎないようにセッターを設ける
  //pc.printf("%d\n",d_en_count);
}

void target_speed_calc(void) {
  r = sqrt(pow(controller.axes.x, 2) + pow(controller.axes.y, 2));
  theta = atan2(controller.axes.y, controller.axes.x);

  if(-pi / 8.0 < theta && theta <= pi / 8.0) {
    target_speed[0] = -PERIOD * r / 100.0;
    target_speed[1] = PERIOD * r / 100.0;
  }
  else if(pi / 8.0 < theta && theta <= pi * 3.0 / 8.0) {
    target_speed[0] = 0;
    target_speed[1] = PERIOD * r / 100.0 / sqrt(2.0);
  }
  else if(pi * 3.0 / 8.0 < theta && theta <= pi * 5.0 / 8.0) {
    target_speed[0] = PERIOD * r / 100.0;
    target_speed[1] = PERIOD * r / 100.0;
  }
  else if(pi * 5.0 / 8.0 < theta && theta <= pi * 7.0 / 8.0) {
    target_speed[0] = PERIOD * r / 100.0 / sqrt(2.0);
    target_speed[1] = 0;
  }
  else if(pi * 7.0 / 8.0 < theta || theta <= -pi * 7.0 / 8.0) {
    target_speed[0] = PERIOD * r / 100.0;
    target_speed[1] = -PERIOD * r / 100.0;
  }
  else if(-pi * 7.0 / 8.0 < theta && theta <= -pi * 5.0 / 8.0) {
    target_speed[0] = 0;
    target_speed[1] = -PERIOD * r / 100.0 / sqrt(2.0);
  }
  else if(-pi * 5.0 / 8.0 < theta && theta <= -pi * 3.0 / 8.0) {
    target_speed[0] = -PERIOD * r / 100.0;
    target_speed[1] = -PERIOD * r / 100.0;
  }
  else if(-pi * 3.0 / 8.0 < theta && theta <= -pi / 8.0) {
    target_speed[0] = -PERIOD * r / 100.0 / sqrt(2.0);
    target_speed[1] = 0;
  }
  else {
    pc.printf("error\nerror\nerror\nerror\nerror\nerror\nerror\nerror\nerror\nerror\nerror\nerror\nerror\n");
  }
}*/

void PickUpFlow() {

  /*
  while(1) {
    if(!UB.read()) {
      pickup.Horizontal();
    }
    else{
      pickup.Vertical();
    }
  }*/

  pickup.Vertical();
  wait_ms(1000);

  PickUpClose.write(0);
  wait_ms(500);

  pickup.Horizontal();
  wait_ms(2000);

  PickUpClose.write(1);
  wait_ms(1000);

  for (float i = 0.0f; i <= 90.0f; i += 1)
  {
    pickup.MoveTo(i);
    wait_ms(20);
  }
  /*
  pickup.MoveTo(30.0f);
  wait_ms(1000);
  //pickup.MoveTo(60.0f);
  wait_ms(1000);
  pickup.Vertical();*/
  //wait_ms(500);

  PickUpClose.write(0);
  /*
  while(1) {
    if(!UB.read()) {
      PickUpClose.write(1);
    }
    else {
      PickUpClose.write(0);
    }
  }*/
}

void test() {
  while(1) {
  pickup.Vertical();
  wait_ms(1000);
  pickup.Horizontal();
  wait_ms(1000);
  }
}

int main() {
  /*
  enA_a.rise(encoder);
  enA_b.rise(encoder);
  sumpling.attach_us(&speed_calc, SUMPLING_TIME_US);
  sumpling.attach_us(PID, SUMPLING_TIME_US);
  sumpling.attach_us(target_speed_calc, SUMPLING_TIME_US);
  */

  while(switch1);
    //pc.printf("%d", 1);
    wait(1.0);

    /*
    motor_a.speed(400);
    motor_b.speed(400);
    motor_c.speed(400);
    motor_.speed(-400);
    wait_us(SUMPLING_TIME_US);
    
    wait(1.15);
    motor_a.speed(0);
    motor_b.speed(0);
    motor_c.speed(0);
    motor_d.speed(0);
    wait(0.5);

    motor_a.speed(-400);
    motor_b.speed(400);
    motor_c.speed(400);
    motor_d.speed(400);
    wait(1.2);

    motor_a.speed(0);
    motor_b.speed(0);
    motor_c.speed(0);
    motor_d.speed(0);
    */

    PickUpFlow();
    wait_us(3000000);

    pass_down.write(1);
    wait_us(1000000);
    pass_down.write(0);
    //test();
}