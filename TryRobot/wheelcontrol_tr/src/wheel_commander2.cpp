#include <math.h>

#define BODY_WIDTH 440.0F // [mm]

#define PI 3.141592F

float theta_1 = 0, theta_2 = 0, theta_3 = 0, theta_4 = 0;
float theta_st[4] = {0}; // table angle (fixed coordinate)
float target_speed_1 = 0, target_speed_2 = 0, target_speed_3 = 0, target_speed_4 = 0;

void wheel_control(float theta_body, float vx, float vy, float w)
{
  // wheel position now
  float wheelpos[4][2] = {0}; // [RF, LF, LB, RB][x, y]
  wheelpos[0][0] = BODY_WIDTH / sqrt(2.0f) * cos(theta_body + PI / 4.0f);
  wheelpos[0][1] = BODY_WIDTH / sqrt(2.0f) * sin(theta_body + PI / 4.0f);
  wheelpos[1][0] = -wheelpos[0][1];
  wheelpos[1][1] = wheelpos[0][0];
  wheelpos[2][0] = -wheelpos[0][0];
  wheelpos[2][1] = -wheelpos[0][1];
  wheelpos[3][0] = -wheelpos[1][0];
  wheelpos[3][1] = -wheelpos[1][1];

  // wheel position ref
  float wheelpos2[4][2] = {0};
  wheelpos2[0][0] = BODY_WIDTH / sqrt(2.0f) * cos(theta_body + w + PI / 4.0f);
  wheelpos2[0][1] = BODY_WIDTH / sqrt(2.0f) * sin(theta_body + w + PI / 4.0f);
  wheelpos2[1][0] = -wheelpos2[0][1];
  wheelpos2[1][1] = wheelpos2[0][0];
  wheelpos2[2][0] = -wheelpos2[0][0];
  wheelpos2[2][1] = -wheelpos2[0][1];
  wheelpos2[3][0] = -wheelpos2[1][0];
  wheelpos2[3][1] = -wheelpos2[1][1];
  for (int i = 0; i < 4; i++)
  {
    wheelpos2[i][0] += vx;
    wheelpos2[i][1] += vy;
  }

  // direction
  float deltapos[4][2] = {0};
  for (int i = 0; i < 4; i++)
  {
    deltapos[i][0] = wheelpos2[i][0] - wheelpos[i][0];
    deltapos[i][1] = wheelpos2[i][1] - wheelpos[i][1];
  }

  static float theta_temp[4] = {0}; // -pi < theta_temp < pi
  for (int i = 0; i < 4; i++)
  {
    if (deltapos[i][1] > 1e-7f)
    {
      theta_temp[i] = atan(-deltapos[i][0] / deltapos[i][1]);
    }
    else if (deltapos[i][1] < -1e-7)
    {
      theta_temp[i] = atan(-deltapos[i][0] / deltapos[i][1]);
      if (deltapos[i][0] > 1e-7f)
        theta_temp[i] -= PI;
      else if (deltapos[i][0] < -1e-7f)
        theta_temp[i] += PI;
      else
        theta_temp[i] = PI;
    }
    else
    {
      if (deltapos[i][0] > 1e-7f)
        theta_temp[i] = -PI / 2.0f;
      else if (deltapos[i][0] < -1e-7f)
        theta_temp[i] = PI / 2.0f;
    }
  }

  // calc theta (move)
  static int n_pi[4] = {0};
  float theta_st_temp[4] = {0.0}; // move angle
  for (int i = 0; i < 4; i++)
  {
    if (theta_st[i] > theta_temp[i] + PI * (float)(n_pi[i] + 1))
      n_pi[i] += 2;
    else if (theta_st[i] < theta_temp[i] + PI * (float)(n_pi[i] - 1))
      n_pi[i] -= 2;
    theta_st_temp[i] = theta_temp[i] + PI * (float)n_pi[i];
  }

  // calc speed and theta(wheel)
  float target_speed_temp[4] = {0};
  for (int i = 0; i < 4; i++)
  {
    target_speed_temp[i] = sqrt(deltapos[i][0] * deltapos[i][0] + deltapos[i][1] * deltapos[i][1]);
    if (theta_st_temp[i] > theta_st[i] + PI / 2.0f && theta_st_temp[i] < theta_st[i] + PI)
    {
      theta_st[i] = theta_st_temp[i];
      theta_st_temp[i] += PI;
      target_speed_temp[i] *= -1.0f;
    }
    else if (theta_st_temp[i] < theta_st[i] - PI / 2.0f && theta_st_temp[i] > theta_st[i] - PI)
    {
      theta_st[i] = theta_st_temp[i];
      theta_st_temp[i] -= PI;
      target_speed_temp[i] *= -1.0f;
    }
    else
    {
      theta_st[i] = theta_st_temp[i];
    }
  }
  
  theta_1 = theta_st_temp[0] - theta_body;
  theta_2 = theta_st_temp[1] - theta_body;
  theta_3 = theta_st_temp[2] - theta_body;
  theta_4 = theta_st_temp[3] - theta_body;
  target_speed_1 = target_speed_temp[0];
  target_speed_2 = target_speed_temp[1];
  target_speed_3 = target_speed_temp[2];
  target_speed_4 = target_speed_temp[3];
}
