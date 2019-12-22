// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);
private slots:
    void on_doubleSpinBox_vitesse_max_poisson_valueChanged(double arg1);
    void on_doubleSpinBox_vitesse_max_predateur_valueChanged(double arg1);
    void on_doubleSpinBox_coef_separation_valueChanged(double arg1);
    void on_doubleSpinBox_coef_alignement_valueChanged(double arg1);
    void on_doubleSpinBox_coef_cohesion_valueChanged(double arg1);
    void on_doubleSpinBox_rayon_detection_valueChanged(double arg1);
};

#endif // PRINC_H
