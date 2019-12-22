// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include "banc.h"
#include "obstacle.h"
#include "predateur.h"
#include "smoke.h"

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

    Banc banc;
    list<Obstacle> obstacles;
    list<Predateur> predateurs;

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    float xRot=20.0f, yRot=0.0f, zRot=0.0f;
    float xPos=0.0f,  yPos=0.0f, zPos=-50.0f;
    QTimer *timer = nullptr;
    QElapsedTimer elapsedTimer;
    float dt = 0;
    float windowRatio = 1.0f;
    QPoint lastPos;

    float largeurAquarium = 40;
    float hauteurAquarium = 40;
    float profondeurAquarium = 40;

    QOpenGLShaderProgram *program_sol;
    QOpenGLShaderProgram *program_particule;
    QOpenGLShaderProgram *program_poisson;
    QOpenGLShaderProgram *program_obstacle;
    QOpenGLShaderProgram *program_predateur;

    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_poisson;
    QOpenGLBuffer vbo_aquarium;
    QOpenGLBuffer vbo_obstacle;
    QOpenGLBuffer vbo_predateur;
    QOpenGLTexture *textures[2];

    void makeGLObjects();
    void tearGLObjects();
};

#endif // GLAREA_H
