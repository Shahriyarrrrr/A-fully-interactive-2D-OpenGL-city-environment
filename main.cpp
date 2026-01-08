#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <ctime>

void drawCircleMidpoint(float cx, float cy, float r);
void drawLineDDA(float x1, float y1, float x2, float y2);
void drawWindmill();
bool planeOn = false; // airplane
bool showPlane = true; // airplane
float planeX = 0.003f; // airplane
float planeY = 0.65f; // airplane
float planeSpeed = 0.002f; // airplane
bool isRaining = false; // rain

const int RAIN_COUNT = 200;
float rainX[RAIN_COUNT];
float rainY[RAIN_COUNT];
float windmillAngle = 0.0f;

bool snowIsFalling = false;
const int howManySnow =140 ;
float snowX[140];
float snowY[140];
bool isThunder = false;
int thunderTimer = 0;
float humanX = -1.2f; // FOR HUMAN WITH UMBRELLA
bool rainMode = false; // FOR HUMAN WITH UMBRELLA
bool umbrellaOn = false; // FOR HUMAN WITH UMBRELLA


float car1X = -0.8f;  // Red car - bottom lane
float car2X = 0.3f;   // Blue car - top lane
float boat1X = -1.0f;
float boat2X = 0.5f;
float cloud1X = -0.75f;
float cloud2X = -0.05f;
float cloud3X = 0.55f;
float bird1X = 0.30f; // Bird 1 position
float bird2X = 0.48f; // Bird 2 position
float bird3X = 0.62f; // Bird 3 position

// Control variables
bool isNightMode = false;
bool animationPaused = false;
float car1Speed = 0.01f;
float car2Speed = -0.012f;

void drawThunder()
{
    if (!isThunder) return;

    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_STRIP);

        glVertex2f(cloud1X + 0.1f, 0.78f);
        glVertex2f(cloud1X + 0.05f, 0.5f);
        glVertex2f(cloud1X + 0.15f, 0.3f);
        glVertex2f(cloud1X + 0.0f, 0.0f);
        glVertex2f(cloud1X + 0.1f, -0.3f);
    glEnd();

    glLineWidth(1.0f);
}

void makeSnow() {
    srand(time(NULL));
    for(int i = 0; i < howManySnow; i++) {
        snowX[i] = -1.0f + (rand() % 2000) / 1000.0f;
        snowY[i] = 0.6f + (rand() % 1400) / 1000.0f;
    }
}

void snowDraw()
{
    if(!snowIsFalling) return;

    glColor3f(1,1,1);
    glPointSize(2.8);

    glBegin(GL_POINTS);
    for(int i = 0; i < howManySnow; i++)
        {
        glVertex2f(snowX[i], snowY[i]);
    }
    glEnd();
}

//snow
void snowMove()
{
    if(!snowIsFalling) return;

    for(int i = 0; i < howManySnow; i++)
        {
        snowY[i] -= 0.007;

        if(snowY[i] < -1.1) {
            snowY[i] = 1.3;

        }
    }
}



void initRain()
{
    for (int i = 0; i < RAIN_COUNT; i++)
    {
        rainX[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        rainY[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    }
}
void drawRain()
{
    if (!isRaining) return;

    glColor3f(0.7f, 0.7f, 1.0f); // light blue rain
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    for (int i = 0; i < RAIN_COUNT; i++)
    {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i], rainY[i] - 0.05f);
    }
    glEnd();
}

// FOR CLOUDS
void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * 3.1416f * i / 100;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void drawRiver()
{
    // River water
    if(isNightMode)
        {
        glColor3f(0.1f, 0.2f, 0.4f);  // Dark blue for night
    } else {
        glColor3f(0.2f, 0.4f, 0.8f);  // Bright blue for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.95f);
        glVertex2f(1.0f, -0.95f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

    // River wave lines
    if(isNightMode)
        {
        glColor3f(0.2f, 0.3f, 0.6f);  // Darker for night
    } else
    {
        glColor3f(0.3f, 0.5f, 0.9f);  // Lighter for day
    }

    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for(float x = -1.0f; x <= 1.0f; x += 0.2f)
    {
        glVertex2f(x, -0.96f);
        glVertex2f(x + 0.1f, -0.98f);
    }
    glEnd();
}

void drawBezierCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_LINE_STRIP);
    for(float t = 0; t <= 1.0f; t += 0.01f)
    {
        float t2 = t * t;
        float t3 = t2 * t;
        float mt = 1 - t;
        float mt2 = mt * mt;
        float mt3 = mt2 * mt;

        float x = mt3*x0 + 3*mt2*t*x1 + 3*mt*t2*x2 + t3*x3;
        float y = mt3*y0 + 3*mt2*t*y1 + 3*mt*t2*y2 + t3*y3;

        glVertex2f(x, y);
    }
    glEnd();
}



void drawBridge()
{
    // Bridge are made of Bezier curve
    glColor3f(0.5f, 0.3f, 0.1f);
    glLineWidth(4.0f);
    drawBezierCurve(-0.4f, -0.92f, -0.2f, -0.88f, 0.2f, -0.88f, 0.4f, -0.92f);
    glLineWidth(1.0f);

    // Bridge supports
    glColor3f(0.4f, 0.2f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(-0.35f, -0.92f);
        glVertex2f(-0.33f, -0.92f);
        glVertex2f(-0.33f, -0.95f);
        glVertex2f(-0.35f, -0.95f);

        glVertex2f(0.33f, -0.92f);
        glVertex2f(0.35f, -0.92f);
        glVertex2f(0.35f, -0.95f);
        glVertex2f(0.33f, -0.95f);
    glEnd();
}
void drawBoats()
{
    // Boat 1
    glColor3f(0.8f, 0.6f, 0.2f); // Boat body
    glBegin(GL_QUADS);
        glVertex2f(boat1X - 0.15f, -0.97f);
        glVertex2f(boat1X + 0.15f, -0.97f);
        glVertex2f(boat1X + 0.1f, -0.93f);
        glVertex2f(boat1X - 0.1f, -0.93f);
    glEnd();

    glColor3f(0.5f, 0.3f, 0.1f); // Boat stripe
    glBegin(GL_QUADS);
        glVertex2f(boat1X - 0.12f, -0.95f);
        glVertex2f(boat1X + 0.12f, -0.95f);
        glVertex2f(boat1X + 0.12f, -0.94f);
        glVertex2f(boat1X - 0.12f, -0.94f);
    glEnd();

    glColor3f(0.3f, 0.2f, 0.1f); // Mast
    glBegin(GL_QUADS);
        glVertex2f(boat1X - 0.01f, -0.93f);
        glVertex2f(boat1X + 0.01f, -0.93f);
        glVertex2f(boat1X + 0.01f, -0.87f);
        glVertex2f(boat1X - 0.01f, -0.87f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.7f); // Sail
    glBegin(GL_TRIANGLES);
        glVertex2f(boat1X, -0.87f);
        glVertex2f(boat1X - 0.1f, -0.90f);
        glVertex2f(boat1X, -0.93f);
    glEnd();

    // Boat 2
    glColor3f(0.6f, 0.2f, 0.2f); // Boat body
    glBegin(GL_QUADS);
        glVertex2f(boat2X - 0.12f, -0.97f);
        glVertex2f(boat2X + 0.12f, -0.97f);
        glVertex2f(boat2X + 0.08f, -0.94f);
        glVertex2f(boat2X - 0.08f, -0.94f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.9f); // Cabin
    glBegin(GL_QUADS);
        glVertex2f(boat2X - 0.05f, -0.94f);
        glVertex2f(boat2X + 0.05f, -0.94f);
        glVertex2f(boat2X + 0.05f, -0.90f);
        glVertex2f(boat2X - 0.05f, -0.90f);
    glEnd();

    // Cabin windows - light on at night
    if(isNightMode)
    {
        glColor3f(1.0f, 0.9f, 0.5f); // Yellow light for night
    }
    else
    {
        glColor3f(0.1f, 0.1f, 0.1f); // Dark for day
    }

    glBegin(GL_QUADS);
        glVertex2f(boat2X - 0.03f, -0.92f);
        glVertex2f(boat2X - 0.01f, -0.92f);
        glVertex2f(boat2X - 0.01f, -0.91f);
        glVertex2f(boat2X - 0.03f, -0.91f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(boat2X + 0.01f, -0.92f);
        glVertex2f(boat2X + 0.03f, -0.92f);
        glVertex2f(boat2X + 0.03f, -0.91f);
        glVertex2f(boat2X + 0.01f, -0.91f);
    glEnd();
}

void drawBank()
{
    // River bank (sand/grass area)
    if(isNightMode)
        {
        glColor3f(0.6f, 0.5f, 0.3f); // Darker sand for night
    } else {
        glColor3f(0.8f, 0.7f, 0.5f); // Sandy color for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.9f);
        glVertex2f(1.0f, -0.9f);
        glVertex2f(1.0f, -0.95f);
        glVertex2f(-1.0f, -0.95f);
    glEnd();

    // bushes
    if(isNightMode)
        {
        glColor3f(0.1f, 0.3f, 0.1f); // Dark green for night
    }
    else
    {
        glColor3f(0.3f, 0.6f, 0.2f); // Bright green for day
    }

    drawCircle(-0.8f, -0.91f, 0.03f);
    drawCircle(-0.4f, -0.91f, 0.02f);
    drawCircle(0.0f, -0.91f, 0.025f);
    drawCircle(0.4f, -0.91f, 0.03f);
    drawCircle(0.8f, -0.91f, 0.02f);
}

void drawMovingCar1()
{
  // car 1
    // Car body
    glColor3f(0.85f, 0.15f, 0.15f);
    glBegin(GL_QUADS);
        glVertex2f(car1X - 0.1f, -0.74f);
        glVertex2f(car1X + 0.1f, -0.74f);
        glVertex2f(car1X + 0.1f, -0.68f);
        glVertex2f(car1X - 0.1f, -0.68f);
    glEnd();

    // Car top
    glColor3f(0.95f, 0.75f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(car1X - 0.06f, -0.68f);
        glVertex2f(car1X + 0.02f, -0.68f);
        glVertex2f(car1X + 0.0f, -0.64f);
        glVertex2f(car1X - 0.04f, -0.64f);
    glEnd();

    // Wheels
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex2f(car1X - 0.08f, -0.74f);
        glVertex2f(car1X - 0.04f, -0.74f);
        glVertex2f(car1X - 0.04f, -0.77f);
        glVertex2f(car1X - 0.08f, -0.77f);

        glVertex2f(car1X + 0.04f, -0.74f);
        glVertex2f(car1X + 0.08f, -0.74f);
        glVertex2f(car1X + 0.08f, -0.77f);
        glVertex2f(car1X + 0.04f, -0.77f);
    glEnd();
}

void drawMovingCar2()
{
    // Car 2
    // Car body
    glColor3f(0.20f, 0.30f, 0.80f);
    glBegin(GL_QUADS);
        glVertex2f(car2X - 0.09f, -0.66f);
        glVertex2f(car2X + 0.09f, -0.66f);
        glVertex2f(car2X + 0.09f, -0.60f);
        glVertex2f(car2X - 0.09f, -0.60f);
    glEnd();

    // Car top
    glColor3f(0.70f, 0.80f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(car2X - 0.07f, -0.60f);
        glVertex2f(car2X + 0.03f, -0.60f);
        glVertex2f(car2X + 0.01f, -0.56f);
        glVertex2f(car2X - 0.05f, -0.56f);
    glEnd();

    // Wheels
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex2f(car2X - 0.07f, -0.66f);
        glVertex2f(car2X - 0.03f, -0.66f);
        glVertex2f(car2X - 0.03f, -0.69f);
        glVertex2f(car2X - 0.07f, -0.69f);

        glVertex2f(car2X + 0.03f, -0.66f);
        glVertex2f(car2X + 0.07f, -0.66f);
        glVertex2f(car2X + 0.07f, -0.69f);
        glVertex2f(car2X + 0.03f, -0.69f);
    glEnd();
}

void drawMoon()
{
    // Draw moon for night mode
    glColor3f(0.95f, 0.95f, 0.8f);
    drawCircle(0.8f, 0.8f, 0.08f);

    // Moon craters
    glColor3f(0.85f, 0.85f, 0.7f);
    drawCircle(0.78f, 0.82f, 0.015f);
    drawCircle(0.83f, 0.78f, 0.02f);
    drawCircle(0.76f, 0.76f, 0.012f);
}

void drawSun()
{
    // Draw sun for day mode
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.8f, 0.8f, 0.08f);

    // Sun rays
    glColor3f(1.0f, 0.9f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for(int i = 0; i < 12; i++)
    {
        float angle = 2.0f * 3.1416f * i / 12;
        glVertex2f(0.8f + 0.1f * cos(angle), 0.8f + 0.1f * sin(angle));
        glVertex2f(0.8f + 0.15f * cos(angle), 0.8f + 0.15f * sin(angle));
    }
    glEnd();
    glLineWidth(1.0f);
}

void drawBirds()
{
    // flying birds
    //srs
    if(!isNightMode)
        {
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(4.0f);

        // Bird 1
        glBegin(GL_LINES);
        glVertex2f(bird1X, 0.75f);
        glVertex2f(bird1X + 0.03f, 0.78f);
        glVertex2f(bird1X + 0.03f, 0.78f);
        glVertex2f(bird1X + 0.06f, 0.76f);
        glVertex2f(bird1X + 0.06f, 0.76f);
        glVertex2f(bird1X + 0.09f, 0.78f);
        glVertex2f(bird1X + 0.09f, 0.78f);
        glVertex2f(bird1X + 0.12f, 0.75f);
        glEnd();

        // Bird 2
        glBegin(GL_LINES);
        glVertex2f(bird2X, 0.72f);
        glVertex2f(bird2X + 0.03f, 0.75f);
        glVertex2f(bird2X + 0.03f, 0.75f);
        glVertex2f(bird2X + 0.06f, 0.73f);
        glVertex2f(bird2X + 0.06f, 0.73f);
        glVertex2f(bird2X + 0.09f, 0.75f);
        glVertex2f(bird2X + 0.09f, 0.75f);
        glVertex2f(bird2X + 0.12f, 0.72f);
        glEnd();

        // Bird 3
        glBegin(GL_LINES);
        glVertex2f(bird3X, 0.78f);
        glVertex2f(bird3X + 0.03f, 0.81f);
        glVertex2f(bird3X + 0.03f, 0.81f);
        glVertex2f(bird3X + 0.06f, 0.79f);
        glVertex2f(bird3X + 0.06f, 0.79f);
        glVertex2f(bird3X + 0.09f, 0.81f);
        glVertex2f(bird3X + 0.09f, 0.81f);
        glVertex2f(bird3X + 0.12f, 0.78f);
        glEnd();

        glLineWidth(2.0f);
    }
}

//srs
// windmill
void update()
{
    windmillAngle += 0.5f;

    if (windmillAngle >= 360.0f)
        windmillAngle = 0.0f;

    glutPostRedisplay();
}


void walkHuman(int)
{
    humanX += 0.004f;

    if (humanX > 1.2f)
        humanX = -1.2f;

    glutPostRedisplay();
    glutTimerFunc(30, walkHuman, 0);
}

void update(int value)
{
    if(!animationPaused) {
        // Move cars
        car1X += car1Speed;
        if(car1X > 1.2f) car1X = -1.2f;
        if(car1X < -1.2f) car1X = 1.2f;

        car2X += car2Speed;
        if(car2X > 1.2f) car2X = -1.2f;
        if(car2X < -1.2f) car2X = 1.2f;

        // Move boats
        boat1X += 0.005f;
        if(boat1X > 1.2f) boat1X = -1.2f;

        boat2X += 0.003f;
        if(boat2X > 1.2f) boat2X = -1.2f;

        // Move clouds slowly
        cloud1X += 0.0005f;
        cloud2X += 0.0007f;
        cloud3X += 0.0004f;

        if(cloud1X > 1.3f) cloud1X = -1.3f;
        if(cloud2X > 1.3f) cloud2X = -1.3f;
        if(cloud3X > 1.3f) cloud3X = -1.3f;

        // Move birds in day time
        if(!isNightMode) {
            bird1X += 0.003f;  // Birds fly slower than clouds
            bird2X += 0.0025f;
            bird3X += 0.0035f;

            // Reset birds when they fly off screen
            if(bird1X > 1.5f) bird1X = -0.5f;
            if(bird2X > 1.5f) bird2X = -0.5f;
            if(bird3X > 1.5f) bird3X = -0.5f;
        }

        if (isRaining)
        {
            for (int i = 0; i < RAIN_COUNT; i++)
            {
                rainY[i] -= 0.035f; // smoother speed
                if (rainY[i] < -1.0f)
                {
                    rainY[i] = 1.0f;
                    rainX[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                }
            }
        }
        snowMove();
        if (isThunder)
        {
        thunderTimer--;
        if (thunderTimer <= 0) {
            isThunder = false;
        }
    }

    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


//srs
void drawAirplane()
{
    if (!planeOn) return;

    glPushMatrix();
    glTranslatef(planeX, planeY, 0.0f);

    // body
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(-0.12f, -0.02f);
        glVertex2f( 0.12f, -0.02f);
        glVertex2f( 0.14f,  0.02f);
        glVertex2f(-0.14f,  0.02f);
    glEnd();

    // top body
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS);
        glVertex2f(-0.10f, 0.02f);
        glVertex2f( 0.10f, 0.02f);
        glVertex2f( 0.08f, 0.05f);
        glVertex2f(-0.08f, 0.05f);
    glEnd();

    // wings
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-0.02f, -0.02f);
        glVertex2f( 0.02f, -0.02f);
        glVertex2f( 0.18f, -0.08f);
        glVertex2f(-0.18f, -0.08f);
    glEnd();

    // tail
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.12f, 0.02f);
        glVertex2f(-0.08f, 0.10f);
        glVertex2f(-0.04f, 0.02f);
    glEnd();

    // windows
    glColor3f(0.2f, 0.4f, 0.8f);
    for (int i = 0; i < 3; i++)
    {
        glBegin(GL_QUADS);
            glVertex2f(-0.02f + i*0.04f, 0.01f);
            glVertex2f( 0.01f + i*0.04f, 0.01f);
            glVertex2f( 0.01f + i*0.04f, 0.03f);
            glVertex2f(-0.02f + i*0.04f, 0.03f);
        glEnd();
    }

    glPopMatrix();
}

void drawPlane()
{
    if (!planeOn) return;

    float y = 0.65f;   // sky height

    // body
    glColor3f(0.8f, 0.8f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(planeX - 0.08f, y);
        glVertex2f(planeX + 0.08f, y);
        glVertex2f(planeX + 0.06f, y - 0.03f);
        glVertex2f(planeX - 0.06f, y - 0.03f);
    glEnd();

    // cockpit
    glColor3f(0.3f, 0.3f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(planeX + 0.02f, y);
        glVertex2f(planeX + 0.06f, y);
        glVertex2f(planeX + 0.05f, y - 0.02f);
        glVertex2f(planeX + 0.02f, y - 0.02f);
    glEnd();

    // wings
    glColor3f(0.6f, 0.6f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(planeX, y - 0.015f);
        glVertex2f(planeX - 0.12f, y - 0.06f);
        glVertex2f(planeX + 0.12f, y - 0.06f);
    glEnd();

    // tail
    glBegin(GL_TRIANGLES);
        glVertex2f(planeX - 0.06f, y);
        glVertex2f(planeX - 0.09f, y + 0.04f);
        glVertex2f(planeX - 0.04f, y);
    glEnd();
}

void updatePlane(int value)
{
    if (planeOn)
    {
        //planeX += 0.01f;
        planeX += planeSpeed;

        if (planeX > 1.4f)
            planeX = -1.4f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updatePlane, 0);
}



// Keyboard function
void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'd': // Day mode
        case 'D':
            isNightMode = false;
            break;

        case 'n': // Night mode
        case 'N':
            isNightMode = true;
            break;

        case ' ': // Space bar to pause/resume animation
            animationPaused = !animationPaused;
            break;
          case 'w':
            snowIsFalling= true;
            break;
        case 'W':
          snowIsFalling = false;
            break;
            case 't':
case 'T':
    isThunder = true;
    thunderTimer = 10;
    break;
    case 'p':
    isThunder = false;
    break;


       case 'r': // start rain
        case 'R':
            if (key == 'r') {
                isRaining = true;
            }
            else { // 'R' - reset
                car1X = -0.8f;
                car2X = 0.3f;
                bird1X = 0.30f;
                bird2X = 0.48f;
                bird3X = 0.62f;
            }
            break;
        case 's': // stop rain
        case 'S':
            isRaining = false;
            break;

        case '1': // Increase red car speed
            car1Speed += 0.002f;
            break;

        case '2': // Decrease red car speed
            car1Speed -= 0.002f;
            break;

        case '3': // Increase blue car speed
            car2Speed += 0.002f;
            break;

        case '4': // Decrease blue car speed
            car2Speed -= 0.002f;
            break;

        case '5': // Reverse red car direction
            car1Speed = -car1Speed;
            break;

        case '6': // Reverse blue car direction
            car2Speed = -car2Speed;
            break;

        case 'b': // Speed up birds
        case 'B':
            bird1X += 0.1f; // Jump forward
            break;


case 'u':
case 'U':
    if (isRaining)
        umbrellaOn = !umbrellaOn;
    break;
// keys to control keyboard

case 'm':
case 'M':
    planeOn = !planeOn;
    break;
case '+':
    planeSpeed += 0.0005f;
    break;

case '-':
    planeSpeed -= 0.0005f;
    if (planeSpeed < 0.0002f) planeSpeed = 0.0002f;
    break;

        case 27: // ESC key to cancel any feature
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// Special keyboard function for arrow keys
void specialKeys(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_UP: // Speed up all animation
            car1Speed *= 1.1f;
            car2Speed *= 1.1f;
            break;

        case GLUT_KEY_DOWN: // Slow down all animation
            car1Speed *= 0.9f;
            car2Speed *= 0.9f;
            break;

        case GLUT_KEY_LEFT: // Move red car left manually
            car1X -= 0.05f;
            break;

        case GLUT_KEY_RIGHT: // Move red car right manually
            car1X += 0.05f;
            break;

        case GLUT_KEY_F1: // Reset all positions
            car1X = -0.8f;
            car2X = 0.3f;
            bird1X = 0.30f;
            bird2X = 0.48f;
            bird3X = 0.62f;
            car1Speed = 0.01f;
            car2Speed = -0.012f;
            break;
    }

    glutPostRedisplay();
}


void drawLineDDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
}

void drawCircleMidpoint(float cx, float cy, float r)
{
    int x = 0;
    int y = (int)r;
    int d = 1 - (int)r;

    glBegin(GL_POINTS);
    while (x <= y)
    {
        glVertex2f(cx + x, cy + y);
        glVertex2f(cx - x, cy + y);
        glVertex2f(cx + x, cy - y);
        glVertex2f(cx - x, cy - y);
        glVertex2f(cx + y, cy + x);
        glVertex2f(cx - y, cy + x);
        glVertex2f(cx + y, cy - x);
        glVertex2f(cx - y, cy - x);

        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

void drawWindmill()
{
    float cx = 0.65f;   // X position (near mountain)
    float cy = -0.10f;  // Y position (hub/center)
    float bladeLen = 0.12f;

    // Tower - LONGER, touching the road
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(cx - 0.015f, -0.80f);  // Bottom touches road at -0.80f
        glVertex2f(cx + 0.015f, -0.80f);
        glVertex2f(cx + 0.010f, cy);      // Top at hub
        glVertex2f(cx - 0.010f, cy);
    glEnd();

    // Rotating blades
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glRotatef(windmillAngle, 0.0f, 0.0f, 1.0f);

    // Draw 3 blades
    glColor3f(0.9f, 0.9f, 0.9f);
    glLineWidth(3.0f);

    for (int i = 0; i < 3; i++)
    {
        float angle = i * 120.0f; // 3 blades at 120° apart
        float rad = angle * 3.1416f / 180.0f;

        float x2 = bladeLen * cos(rad);
        float y2 = bladeLen * sin(rad);

        // Blade as filled triangle
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(x2 - 0.02f * sin(rad), y2 + 0.02f * cos(rad));
            glVertex2f(x2 + 0.02f * sin(rad), y2 - 0.02f * cos(rad));
        glEnd();
    }

    glPopMatrix();
    glLineWidth(1.0f);

    // Hub (center circle)
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCircleMidpoint(cx, cy, 0.025f);
}

void updateWindmill(int value)
{
    windmillAngle += 2.0f;
    if (windmillAngle > 360.0f)
        windmillAngle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updateWindmill, 0);
}




void display()
{
    // to set the BG
    if(isNightMode)
    {
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Dark blue for night
    }
    else
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White for day
    }

    glClear(GL_COLOR_BUFFER_BIT);

    // Sky
    if(isNightMode)
    {
        glColor3f(0.1f, 0.1f, 0.3f); // Dark blue for night
    }
    else
    {
        glColor3f(0.6f, 0.85f, 1.0f); // Light blue for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-1.0f,  1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f( 1.0f, -0.6f);
        glVertex2f(-1.0f, -0.6f);
    glEnd();

    // Draw sun or moon
    if(isNightMode)
    {
        drawMoon();
    }
    else
    {
        drawSun();
    }

    // BACK MOUNTAINS
    if(isNightMode)
    {
        glColor3f(0.25f, 0.35f, 0.30f); // Dark green for night
    }
    else
    {
        glColor3f(0.45f, 0.60f, 0.50f); // Light green for day
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(-1.3f, -0.55f);
        glVertex2f(-0.9f,  0.45f);
        glVertex2f(-0.5f, -0.55f);

        glVertex2f(-0.9f, -0.55f);
        glVertex2f(-0.4f,  0.50f);
        glVertex2f( 0.1f, -0.55f);

        glVertex2f(-0.4f, -0.55f);
        glVertex2f( 0.2f,  0.55f);
        glVertex2f( 0.8f, -0.55f);

        glVertex2f( 0.2f, -0.55f);
        glVertex2f( 0.7f,  0.48f);
        glVertex2f( 1.3f, -0.55f);
    glEnd();

    // FRONT MOUNTAINS
    if(isNightMode)
    {
        glColor3f(0.10f, 0.20f, 0.15f); // Very dark green for night
    }
    else
    {
        glColor3f(0.20f, 0.38f, 0.26f); // Medium green for day
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(-1.3f, -0.65f);
        glVertex2f(-0.8f,  0.25f);
        glVertex2f(-0.3f, -0.65f);

        glVertex2f(-0.8f, -0.65f);
        glVertex2f(-0.2f,  0.30f);
        glVertex2f( 0.3f, -0.65f);

        glVertex2f(-0.2f, -0.65f);
        glVertex2f( 0.4f,  0.35f);
        glVertex2f( 0.9f, -0.65f);

        glVertex2f( 0.4f, -0.65f);
        glVertex2f( 0.9f,  0.28f);
        glVertex2f( 1.4f, -0.65f);
    glEnd();


    drawWindmill(); // TO READ THE WINDMILL

    // CLOUDS
    if(isNightMode)
    {
        glColor3f(0.4f, 0.4f, 0.5f); // Gray for night
    }
    else
    {
        glColor3f(0.95f, 0.95f, 0.95f); // White for day
    }

    drawCircle(cloud1X, 0.78f, 0.08f);
    drawCircle(cloud1X + 0.07f, 0.82f, 0.10f);
    drawCircle(cloud1X + 0.17f, 0.78f, 0.08f);

    drawCircle(cloud2X, 0.75f, 0.09f);
    drawCircle(cloud2X + 0.08f, 0.79f, 0.11f);
    drawCircle(cloud2X + 0.18f, 0.75f, 0.09f);

    drawCircle(cloud3X, 0.80f, 0.08f);
    drawCircle(cloud3X + 0.08f, 0.84f, 0.10f);
    drawCircle(cloud3X + 0.17f, 0.80f, 0.08f);


    drawBirds(); //TO DRAW THE BIRLS WITH DDA


    // Building 1
    if(isNightMode)
    {
        glColor3f(0.4f, 0.4f, 0.4f); // Dark gray for night
    }
    else
    {
        glColor3f(0.6f, 0.6f, 0.6f); // Light gray for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.95f, -0.6f);
        glVertex2f(-0.80f, -0.6f);
        glVertex2f(-0.80f,  0.3f);
        glVertex2f(-0.95f,  0.3f);
    glEnd();

    // Building windows
    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.8f); // Yellow light
    }
    else
    {
        glColor3f(0.2f, 0.4f, 0.8f); // Blue for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.90f,  0.2f);
        glVertex2f(-0.85f,  0.2f);
        glVertex2f(-0.85f,  0.0f);
        glVertex2f(-0.90f,  0.0f);

        glVertex2f(-0.90f, -0.2f);
        glVertex2f(-0.85f, -0.2f);
        glVertex2f(-0.85f, -0.4f);
        glVertex2f(-0.90f, -0.4f);
    glEnd();

    // Building 2
    if(isNightMode)
    {
        glColor3f(0.6f, 0.5f, 0.3f); // Dark beige for night
    }
    else
    {
        glColor3f(0.8f, 0.7f, 0.5f); // Light beige for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.75f, -0.6f);
        glVertex2f(-0.55f, -0.6f);
        glVertex2f(-0.55f,  0.5f);
        glVertex2f(-0.75f,  0.5f);
    glEnd();

    // Building 2 windows
    if(isNightMode)
    {
        glColor3f(0.9f, 0.9f, 0.6f); // Yellowish light
    }
    else
    {
        glColor3f(0.2f, 0.4f, 0.8f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.70f,  0.4f);
        glVertex2f(-0.60f,  0.4f);
        glVertex2f(-0.60f,  0.2f);
        glVertex2f(-0.70f,  0.2f);

        glVertex2f(-0.70f,  0.0f);
        glVertex2f(-0.60f,  0.0f);
        glVertex2f(-0.60f, -0.2f);
        glVertex2f(-0.70f, -0.2f);

        glVertex2f(-0.70f, -0.4f);
        glVertex2f(-0.60f, -0.4f);
        glVertex2f(-0.60f, -0.6f);
        glVertex2f(-0.70f, -0.6f);
    glEnd();

    // Building 3
    if(isNightMode)
    {
        glColor3f(0.4f, 0.5f, 0.6f);
    }
    else
    {
        glColor3f(0.6f, 0.8f, 0.9f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.50f, -0.6f);
        glVertex2f(-0.30f, -0.6f);
        glVertex2f(-0.30f,  0.1f);
        glVertex2f(-0.50f,  0.1f);
    glEnd();

    //Building3 window
    if(isNightMode) glColor3f(1.0f, 0.9f, 0.5f);
    else glColor3f(0.2f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(-0.48f, 0.05f);
        glVertex2f(-0.44f, 0.05f);
        glVertex2f(-0.44f, -0.03f);
        glVertex2f(-0.48f, -0.03f);
        glVertex2f(-0.38f, 0.05f);
        glVertex2f(-0.34f, 0.05f);
        glVertex2f(-0.34f, -0.03f);
        glVertex2f(-0.38f, -0.03f);

        glVertex2f(-0.48f, -0.20f);

        glVertex2f(-0.44f, -0.20f);
        glVertex2f(-0.44f, -0.28f);
        glVertex2f(-0.48f, -0.28f);
        glVertex2f(-0.38f, -0.20f);
        glVertex2f(-0.34f, -0.20f);
        glVertex2f(-0.34f, -0.28f);
        glVertex2f(-0.38f, -0.28f);

        glVertex2f(-0.48f, -0.45f);
        glVertex2f(-0.44f, -0.45f);
        glVertex2f(-0.44f, -0.53f);
        glVertex2f(-0.48f, -0.53f);
        glVertex2f(-0.38f, -0.45f);
        glVertex2f(-0.34f, -0.45f);
        glVertex2f(-0.34f, -0.53f);
        glVertex2f(-0.38f, -0.53f);
    glEnd();


    // Building 4
    if(isNightMode)
    {
        glColor3f(0.7f, 0.4f, 0.4f);
    }
    else
    {
        glColor3f(0.9f, 0.6f, 0.6f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.6f);
        glVertex2f(-0.15f, -0.6f);
        glVertex2f(-0.15f,  0.35f);
        glVertex2f(-0.25f,  0.35f);
    glEnd();
    //bulding 04 window
if(isNightMode)
{
    glColor3f(1.0f, 0.9f, 0.6f);
}
else
{
    glColor3f(0.2f, 0.4f, 0.8f);
}
glBegin(GL_QUADS);
// Window 1
glVertex2f(-0.23f, 0.30f);
glVertex2f(-0.17f, 0.30f);
glVertex2f(-0.17f, 0.15f);
glVertex2f(-0.23f, 0.15f);
// Window 2
glVertex2f(-0.23f, 0.05f);
glVertex2f(-0.17f, 0.05f);
glVertex2f(-0.17f, -0.10f);
glVertex2f(-0.23f, -0.10f);
// Window 3
glVertex2f(-0.23f, -0.20f);
glVertex2f(-0.17f, -0.20f);
glVertex2f(-0.17f, -0.35f);
glVertex2f(-0.23f, -0.35f);
glEnd();

    // Building 5
    if(isNightMode)
    {
        glColor3f(0.4f, 0.6f, 0.5f);
    }
    else
    {
        glColor3f(0.6f, 0.9f, 0.7f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.10f, -0.6f);
        glVertex2f( 0.10f, -0.6f);
        glVertex2f( 0.10f,  0.25f);
        glVertex2f(-0.10f,  0.25f);
    glEnd();
    //Bulding 05 windows
    if(isNightMode) glColor3f(1.0f, 0.95f, 0.6f);
else glColor3f(0.2f, 0.4f, 0.8f);
glBegin(GL_QUADS);
// Window 1
glVertex2f(-0.08f, 0.20f);
glVertex2f(0.08f, 0.20f);
glVertex2f(0.08f, 0.05f);
glVertex2f(-0.08f, 0.05f);
// Window 2
glVertex2f(-0.08f, -0.05f);
glVertex2f(0.08f, -0.05f);
glVertex2f(0.08f, -0.20f);
glVertex2f(-0.08f, -0.20f);
// Window 3
glVertex2f(-0.08f, -0.30f);
glVertex2f(0.08f, -0.30f);
glVertex2f(0.08f, -0.45f);
glVertex2f(-0.08f, -0.45f);
glEnd();

    // Building 6
    if(isNightMode)
    {
        glColor3f(0.5f, 0.5f, 0.7f);
    }
    else
    {
        glColor3f(0.7f, 0.7f, 0.9f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.15f, -0.6f);
        glVertex2f(0.35f, -0.6f);
        glVertex2f(0.35f,  0.45f);
        glVertex2f(0.15f,  0.45f);
    glEnd();
    //Bulding 06 wnidow

if(isNightMode) glColor3f(1.0f, 0.95f, 0.6f); // Yellow light
    else glColor3f(0.2f, 0.4f, 0.8f); // Blue for day
    glBegin(GL_QUADS);
        // Window 1
        glVertex2f(0.17f, 0.40f);
        glVertex2f(0.25f, 0.40f);
        glVertex2f(0.25f, 0.30f);
        glVertex2f(0.17f, 0.30f);

        // Window 2
        glVertex2f(0.27f, 0.40f);
        glVertex2f(0.33f, 0.40f);
        glVertex2f(0.33f, 0.30f);
        glVertex2f(0.27f, 0.30f);

        // Window 3
        glVertex2f(0.17f, 0.20f);
        glVertex2f(0.25f, 0.20f);
        glVertex2f(0.25f, 0.10f);
        glVertex2f(0.17f, 0.10f);

        // Window 4
        glVertex2f(0.27f, 0.20f);
        glVertex2f(0.33f, 0.20f);
        glVertex2f(0.33f, 0.10f);
        glVertex2f(0.27f, 0.10f);
    glEnd();

    // triangle top
    if(isNightMode)
    {
        glColor3f(0.3f, 0.2f, 0.2f);
    }
    else
    {
        glColor3f(0.5f, 0.3f, 0.3f);
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(0.15f, 0.45f);
        glVertex2f(0.35f, 0.45f);
        glVertex2f(0.25f, 0.60f);
    glEnd();

    //Building 7
    if(isNightMode)
    {
        glColor3f(0.7f, 0.7f, 0.4f);
    }
    else
    {
        glColor3f(0.9f, 0.9f, 0.6f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.40f, -0.6f);
        glVertex2f(0.60f, -0.6f);
        glVertex2f(0.60f,  0.15f);
        glVertex2f(0.40f,  0.15f);
    glEnd();
   // Building 7 windows
if(isNightMode) glColor3f(1.0f, 0.95f, 0.6f); // Yellowish light
else glColor3f(0.2f, 0.4f, 0.8f); // Blue for day

glBegin(GL_QUADS);

// Window 1
glVertex2f(0.42f, 0.05f);
glVertex2f(0.48f, 0.05f);
glVertex2f(0.48f, -0.10f);
glVertex2f(0.42f, -0.10f);
// Window 2
glVertex2f(0.52f, 0.05f);
glVertex2f(0.58f, 0.05f);
glVertex2f(0.58f, -0.10f);
glVertex2f(0.52f, -0.10f);
// Window 3
glVertex2f(0.42f, -0.20f);
glVertex2f(0.48f, -0.20f);
glVertex2f(0.48f, -0.35f);
glVertex2f(0.42f, -0.35f);
// Window 4
glVertex2f(0.52f, -0.20f);
glVertex2f(0.58f, -0.20f);
glVertex2f(0.58f, -0.35f);
glVertex2f(0.52f, -0.35f);
glEnd();


    // Building 8
    if(isNightMode)
    {
        glColor3f(0.4f, 0.4f, 0.6f);
    }
    else
    {
        glColor3f(0.6f, 0.6f, 0.8f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.65f, -0.6f);
        glVertex2f(0.85f, -0.6f);
        glVertex2f(0.85f,  0.30f);
        glVertex2f(0.65f,  0.30f);
    glEnd();

    //Bulding 08 windows
   if(isNightMode) glColor3f(1.0f, 0.95f, 0.6f); // Night light
else glColor3f(0.2f, 0.4f, 0.8f);             // Day color

glBegin(GL_QUADS);


// Window 1
glVertex2f(0.735f, 0.15f);
glVertex2f(0.795f, 0.15f);
glVertex2f(0.795f, 0.00f);
glVertex2f(0.735f, 0.00f);

// Window 2
glVertex2f(0.67f, -0.1f);
glVertex2f(0.73f, -0.1f);
glVertex2f(0.73f, -0.25f);
glVertex2f(0.67f, -0.25f);

glEnd();
    // Building 9
    if(isNightMode)
    {
        glColor3f(0.6f, 0.6f, 0.6f);
    }
    else
    {
        glColor3f(0.8f, 0.8f, 0.8f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.88f, -0.6f);
        glVertex2f(1.05f, -0.6f);
        glVertex2f(1.05f,  0.25f);
        glVertex2f(0.88f,  0.25f);
    glEnd();
    // Building 9 windows
if(isNightMode) glColor3f(1.0f, 0.95f, 0.6f);
else glColor3f(0.2f, 0.4f, 0.8f);

glBegin(GL_QUADS);
// Column 1
glVertex2f(0.92f, 0.25f);
glVertex2f(0.98f, 0.25f);
glVertex2f(0.98f, 0.10f);
glVertex2f(0.92f, 0.10f);
glVertex2f(0.92f, 0.0f);
glVertex2f(0.98f, 0.0f);
glVertex2f(0.98f, -0.15f);
glVertex2f(0.92f, -0.15f);
glVertex2f(0.92f, -0.25f);
glVertex2f(0.98f, -0.25f);
glVertex2f(0.98f, -0.40f);
glVertex2f(0.92f, -0.40f);
// Column 2
glVertex2f(1.02f, 0.25f);
glVertex2f(1.08f, 0.25f);
glVertex2f(1.08f, 0.10f);
glVertex2f(1.02f, 0.10f);
glVertex2f(1.02f, 0.0f);
glVertex2f(1.08f, 0.0f);
glVertex2f(1.08f, -0.15f);
glVertex2f(1.02f, -0.15f);
glVertex2f(1.02f, -0.25f);
glVertex2f(1.08f, -0.25f);
glVertex2f(1.08f, -0.40f);
glVertex2f(1.02f, -0.40f);
glEnd();


// SRS
// HUMAN

// head
glColor3f(1.0f, 0.85f, 0.7f);
drawCircle(humanX + 0.03f, -0.54f, 0.025f);

// COWBOY HEAD

// hat brim
glColor3f(0.40f, 0.25f, 0.10f);
glBegin(GL_QUADS);
    glVertex2f(humanX - 0.005f, -0.52f);
    glVertex2f(humanX + 0.065f, -0.52f);
    glVertex2f(humanX + 0.065f, -0.50f);
    glVertex2f(humanX - 0.005f, -0.50f);
glEnd();

// hat crown
glBegin(GL_QUADS);
    glVertex2f(humanX + 0.015f, -0.50f);
    glVertex2f(humanX + 0.045f, -0.50f);
    glVertex2f(humanX + 0.045f, -0.46f);
    glVertex2f(humanX + 0.015f, -0.46f);
glEnd();


// BODY
glColor3f(0.2f, 0.4f, 0.8f);
glBegin(GL_QUADS);
    glVertex2f(humanX + 0.01f, -0.57f);
    glVertex2f(humanX + 0.05f, -0.57f);
    glVertex2f(humanX + 0.05f, -0.66f);
    glVertex2f(humanX + 0.01f, -0.66f);
glEnd();


// LEGS
glColor3f(0.1f, 0.1f, 0.1f);

// left leg
glBegin(GL_QUADS);
    glVertex2f(humanX + 0.015f, -0.66f);
    glVertex2f(humanX + 0.03f,  -0.66f);
    glVertex2f(humanX + 0.025f, -0.75f);
    glVertex2f(humanX + 0.01f,  -0.75f);
glEnd();

// right leg
glBegin(GL_QUADS);
    glVertex2f(humanX + 0.03f,  -0.66f);
    glVertex2f(humanX + 0.045f, -0.66f);
    glVertex2f(humanX + 0.05f,  -0.75f);
    glVertex2f(humanX + 0.035f, -0.75f);
glEnd();


// UMBRELLA
if (umbrellaOn && isRaining)
{
    // umbrella handle
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(humanX + 0.055f, -0.58f);
        glVertex2f(humanX + 0.062f, -0.58f);
        glVertex2f(humanX + 0.062f, -0.45f);
        glVertex2f(humanX + 0.055f, -0.45f);
    glEnd();

    // ITS CAP
    glColor3f(0.85f, 0.1f, 0.1f);

    float cx = humanX + 0.058f;
    float cy = -0.43f;
    float r  = 0.055f;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 180; i++)
        {
            float angle = i * 3.1416f / 180.0f;
            glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
        }
    glEnd();

    // canopy edge trim
    glColor3f(0.6f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(cx - r, cy);
        glVertex2f(cx + r, cy);
    glEnd();
}



    // Trees
    if(isNightMode)
    {
        glColor3f(0.35f, 0.17f, 0.05f); // Dark brown
    }
    else
    {
        glColor3f(0.55f, 0.27f, 0.07f); // Light brown
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.90f, -0.80f);
        glVertex2f(-0.87f, -0.80f);
        glVertex2f(-0.87f, -0.65f);
        glVertex2f(-0.90f, -0.65f);
    glEnd();

    if(isNightMode)
    {
        glColor3f(0.0f, 0.3f, 0.0f); // Dark green
    }
    else
    {
        glColor3f(0.0f, 0.6f, 0.0f); // Light green
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.95f, -0.65f);
        glVertex2f(-0.82f, -0.65f);
        glVertex2f(-0.885f, -0.55f);
    glEnd();

    // Tree 2 trunk
    if(isNightMode)
    {
        glColor3f(0.35f, 0.17f, 0.05f);
    }
    else
    {
        glColor3f(0.55f, 0.27f, 0.07f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.15f, -0.80f);
        glVertex2f(-0.12f, -0.80f);
        glVertex2f(-0.12f, -0.65f);
        glVertex2f(-0.15f, -0.65f);
    glEnd();

    // Tree 2 leaves
    if(isNightMode)
    {
        glColor3f(0.0f, 0.35f, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 0.7f, 0.0f);
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.20f, -0.65f);
        glVertex2f(-0.07f, -0.65f);
        glVertex2f(-0.135f, -0.54f);
    glEnd();

    // Tree 3 trunk
    if(isNightMode)
    {
        glColor3f(0.35f, 0.17f, 0.05f);
    }
    else
    {
        glColor3f(0.55f, 0.27f, 0.07f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.55f, -0.80f);
        glVertex2f(0.58f, -0.80f);
        glVertex2f(0.58f, -0.65f);
        glVertex2f(0.55f, -0.65f);
    glEnd();

    // Tree 3 leaves
    if(isNightMode)
    {
        glColor3f(0.0f, 0.3f, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 0.65f, 0.0f);
    }

    glBegin(GL_TRIANGLES);
        glVertex2f(0.50f, -0.65f);
        glVertex2f(0.63f, -0.65f);
        glVertex2f(0.565f, -0.54f);
    glEnd();

    // Street Lamps SRS
    // Lamp 1 pole
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-0.65f, -0.80f);
        glVertex2f(-0.63f, -0.80f);
        glVertex2f(-0.63f, -0.40f);
        glVertex2f(-0.65f, -0.40f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(-0.65f, -0.40f);
        glVertex2f(-0.58f, -0.40f);
        glVertex2f(-0.58f, -0.38f);
        glVertex2f(-0.65f, -0.38f);
    glEnd();

    // Lamp head
    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.8f); // Bright yellow
    }
    else
    {
        glColor3f(1.0f, 0.9f, 0.5f); // Pale yellow
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.60f, -0.38f);
        glVertex2f(-0.56f, -0.38f);
        glVertex2f(-0.56f, -0.35f);
        glVertex2f(-0.60f, -0.35f);
    glEnd();

    // Light glow
    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.7f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.61f, -0.38f);
            glVertex2f(-0.55f, -0.38f);
            glVertex2f(-0.58f, -0.65f);
        glEnd();
    }

    // Lamp 2
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-0.05f, -0.80f);
        glVertex2f(-0.03f, -0.80f);
        glVertex2f(-0.03f, -0.40f);
        glVertex2f(-0.05f, -0.40f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(-0.05f, -0.40f);
        glVertex2f( 0.02f, -0.40f);
        glVertex2f( 0.02f, -0.38f);
        glVertex2f(-0.05f, -0.38f);
    glEnd();

    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.8f);
    }
    else
    {
        glColor3f(1.0f, 0.9f, 0.5f);
    }

    glBegin(GL_QUADS);
        glVertex2f( 0.00f, -0.38f);
        glVertex2f( 0.04f, -0.38f);
        glVertex2f( 0.04f, -0.35f);
        glVertex2f( 0.00f, -0.35f);
    glEnd();

    if(isNightMode) {
        glColor3f(1.0f, 1.0f, 0.7f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.01f, -0.38f);
            glVertex2f( 0.05f, -0.38f);
            glVertex2f( 0.02f, -0.65f);
        glEnd();
    }

    // Lamp 3
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0.54f, -0.80f);
        glVertex2f(0.56f, -0.80f);
        glVertex2f(0.56f, -0.40f);
        glVertex2f(0.54f, -0.40f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(0.54f, -0.40f);
        glVertex2f(0.61f, -0.40f);
        glVertex2f(0.61f, -0.38f);
        glVertex2f(0.54f, -0.38f);
    glEnd();

    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.8f);
    }
    else
    {
        glColor3f(1.0f, 0.9f, 0.5f);
    }

    glBegin(GL_QUADS);
        glVertex2f(0.59f, -0.38f);
        glVertex2f(0.63f, -0.38f);
        glVertex2f(0.63f, -0.35f);
        glVertex2f(0.59f, -0.35f);
    glEnd();

    if(isNightMode)
    {
        glColor3f(1.0f, 1.0f, 0.7f);
        glBegin(GL_TRIANGLES);
            glVertex2f(0.58f, -0.38f);
            glVertex2f(0.64f, -0.38f);
            glVertex2f(0.61f, -0.65f);
        glEnd();
    }

    // ROAD WITH TWO LANES
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.8f);
        glVertex2f( 1.0f, -0.8f);
        glVertex2f( 1.0f, -0.6f);
        glVertex2f(-1.0f, -0.6f);
    glEnd();

    // Lane divider
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.7f);
        glVertex2f( 1.0f, -0.7f);
        glVertex2f( 1.0f, -0.69f);
        glVertex2f(-1.0f, -0.69f);
    glEnd();

    // ROAD WHITE DASHES FOR BOTH LANES
    glColor3f(1.0f, 1.0f, 1.0f);

    // TOP LANE dashes
    glBegin(GL_QUADS);
        glVertex2f(-0.9f, -0.65f);
        glVertex2f(-0.7f, -0.65f);
        glVertex2f(-0.7f, -0.63f);
        glVertex2f(-0.9f, -0.63f);

        glVertex2f(-0.5f, -0.65f);
        glVertex2f(-0.3f, -0.65f);
        glVertex2f(-0.3f, -0.63f);
        glVertex2f(-0.5f, -0.63f);

        glVertex2f(-0.1f, -0.65f);
        glVertex2f( 0.1f, -0.65f);
        glVertex2f( 0.1f, -0.63f);
        glVertex2f(-0.1f, -0.63f);

        glVertex2f( 0.3f, -0.65f);
        glVertex2f( 0.5f, -0.65f);
        glVertex2f( 0.5f, -0.63f);
        glVertex2f( 0.3f, -0.63f);

        glVertex2f( 0.7f, -0.65f);
        glVertex2f( 0.9f, -0.65f);
        glVertex2f( 0.9f, -0.63f);
        glVertex2f( 0.7f, -0.63f);
    glEnd();

    // BOTTOM LANE dashes
    glBegin(GL_QUADS);
        glVertex2f(-0.9f, -0.73f);
        glVertex2f(-0.7f, -0.73f);
        glVertex2f(-0.7f, -0.71f);
        glVertex2f(-0.9f, -0.71f);

        glVertex2f(-0.5f, -0.73f);
        glVertex2f(-0.3f, -0.73f);
        glVertex2f(-0.3f, -0.71f);
        glVertex2f(-0.5f, -0.71f);

        glVertex2f(-0.1f, -0.73f);
        glVertex2f( 0.1f, -0.73f);
        glVertex2f( 0.1f, -0.71f);
        glVertex2f(-0.1f, -0.71f);

        glVertex2f( 0.3f, -0.73f);
        glVertex2f( 0.5f, -0.73f);
        glVertex2f( 0.5f, -0.71f);
        glVertex2f( 0.3f, -0.71f);

        glVertex2f( 0.7f, -0.73f);
        glVertex2f( 0.9f, -0.73f);
        glVertex2f( 0.9f, -0.71f);
        glVertex2f( 0.7f, -0.71f);
    glEnd();

    // DrawING moving cars
    drawMovingCar1();  // Red car on bottom lane
    drawMovingCar2();  // Blue car on top lane

    // Grass area below road
    if(isNightMode)
    {
        glColor3f(0.1f, 0.4f, 0.1f); // Dark green for night
    }
    else
    {
        glColor3f(0.2f, 0.8f, 0.2f); // Bright green for day
    }

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.9f);
        glVertex2f( 1.0f, -0.9f);
        glVertex2f( 1.0f, -0.8f);
        glVertex2f(-1.0f, -0.8f);
    glEnd();

    // Draw bank
    drawBank();

    // Draw river
    drawRiver();
    if(isNightMode)
    {
    glColor3f(0.4f, 0.25f, 0.1f);
}
else
{
    glColor3f(0.6f, 0.4f, 0.2f);
}
glLineWidth(5.0f);
drawBezierCurve(-0.5f, -0.92f, -0.25f, -0.87f, 0.25f, -0.87f, 0.5f, -0.92f);
glLineWidth(1.0f);

// Bridge support pillars
glBegin(GL_QUADS);
    glVertex2f(-0.42f, -0.92f);
glVertex2f(-0.38f, -0.92f);
    glVertex2f(-0.38f, -0.96f);
    glVertex2f(-0.42f, -0.96f);

    glVertex2f(0.38f, -0.92f);
    glVertex2f(0.42f, -0.92f);
    glVertex2f(0.42f, -0.96f);
    glVertex2f(0.38f, -0.96f);
glEnd();
drawWindmill();
    // Draw boats
    drawBoats();
    drawRain();
    snowDraw();
    drawThunder();
    drawAirplane();
    drawPlane();
drawWindmill();
    glutSwapBuffers();
    glFlush();
}

void initGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 480);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("City Scenery ");
    glutTimerFunc(16, updatePlane, 0);

    initRain();

    glutDisplayFunc(display);
    glutTimerFunc(16, updatePlane, 0);
    glutIdleFunc(update);

    glutTimerFunc(25, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    makeSnow();
    srand(time(NULL));
    initGL();
    glutKeyboardFunc(keyboard);   //srs_ for human functions
    glutTimerFunc(30, walkHuman, 0);  //srs_ for human functions
    glutTimerFunc(16, updateWindmill, 0);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updatePlane, 0);
    glutTimerFunc(0, updatePlane, 0);
    glutMainLoop();
    return 0;
}

