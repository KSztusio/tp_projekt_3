#include "planar_quadrotor_visualizer.h"

PlanarQuadrotorVisualizer::PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr) : quadrotor_ptr(quadrotor_ptr) {}

/**
 * TODO: Improve visualizetion
 * 1. Transform coordinates from quadrotor frame to image frame so the circle is in the middle of the screen
 * 2. Use more shapes to represent quadrotor (e.x. try replicate http://underactuated.mit.edu/acrobot.html#section3 or do something prettier)
 * 3. Animate proppelers (extra points)
 */

void drawRectangle(std::shared_ptr<SDL_Renderer>& renderer, double r_theta, float r_x, float r_y, float l, float h, Uint32 color, int n, bool line)
{
    Sint16 vx[4];
    Sint16 vy[4];

    float xr_1, xr_2, yr_1, yr_2;

    xr_1 = r_x + cos(r_theta) * (l / 2);
    xr_2 = r_x - cos(r_theta) * (l / 2);
    yr_1 = r_y - sin(r_theta) * (l / 2);
    yr_2 = r_y + sin(r_theta) * (l / 2);
    vx[0] = xr_1 - sin(r_theta) * (h / 2);
    vx[1] = xr_1 + sin(r_theta) * (h / 2);
    vx[2] = xr_2 + sin(r_theta) * (h / 2) * n;
    vx[3] = xr_2 - sin(r_theta) * (h / 2) * n;
    vy[0] = yr_1 - cos(r_theta) * (h / 2);
    vy[1] = yr_1 + cos(r_theta) * (h / 2);
    vy[2] = yr_2 + cos(r_theta) * (h / 2) * n;
    vy[3] = yr_2 - cos(r_theta) * (h / 2) * n;

    // Draw the rectangle body
    filledPolygonColor(renderer.get(), vx, vy, 4, color);
  if(line){
    // Calculate propeller line coordinates
    float propeller_x1 = (vx[0] + vx[1]) / 2;
    float propeller_y1 = (vy[0] + vy[1]) / 2;
    float propeller_x2 = (vx[2] + vx[3]) / 2;
    float propeller_y2 = (vy[2] + vy[3]) / 2;

    // Draw propeller line
    thickLineColor(renderer.get(), propeller_x1, propeller_y1, propeller_x2, propeller_y2, 1, 0xFF000000);
    
    // Draw the black border
    aapolygonColor(renderer.get(), vx, vy, 4, 0xFF000000);
}
}



void PlanarQuadrotorVisualizer::render(std::shared_ptr<SDL_Renderer> &gRenderer, float t)
{
    Eigen::VectorXf state = quadrotor_ptr->GetState();
    float q_x, q_y, q_theta, x1, x2, y1, y2, xl, xr, yl, yr, tx_l, tx_r, ty_l, ty_r;
    const int l = 150, h = 10, r = 15, k = 40;
    /* x, y, theta coordinates */
    q_x = state[0];
    q_y = state[1];
    q_theta = state[2];

    x1 = q_x + cos(q_theta) * (l / 2);
    x2 = q_x - cos(q_theta) * (l / 2);
    y1 = q_y - sin(q_theta) * (l / 2);
    y2 = q_y + sin(q_theta) * (l / 2);
    xr = x1 - sin(q_theta) * k / 2;
    xl = x2 - sin(q_theta) * k / 2;
    yr = y1 - cos(q_theta) * k / 2;
    yl = y2 - cos(q_theta) * k / 2;

    tx_r = x1 - sin(q_theta) * k;
    tx_l = x2 - sin(q_theta) * k;
    ty_r = y1 - cos(q_theta) * k;
    ty_l = y2 - cos(q_theta) * k;

    drawRectangle(gRenderer, q_theta, q_x, q_y, l, h, 0xFF722159, 1, false);

    // prostakaty boczne
    drawRectangle(gRenderer, q_theta, xr, yr, 2, 40, 0xFF000000, 1, false);
    drawRectangle(gRenderer, q_theta, xl, yl, 2, 40, 0xFF000000, 1, false);

    // smigla
    drawRectangle(gRenderer, q_theta, tx_l, ty_l, 40 * sin(t), 10, 0xFFE189C1, -1, true);
    drawRectangle(gRenderer, q_theta, tx_r, ty_r, 40 * sin(t), 10, 0xFFE189C1, -1, true);
}