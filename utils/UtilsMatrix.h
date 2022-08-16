#include "UtilsPoint.h"
#define MATRIX_SIZE 4

class Matrix{

public:
    int rows, cols;

    vector< vector<double> > matrix;

    Matrix(){
        rows = MATRIX_SIZE;
        cols = MATRIX_SIZE;
        setMatrix(rows,cols);
    }

    Matrix(int rows, int cols){
        this->rows = rows;
        this->cols = cols;
        setMatrix(rows,cols);
    }

    void setMatrix(int rows, int cols){
        this->rows = rows;
        this->cols = cols;
        matrix.resize(rows);
        for(int i = 0; i < rows; i++){
            matrix[i].resize(cols);
        }
    }

    void printMatrix(){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    friend ostream &operator<<(ostream &output, const Matrix &m){
        for(int i = 0; i < m.rows; i++){
            for(int j = 0; j < m.cols; j++){
                output<<m.matrix[i][j]<<" ";
            }
            output<<endl;
        }
        return output;
    }

    Matrix operator+(const Matrix &m){
        Matrix m1(rows,cols);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                m1.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
            }
        }
        return m1;
    }

    Matrix operator-(const Matrix &m){
        Matrix m1(rows,cols);
        
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                m1.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
            }
        }
        return m1;
    }

    Matrix operator*(const double &d){
        Matrix m1(rows,cols);
        
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                m1.matrix[i][j] = matrix[i][j] * d;
            }
        }
        return m1;
    }

    Matrix operator*(const Matrix &m){
        Matrix m1(rows,cols);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                m1.matrix[i][j] = 0;
                for(int k = 0; k < rows; k++){
                    m1.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
                }
            }
        }
        return m1;
    }

    Matrix operator=(const Matrix &m){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                matrix[i][j] = m.matrix[i][j];
            }
        }
        return *this;
    }

    Matrix transpose(){
        Matrix m1(cols,rows);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                m1.matrix[j][i] = matrix[i][j];
            }
        }
        return m1;
    }

    static Matrix identityMatrix(){
        int rows = MATRIX_SIZE;
        int cols = MATRIX_SIZE;
        Matrix m1(rows,cols);
        for(int i = 0; i < rows; i++){
            m1.matrix[i][i] = 1;
        }
        return m1;
    }

    static Matrix translationMatrix(Point t){
        Matrix m = identityMatrix();
        m.matrix[0][3] = t.x;
        m.matrix[1][3] = t.y;
        m.matrix[2][3] = t.z;
        return m;
    }

    static Matrix scalingMatrix(Point s){
        Matrix m = identityMatrix();
        m.matrix[0][0] = s.x;
        m.matrix[1][1] = s.y;
        m.matrix[2][2] = s.z;
        return m;
    }

    static Matrix translationMatrix(double tx, double ty, double tz){
        Matrix m = identityMatrix();
        m.matrix[0][3] = tx;
        m.matrix[1][3] = ty;
        m.matrix[2][3] = tz;
        return m;
    }

    static Matrix rotationMatrix(double angle, Point a){
        
        a = a.normalize();

        Point i = Point(1,0,0);
        Point j = Point(0,1,0);
        Point k = Point(0,0,1);

        Point c1 = Point::RodriguesFormula(i, a, angle);
        Point c2 = Point::RodriguesFormula(j, a, angle);
        Point c3 = Point::RodriguesFormula(k, a, angle);

        Matrix m = identityMatrix();

        Point c[] = {c1, c2, c3};

        for(int i = 0; i < 3; i++){
            m.matrix[i][0] = c[i].x;
            m.matrix[i][1] = c[i].y;
            m.matrix[i][2] = c[i].z;
        }

        return m;
    }

    static Matrix rotationMatrix(Point c1, Point c2, Point c3){
        Point c[] = {c1, c2, c3};
        Matrix m = identityMatrix();
        for(int i = 0; i < 3; i++){
            m.matrix[i][0] = c[i].x;
            m.matrix[i][1] = c[i].y;
            m.matrix[i][2] = c[i].z;
        }
        return m;
    }

    static Matrix projectionMatrix(double fovY, double aspectRatio, double near, double far){
        double fovX = fovY * aspectRatio;
        
        double t = near * tan(deg2rad(fovY/2.0));//t = near * tan(fovY/2)
        double r = near * tan(deg2rad(fovX/2.0));//r = near * tan(fovX/2)
        
        Matrix m = identityMatrix();
        m.matrix[0][0] = near/r;
        m.matrix[1][1] = near/t;
        m.matrix[2][2] = -(far+near)/(far-near);
        m.matrix[2][3] = -(2*far*near)/(far-near);
        m.matrix[3][2] = -1;
        m.matrix[3][3] = 0;
        return m;
    }

    static Matrix pointMatrix(Point p){
        Matrix m(4,1);
        m.matrix[0][0] = p.x;
        m.matrix[1][0] = p.y;
        m.matrix[2][0] = p.z;
        m.matrix[3][0] = 1;
        return m;
    }

    Point getPoint(){
        Point p(matrix[0][0], matrix[1][0], matrix[2][0]);
        if(matrix[3][0] != 0){
            p = p/matrix[3][0];
        }
        return p;
    }

    ~Matrix(){
        for(int i = 0; i < rows; i++){
            vector<double>().swap(matrix[i]);
        }
        vector< vector<double> >().swap(matrix);
    }

};

