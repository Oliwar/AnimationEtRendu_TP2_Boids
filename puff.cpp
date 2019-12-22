#include "puff.h"
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QDebug>
#include <math.h>

Puff::Puff(QVector3D position, float taille, QVector3D vitesse, float life, QVector3D color)
{
    this->position = position;
    this->taille = taille;
    this->vitesse = vitesse;
    this->life = life;
    this->color = color;
}

void Puff::animate(float dt){
    this->position += this->vitesse * dt;
    this->life -= dt;
    this->taille += dt*0.5f;
    this->numTexture += dt*10.0f;
    if(numTexture > 15.0f) numTexture = 0;
}

void Puff::display(QOpenGLShaderProgram *sp, QOpenGLBuffer vbo_particule){
    vbo_particule.bind();

    this->modelMatrix.setToIdentity();
    this->modelMatrix.translate(position);

    sp->setUniformValue("modelMatrix", this->modelMatrix);
    sp->setUniformValue("particleSize", this->taille);
    sp->setUniformValue("elapsed", this->life);
    sp->setUniformValue("color", color);
    sp->setUniformValue("numTexture", floor(numTexture));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

