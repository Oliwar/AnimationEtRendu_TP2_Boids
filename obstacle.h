#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector3D>

class Obstacle
{
public:
    Obstacle(QVector3D position, float rayon);

    QVector3D position;
    float rayon;

    void affiche(QOpenGLShaderProgram *sp, int start);

    QMatrix4x4 modelMatrix;
};

#endif // OBSTACLE_H
