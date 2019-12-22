#ifndef SMOKE_H
#define SMOKE_H

#include "puff.h"
#include <QVector3D>
#include <list>
#include <QOpenGLShaderProgram>

using namespace std;

class Smoke
{
public:
    Smoke(QVector3D position, float timeInterval);
    void animate(float dt);
    QVector3D position;
    list<Puff> puffsList;
    float timeInterval;
    float elapsedTime = 0;
    float time_to_live_particle = std::rand()%8+5;
    QVector3D particle_color = QVector3D(1, 1, 1);
    void display(QOpenGLShaderProgram *sp, QOpenGLBuffer vbo_particule);
private:

};

#endif // SMOKE_H
