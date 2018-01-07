#ifndef CLONING_H
#define CLONING_H

#include <QImage>
#include <deque>
#include <vector>
#include <QRgb>
#include <algorithm>
#include <IterativeLinearSolvers>
#include <QDebug>
#include <QColor>
#define EIGEN_INITIALIZE_MATRICES_BY_ZERO

class Cloning
{
public:
    Cloning();
    static std::vector<std::vector<bool> > masked(const QImage & source);
    static QImage directCloning(const QImage & source, const QImage& background);
    static QImage seamlessCloning(const QImage & sourceplus, const QImage& background, const QImage & source);    
};

#endif // CLONING_H
