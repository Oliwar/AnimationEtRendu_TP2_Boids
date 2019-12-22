// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <cmath>

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    elapsedTimer.start();


    Obstacle o1 = Obstacle(QVector3D(5, 0, -7), 5.0f);
    Obstacle o2 = Obstacle(QVector3D(-10, 10, 0), 3.0f);
    Obstacle o3 = Obstacle(QVector3D(0, -9, 5), 2.0f);

    obstacles.push_back(o1);
    obstacles.push_back(o2);
    obstacles.push_back(o3);

    Predateur p1 = Predateur(QVector3D(0, 0, 0), 1.0f, QVector3D(0.5f, 0.5f, 0.5f));

    predateurs.push_back(p1);

    banc = Banc(100, largeurAquarium, hauteurAquarium, profondeurAquarium, obstacles, predateurs);
}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5f,0.5f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_sol = new QOpenGLShaderProgram(this);
    program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_sol->log();
    }
    program_sol->setUniformValue("texture", 0);

    // shader de billboard
    program_particule = new QOpenGLShaderProgram(this);
    program_particule->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_particule->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_particule->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_particule->log();
    }
    program_particule->setUniformValue("texture", 0);

    // shader du poisson
    program_poisson = new QOpenGLShaderProgram(this);
    program_poisson->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/poisson.vsh");
    program_poisson->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/poisson.fsh");
    if (! program_poisson->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_obstacle->log();
    }
    program_poisson->setUniformValue("texture", 0);

    // shader de l'obstacle
    program_obstacle = new QOpenGLShaderProgram(this);
    program_obstacle->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/obstacle.vsh");
    program_obstacle->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/obstacle.fsh");
    if (! program_obstacle->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_obstacle->log();
    }
    program_obstacle->setUniformValue("texture", 0);

    // shader du predateur
    program_predateur = new QOpenGLShaderProgram(this);
    program_predateur->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/predateur.vsh");
    program_predateur->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/predateur.fsh");
    if (! program_predateur->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_predateur->log();
    }
    program_predateur->setUniformValue("texture", 0);

}


void GLArea::makeGLObjects()
{
    // Création de l'aquarium
    largeurAquarium /= 2;
    hauteurAquarium /= 2;
    profondeurAquarium /= 2;
    GLfloat vertices_aquarium[] = {
        -largeurAquarium, -hauteurAquarium, -profondeurAquarium,
         largeurAquarium, -hauteurAquarium, -profondeurAquarium,
        -largeurAquarium,  hauteurAquarium, -profondeurAquarium,
         largeurAquarium,  hauteurAquarium, -profondeurAquarium,
        -largeurAquarium, -hauteurAquarium, -profondeurAquarium,
        -largeurAquarium,  hauteurAquarium, -profondeurAquarium,
         largeurAquarium, -hauteurAquarium, -profondeurAquarium,
         largeurAquarium,  hauteurAquarium, -profondeurAquarium,

        -largeurAquarium, -hauteurAquarium,  profondeurAquarium,
         largeurAquarium, -hauteurAquarium,  profondeurAquarium,
        -largeurAquarium,  hauteurAquarium,  profondeurAquarium,
         largeurAquarium,  hauteurAquarium,  profondeurAquarium,
        -largeurAquarium, -hauteurAquarium,  profondeurAquarium,
        -largeurAquarium,  hauteurAquarium,  profondeurAquarium,
         largeurAquarium, -hauteurAquarium,  profondeurAquarium,
         largeurAquarium,  hauteurAquarium,  profondeurAquarium,

        -largeurAquarium, -hauteurAquarium, -profondeurAquarium,
        -largeurAquarium, -hauteurAquarium,  profondeurAquarium,
        -largeurAquarium,  hauteurAquarium, -profondeurAquarium,
        -largeurAquarium,  hauteurAquarium,  profondeurAquarium,
         largeurAquarium, -hauteurAquarium, -profondeurAquarium,
         largeurAquarium, -hauteurAquarium,  profondeurAquarium,
         largeurAquarium,  hauteurAquarium, -profondeurAquarium,
         largeurAquarium,  hauteurAquarium,  profondeurAquarium
    };


    QVector<GLfloat> vertData_aquarium;
    for (int i = 0; i < 24; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_aquarium.append(vertices_aquarium[i*3+j]);
    }

    vbo_aquarium.create();
    vbo_aquarium.bind();
    vbo_aquarium.allocate(vertData_aquarium.constData(), vertData_aquarium.count() * int(sizeof(GLfloat)));

    // Création du poisson
    GLfloat vertices_poisson[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };

    QVector<GLfloat> vertData_poisson;
    for (int i = 0; i < 3; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_poisson.append(vertices_poisson[i*3+j]);
    }

    vbo_poisson.create();
    vbo_poisson.bind();
    vbo_poisson.allocate(vertData_poisson.constData(), vertData_poisson.count() * int(sizeof(GLfloat)));

    // Création de l'obstacle
    QVector<GLfloat> vertices_obstacle;
    QVector<GLfloat> vertices_obstacle_color;
    float rayon;
    float rouge;
    float vert;
    float bleue;
    for(Obstacle obstacle : obstacles){
        rayon = obstacle.rayon;
        rouge = banc.rand_float(0.0f, 1.0f);
        vert = banc.rand_float(0.0f, 1.0f);
        bleue = banc.rand_float(0.0f, 1.0f);
        for(int i = 0; i <= 40; i++)
        {
            float lat0 = static_cast<float>(M_PI) * (-0.5f + (i - 1) / 40.0f);
            float z0  = rayon*sin(lat0);
            float zr0 =  rayon*cos(lat0);

            float lat1 = static_cast<float>(M_PI) * (-0.5f + i / 40.0f);
            float z1 = rayon*sin(lat1);
            float zr1 = rayon*cos(lat1);

            for(int j = 0; j <= 40; j++)
            {
                float lng = 2 * static_cast<float>(M_PI) * (j - 1) / 40.0f;
                float x = cos(lng);
                float y = sin(lng);

                vertices_obstacle.push_back(x * zr0 + obstacle.position[0]); //X
                vertices_obstacle.push_back(y * zr0 + obstacle.position[1]); //Y
                vertices_obstacle.push_back(z0 + obstacle.position[2]);      //Z
                vertices_obstacle_color.push_back(rouge);
                vertices_obstacle_color.push_back(vert);
                vertices_obstacle_color.push_back(bleue);

                vertices_obstacle.push_back(x * zr1 + obstacle.position[0]); //X
                vertices_obstacle.push_back(y * zr1 + obstacle.position[1]); //Y
                vertices_obstacle.push_back(z1 + obstacle.position[2]);      //Z
                vertices_obstacle_color.push_back(rouge);
                vertices_obstacle_color.push_back(vert);
                vertices_obstacle_color.push_back(bleue);
            }
        }
    }

    QVector<GLfloat> vertData_obstacle;
    for (int i = 0; i < 3200*static_cast<int>(obstacles.size()); ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_obstacle.append(vertices_obstacle[i*3+j]);
        for (int j = 0; j < 3; j++)
            vertData_obstacle.append(vertices_obstacle_color[i*3+j]);
    }

    vbo_obstacle.create();
    vbo_obstacle.bind();
    vbo_obstacle.allocate(vertData_obstacle.constData(), vertData_obstacle.count() * int(sizeof(GLfloat)));

    // Création du predateur
    QVector<GLfloat> vertices_predateur;
    QVector<GLfloat> vertices_predateur_color;
    for(Predateur predateur : predateurs){
        rayon = predateur.rayon;
        rouge = banc.rand_float(0.0f, 1.0f);
        vert = banc.rand_float(0.0f, 1.0f);
        bleue = banc.rand_float(0.0f, 1.0f);
        for(int i = 0; i <= 40; i++)
        {
            float lat0 = static_cast<float>(M_PI) * (-0.5f + (i - 1) / 40.0f);
            float z0  = rayon*sin(lat0);
            float zr0 =  rayon*cos(lat0);

            float lat1 = static_cast<float>(M_PI) * (-0.5f + i / 40.0f);
            float z1 = rayon*sin(lat1);
            float zr1 = rayon*cos(lat1);

            for(int j = 0; j <= 40; j++)
            {
                float lng = 2 * static_cast<float>(M_PI) * (j - 1) / 40.0f;
                float x = cos(lng);
                float y = sin(lng);

                vertices_predateur.push_back(x * zr0 + predateur.position[0]); //X
                vertices_predateur.push_back(y * zr0 + predateur.position[1]); //Y
                vertices_predateur.push_back(z0 + predateur.position[2]);      //Z
                vertices_predateur_color.push_back(rouge);
                vertices_predateur_color.push_back(vert);
                vertices_predateur_color.push_back(bleue);

                vertices_predateur.push_back(x * zr1 + predateur.position[0]); //X
                vertices_predateur.push_back(y * zr1 + predateur.position[1]); //Y
                vertices_predateur.push_back(z1 + predateur.position[2]);      //Z
                vertices_predateur_color.push_back(rouge);
                vertices_predateur_color.push_back(vert);
                vertices_predateur_color.push_back(bleue);
            }
        }
    }

    QVector<GLfloat> vertData_predateur;
    for (int i = 0; i < 3200*static_cast<int>(predateurs.size()); ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_predateur.append(vertices_predateur[i*3+j]);
        for (int j = 0; j < 3; j++)
            vertData_predateur.append(vertices_predateur_color[i*3+j]);
    }

    vbo_predateur.create();
    vbo_predateur.bind();
    vbo_predateur.allocate(vertData_predateur.constData(), vertData_predateur.count() * int(sizeof(GLfloat)));
}


void GLArea::tearGLObjects()
{
    vbo_sol.destroy();
    vbo_poisson.destroy();
    vbo_aquarium.destroy();
    vbo_obstacle.destroy();
    vbo_predateur.destroy();
    for (int i = 0; i < 2; i++)
        delete textures[i];
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);

    /* AQUARIUM */
    vbo_aquarium.bind();
    program_sol->bind();

    QMatrix4x4 modelMatrixAquarium;
    modelMatrixAquarium.translate(0.0f, 0.0f, 0.0f);
    program_sol->setUniformValue("projectionMatrix", projectionMatrix);
    program_sol->setUniformValue("viewMatrix", viewMatrix);
    program_sol->setUniformValue("modelMatrix", modelMatrixAquarium);

    program_sol->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program_sol->enableAttributeArray("in_position");

    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 24);

    program_sol->disableAttributeArray("in_position");
    program_sol->release();
    vbo_aquarium.release();
    /* FIN AQUARIUM */

    /* OBSTACLE */
    vbo_obstacle.bind();
    program_obstacle->bind();

    QMatrix4x4 modelMatrixObstacle;
    modelMatrixObstacle.translate(0.0f, 0.0f, 0.0f);
    program_obstacle->setUniformValue("projectionMatrix", projectionMatrix);
    program_obstacle->setUniformValue("viewMatrix", viewMatrix);

    program_obstacle->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    program_obstacle->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program_obstacle->enableAttributeArray("in_position");
    program_obstacle->enableAttributeArray("colAttr");

    int start = 0;
    for(Obstacle o : obstacles){
        o.affiche(program_obstacle, start);
        start += 3362;
    }

    program_obstacle->disableAttributeArray("in_position");
    program_obstacle->disableAttributeArray("colAttr");
    program_obstacle->release();
    vbo_obstacle.release();
    /* FIN OBSTACLE */

    /* PREDATEUR */
    vbo_predateur.bind();
    program_predateur->bind();

    QMatrix4x4 modelMatrixPredateur;
    modelMatrixPredateur.translate(0.0f, 0.0f, 0.0f);
    program_predateur->setUniformValue("projectionMatrix", projectionMatrix);
    program_predateur->setUniformValue("viewMatrix", viewMatrix);

    program_predateur->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    program_predateur->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program_predateur->enableAttributeArray("in_position");
    program_predateur->enableAttributeArray("colAttr");

    start = 0;
    for(Predateur &p : predateurs){
        p.affiche(program_predateur, start);
        start += 3362;
    }

    program_predateur->disableAttributeArray("in_position");
    program_predateur->disableAttributeArray("colAttr");
    program_predateur->release();
    vbo_predateur.release();
    /* FIN PREDATEUR */

    /* BANC */
    glDepthMask(GL_FALSE);

    // Affichage d'un poisson
    vbo_poisson.bind();
    program_poisson->bind();

    program_poisson->setUniformValue("projectionMatrix", projectionMatrix);
    program_poisson->setUniformValue("viewMatrix", viewMatrix);

    program_poisson->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program_poisson->enableAttributeArray("in_position");

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    banc.affiche(program_poisson);
    glDisable(GL_BLEND);

    program_poisson->disableAttributeArray("in_position");
    program_poisson->release();
    vbo_poisson.release();
    glDepthMask(GL_TRUE);
    /* FIN BANC */
}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key()) {
        case Qt::Key_A :
            xRot -= da;
            update();
            break;

        case Qt::Key_Q :
            xRot += da;
            update();
            break;

        case Qt::Key_Z :
            yRot -= da;
            update();
            break;

        case Qt::Key_S :
            yRot += da;
            update();
            break;

        case Qt::Key_E :
            zRot -= da;
            update();
            break;

        case Qt::Key_D :
            zRot += da;
            update();
            break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;

    banc.anime(dt);

    for(Predateur &p : predateurs)
        p.anime(dt, banc.poissons);

    update();
}
