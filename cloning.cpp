#include "cloning.h"

using std::deque;
using std::vector;
using Eigen::SparseMatrix;
using Eigen::VectorXd;
using Eigen::BiCGSTAB;
Cloning::Cloning()
{
    
}
static const int dx[] = {-1, 0, 0, 1};
static const int dy[] = {0, -1, 1, 0};

vector<vector<bool> > Cloning::masked(const QImage &source)
{
    deque<int> qx, qy;
    vector<vector<bool> > ret(source.width(), vector<bool>(source.height()));
    qx.push_back(0);qy.push_back(0);
    ret[0][0] = true;
    for(int i = 0;i < source.width();i++)
        for(int j = 0;j < source.height();j++)
            if(source.pixel(i, j) == qRgb(255, 0, 0))
                ret[i][j] = true;

    while(!qx.empty()){
        int x = qx.front(), y = qy.front();
        qx.pop_front();qy.pop_front();
        for(int u = 0;u < 4;u++)
        {
            int xx = x + dx[u], yy = y + dy[u];
            if(xx >= 0 && xx < source.width() && yy >= 0 && yy < source.height() && !ret[xx][yy])
            {
                ret[xx][yy] = true;
                qx.push_back(xx);
                qy.push_back(yy);
            }
        }
    }
    return ret;
}

QImage Cloning::directCloning(const QImage &source, const QImage &background)
{
    auto mask = masked(source);
    QImage ret(background.copy());
    for(int i = 0; i < std::min(background.width(), source.width());i++)
        for(int j = 0;j < std::min(background.height(), source.height());j++)
            if(!mask[i][j])
                ret.setPixel(i, j, source.pixel(i,j));
    return ret;
}

QImage Cloning::seamlessCloning(const QImage &sourceplus, const QImage &background, const QImage &source)
{
    auto mask = masked(sourceplus);    
    vector<vector<int> > number(background.width(), vector<int>(background.height()));
    int n = 0;
    for(int i = 0; i < std::min(background.width(), source.width());i++)
        for(int j = 0;j < std::min(background.height(), source.height());j++)
           if(!mask[i][j])
               number[i][j] = n++;
    
    VectorXd result[3] = {VectorXd(n), VectorXd(n), VectorXd(n)}, b[3] = {VectorXd(n), VectorXd(n), VectorXd(n)};
    SparseMatrix<double> A(n, n);
    BiCGSTAB<SparseMatrix<double> > solver;
    for(int i = 0; i < std::min(background.width(), source.width());i++)
        for(int j = 0;j < std::min(background.height(), source.height());j++)
            if(!mask[i][j]){
                int neighbours = 0;
                QRgb tmp = background.pixel(i, j);
                int pij_b[3] = {qRed(tmp), qGreen(tmp), qBlue(tmp)};
                tmp = source.pixel(i, j);
                int pij_s[3] = {qRed(tmp), qGreen(tmp), qBlue(tmp)};
                for(int k = 0; k < 3;k++)
                    b[k](number[i][j]) = 0;
                for(int u = 0;u < 4;u++)
                {
                    int x = i + dx[u], y = j + dy[u];

                    if(x >= 0 && x < std::min(background.width(), source.width())
                            && y >= 0 && y < std::min(background.height(), source.height()))
                    {
                       neighbours++;
                       tmp = background.pixel(x, y);
                       int pxy_b[3] = {qRed(tmp), qGreen(tmp), qBlue(tmp)};
                       tmp = source.pixel(x, y);
                       int pxy_s[3] = {qRed(tmp), qGreen(tmp), qBlue(tmp)};  
                       
                       if(!mask[x][y]){ // interior
                           A.insert(number[i][j], number[x][y]) = -1;
                       }else{ // boundary
                           for(int k = 0;k < 3;k++)
                               b[k](number[i][j]) += pxy_b[k];
                       }
                       
                       for(int k = 0;k < 3;k++)
                           if(std::abs(pij_b[k] - pxy_b[k]) > std::abs(pij_s[k] - pxy_s[k]))
                               b[k](number[i][j]) += pij_b[k] - pxy_b[k];
                           else 
                               b[k](number[i][j]) += pij_s[k] - pxy_s[k];
                    }
                }
                A.insert(number[i][j], number[i][j]) = neighbours;
            }
    solver.compute(A);
    qDebug() << "Solve..." << endl;
    for(int k = 0;k < 3;k++){
        qDebug() << solver.error() << endl;
        result[k] = solver.solve(b[k]);
    }
    
    QImage ret(background.copy());
    for(int i = 0; i < std::min(background.width(), source.width());i++)
        for(int j = 0;j < std::min(background.height(), source.height());j++)
            if(!mask[i][j]){
                uint8_t p[3];
                for(int k = 0;k < 3;k++){
                    double t = result[k](number[i][j]);
                    p[k] =  t > 255 ? 255 : (t < 0 ? 0 : uint8_t(t)); 
                }
                ret.setPixel(i,j, qRgb(p[0], p[1], p[2]));
            }
    return ret;
}
