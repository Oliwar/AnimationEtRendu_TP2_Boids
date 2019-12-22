#include "smoke.h"
#include <QDebug>

Smoke::Smoke(QVector3D position, float timeInterval)
{
    this->position = position;
    this->timeInterval = timeInterval;
}

void Smoke::animate(float dt){
    this->elapsedTime += dt;
    if(this->elapsedTime > this->timeInterval){
        this->elapsedTime = 0;
        QVector3D *vitesse = new QVector3D(0, 1.0f, 0);
        Puff *puff = new Puff(this->position, 1.0f, *vitesse, time_to_live_particle, particle_color);
        this->puffsList.push_back(*puff);
    }

    list<Puff>::iterator i;
    i = puffsList.begin();
    while(i != puffsList.end()){
        if( i->life <= 0 ) i = puffsList.erase(i);
        else i++;
    }

    i = puffsList.begin();
    while(i != puffsList.end()){
        i->animate(dt);
        i++;
    }
}

void Smoke::display(QOpenGLShaderProgram *sp, QOpenGLBuffer vbo_particule){
    list<Puff>::iterator i;
    i = puffsList.begin();
    while(i != puffsList.end()){
        i->display(sp, vbo_particule);
        i++;
    }
}
