#ifndef POISSON_H
#define POISSON_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector3D>

class Poisson
{
public:
    Poisson();
    Poisson(QVector3D position, QVector3D vitesse, float taille, float distance);
    //~Poisson();
    void anime(float dt);
    bool dans_voisinage(QVector3D p);
    bool dans_voisinage_sphere(QVector3D p, float rayon);
    void affiche(QOpenGLShaderProgram *sp);

    QVector3D position;
    QVector3D vitesse;
    float taille;
    float distance;
    float vitesse_max = 1.0f;

    QMatrix4x4 modelMatrix;
};

#endif // POISSON_H
