#include <math.h>
#include <iostream>

static float vx = 0, vy = 0, w = 0, d = 150, theta_1 = 0, theta_2 = 0, theta_3 = 0, theta_4 = 0, theta = 0;//dはロボットから車輪までの距離(単位はm)
static float vx_1 = 0, vy_1 = 0, vx_2 = 0, vy_2 = 0, vx_3 = 0, vy_3 = 0, vx_4 = 0, vy_4 = 0;
static float v_1=0,v_2=0,v_3=0,v_4=0;

void wheel_control(float theta,float vx,float vy, float w)
{

  /*-------------必要な入力-----------
theta:ロボットが傾いている角度。BNOで計測?
vx:x方向のロボットの速度。ジョイスティックで入力
vy:y方向のロボットの速度。ジョイスティックで入力
ただしx,y軸はフィールドに固定されている(正確にはロボットの初期位置に固定されている)
w:ロボットの中心の角速度。ジョイコンで入力
  ------------------------------------*/

    //theta += w * dt;
	if (theta == 0 && vx==0 && vy==0);//0除算を防ぐ
	else {//４輪に渡す角度
		theta_1 = atan((vy + d * w * cos(theta)) / (vx - d * w * sin(theta)));
		theta_2 = atan((vy - d * w * sin(theta)) / (vx - d * w * cos(theta)));
		theta_3 = atan((vy - d * w * cos(theta)) / (vx + d * w * sin(theta)));
		theta_4 = atan((vy + d * w * sin(theta)) / (vx + d * w * cos(theta)));
	}
	vx_1 = vx - d * w * sin(theta);
	vy_1 = vy + d * w * cos(theta);
	vx_2 = vx - d * w * cos(theta);
	vy_2 = vy - d * w * sin(theta);
	vx_3 = vx + d * w * sin(theta);
	vy_3 = vy - d * w * cos(theta);
	vx_4 = vx + d * w * cos(theta);
	vy_4 = vy + d * w * sin(theta);

  float v_1_abs=sqrt(vx_1*vx_1+vy_1*vy_1);
  float v_2_abs=sqrt(vx_2*vx_2+vy_2*vy_2);
  float v_3_abs=sqrt(vx_3*vx_3+vy_3*vy_3);
  float v_4_abs=sqrt(vx_4*vx_4+vy_4*vy_4);

  if(vx_1>=0){
    v_1=v_1_abs;
  }
  else{
    v_1=-1*v_1_abs;
  }

  if(vx_2>=0){
    v_2=v_2_abs;
  }
  else{
    v_2=-1*v_2_abs;
  }

  if(vx_3>=0){
    v_3=v_3_abs;
  }
  else{
    v_3=-1*v_3_abs;
  }

  if(vx_4>=0){
    v_4=v_4_abs;
  }
  else{
    v_4=-1*v_4_abs;
  }

  //モーター角速度に変換
  float target_speed_1 = v_1 * 180*1000/35/M_PI;
  float target_speed_2 = v_2 * 180*1000/35/M_PI;
  float target_speed_3 = v_3 * 180*1000/35/M_PI;
  float target_speed_4 = v_4 * 180*1000/35/M_PI;

  /*
  v_1=sqrt(vx*vx+vy*vy-2*d*w*(vx*sin(theta)-vy*cos(theta))+d*d*w*w);//sqrt(vx_1^2+vy_1^2)
  v_2=sqrt(vx*vx+vy*vy-2*d*w*(vx*cos(theta)+vy*sin(theta))+d*d*w*w);
  v_3=sqrt(vx*vx+vy*vy-2*d*w*(-1*vx*sin(theta)+vy*cos(theta))+d*d*w*w);
  v_4=sqrt(vx*vx+vy*vy-2*d*w*(-1*vx*cos(theta)-vy*sin(theta))+d*d*w*w);
  */
  /*-------------出力-----------
  theta_1
  theta_2
  theta_3
  theta_4
  4輪のx軸からみた反時計回りの角度

  target_speed_1
  target_speed_2
  target_speed_3
  target_speed_4
  車輪の速度(モーターの回転速度に変換している)
  ------------------------------------*/
}
