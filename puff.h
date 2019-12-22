#ifndef PUFF_H
#define PUFF_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Puff
{
public:
    Puff(QVector3D position, float taille, QVector3D vitesse, float life, QVector3D color);
    void animate(float dt);
    void display(QOpenGLShaderProgram *sp, QOpenGLBuffer vbo_particule);
    QVector3D position;
    float taille;
    QVector3D vitesse;
    float life;
    QMatrix4x4 modelMatrix;
    QVector3D color;
    float numTexture = std::rand()%16;
private:

};

#endif // PUFF_H
