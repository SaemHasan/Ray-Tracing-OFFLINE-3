#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<vector>
#include <time.h>

#if __WIN32__
	#include <windows.h>
	#include<glut.h>
#elif __APPLE__
	#include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#define pi (2*acos(0.0))
// #include <bits/stdc++.h>
#define INF 999999999

using namespace std;

double deg2rad(double deg){
    return deg * M_PI / 180.0;
}

// variables
int levelsOfRecursion = 0;
int pixelsAlongBothAxis = 0;
int numberOfObjects = 0;
int numberOfPointLights = 0;
int numberOfSpotLights = 0;

//================================================
// Point Class

class Point{


public:
    double x, y, z;
    Point(){
        x = 0;
        y = 0;
        z = 0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(double x, double y){
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    Point operator+(const Point &p){
        Point p1;
        p1.x = x + p.x;
        p1.y = y + p.y;
        p1.z = z + p.z;
        return p1;
    }

    Point operator-(const Point &p){
        Point p1;
        p1.x = x - p.x;
        p1.y = y - p.y;
        p1.z = z - p.z;
        return p1;
    }

    Point operator*(const double &d){
        Point p1;
        p1.x = x * d;
        p1.y = y * d;
        p1.z = z * d;
        return p1;
    }

    Point operator/(const double &d){
        Point p1;
        p1.x = x / d;
        p1.y = y / d;
        p1.z = z / d;
        return p1;
    }

    Point operator=(const Point &p){
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    Point normalize(){
        double norm = sqrt(x*x + y*y + z*z);
        x /= norm;
        y /= norm;
        z /= norm;
        return *this;
    }

    Point operator-(){
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    double dot(const Point &p){
        return x*p.x + y*p.y + z*p.z;
    }

    Point cross(const Point &p){
        Point p1;
        p1.x = y*p.z - z*p.y;
        p1.y = z*p.x - x*p.z;
        p1.z = x*p.y - y*p.x;
        return p1;
    }

    double length(){
        return sqrt(x*x + y*y + z*z);
    }

    
    friend ostream &operator<<( ostream &output, const Point &p ) { 
        output<< p.x << " " << p.y << " " << p.z << endl;
         return output;            
      }

    friend istream &operator>>( istream  &input, Point &p ) { 
        input >> p.x >> p.y >> p.z;
        return input;            
    }

    static Point rotateVector(Point p, Point ax, double angle){
	    Point r;
	    Point c = ax.cross(p);
        r = p * cos(angle) + c * sin(angle);
	    return r;
    }

    static Point RodriguesFormula(Point x, Point a, double angle){
        angle = deg2rad(angle);
        Point p1 = x * cos(angle);
        Point p2 = a * a.dot(x) * (1 - cos(angle));
        Point p3 = x.cross(a) * sin(angle);
        return p1 + p2 + p3;
    }

    void print(){
        cout<<"( x: "<<x<<", y: "<<y<<", z: "<<z<<")"<<endl;
    }
};

// point class ends here =========================================


// Color class starts here =======================================

class Color{
    public:
        double r, g, b;

        Color(int r, int g, int b){
            this->r = r;
            this->g = g;
            this->b = b;
        }
        Color(){
            r = rand()%256;
            g = rand()%256;
            b = rand()%256;
        }

        Color operator=(const Color &c){
            r = c.r;
            g = c.g;
            b = c.b;
            return *this;
        }

        friend istream &operator>>(istream  &input, Color &c ) { 
            input >> c.r >> c.g >> c.b;
            return input;            
        }

        friend ostream& operator<<(ostream& os, const Color& c){
            os<<c.r<<" "<<c.g<<" "<<c.b<<endl;
            return os;
        }
};


// Color class ends here =========================================


// Object class starts here =======================================

class Object{

public:
    Point reference_point;
    double height, width, length;
    Color color;
    double coefficients[4];// ambient, diffuse, specular, reflection coefficients
    int shine; //exponent term of specular component

    Object(){
        reference_point = Point(0, 0, 0);
        height = 0;
        width = 0;
        length = 0;
        color = Color();
        coefficients[0] = 0;
        coefficients[1] = 0;
        coefficients[2] = 0;
        coefficients[3] = 0;
        shine = 0;
    }

    virtual void draw(){

    }
    void setColor(Color c){
        color = c;
    }
    void setShine(int s){
        shine = s;
    }
    void setCoEfficients(double ambient, double diffuse, double specular, double reflection_coefficient){
        coefficients[0] = ambient;
        coefficients[1] = diffuse;
        coefficients[2] = specular;
        coefficients[3] = reflection_coefficient;
    }

    friend ostream& operator<<(ostream& os, const Object& o){
        os<<"reference_point: "<<o.reference_point<<endl;
        os<<"height width length:"<<endl;
        os<<o.height<<" "<<o.width<<" "<<o.length<<endl;
        os<<"color: "<<o.color<<endl;
        os<<"coefficients: "<<endl;
        os<<o.coefficients[0]<<" "<<o.coefficients[1]<<" "<<o.coefficients[2]<<" "<<o.coefficients[3]<<" "<<o.shine<<endl;
        return os;
    }
};

// Object class ends here =========================================

class Triangle : public Object{

public:
    Point points[3];
    Color color;

    Triangle():Object(){
        points[0] = Point();
        points[1] = Point();
        points[2] = Point();
        color = Color();
    }

    Triangle(Point p1, Point p2, Point p3, Color c){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        color = c;
    }

    Triangle(Point p1, Point p2, Point p3){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        color = Color();
    }

    void draw(){
        // cout<<"Triangle drawing"<<endl;
        glBegin(GL_TRIANGLES);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(points[0].x, points[0].y, points[0].z);
        glVertex3f(points[1].x, points[1].y, points[1].z);
        glVertex3f(points[2].x, points[2].y, points[2].z);
        glEnd();
    }

    friend ostream& operator<<(ostream &out, const Triangle &t){
        out<<"Triangle:"<<endl;
        out<<t.points[0]<<t.points[1]<<t.points[2]<<endl;
        out<<"Colors:"<<endl;
        out<<t.color<<endl;
        out<<"Coefficients:"<<endl;
        out<<t.coefficients[0]<<" "<<t.coefficients[1]<<" "<<t.coefficients[2]<<" "<<t.coefficients[3]<<endl;
        out<<"Shine:"<<endl;
        out<<t.shine<<endl;
        return out;
    }

    friend istream& operator>>(istream &in, Triangle &t){
        in>>t.points[0]>>t.points[1]>>t.points[2];
        in>>t.color;
        in>>t.coefficients[0]>>t.coefficients[1]>>t.coefficients[2]>>t.coefficients[3]>>t.shine;
        return in;
    } 
};

// Triangle class ends here =========================================

// Sphere class starts here =========================================
class Sphere : public Object{
public:
    Sphere():Object(){
        
    }
    Sphere(Point center, double radius){
        reference_point = center;
        length = radius;
    }

    void draw(){
        // cout<<"Sphere drawing"<<endl;
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.r, color.g, color.b);
        glutSolidSphere(length, 20, 20); // check later
        glPopMatrix();
    }

    friend istream& operator>>(istream &in, Sphere &s){
        in>>s.reference_point;
        in>>s.length;
        in>>s.color;
        in>>s.coefficients[0]>>s.coefficients[1]>>s.coefficients[2]>>s.coefficients[3]>>s.shine;
        return in;
    }

    friend ostream& operator<<(ostream &out, const Sphere &s){
        out<<"Sphere:"<<endl;
        out<<s.reference_point<<s.length<<endl;
        out<<"Colors:"<<endl;
        out<<s.color<<endl;
        out<<"Coefficients:"<<endl;
        out<<s.coefficients[0]<<" "<<s.coefficients[1]<<" "<<s.coefficients[2]<<" "<<s.coefficients[3]<<endl;
        out<<"Shine:"<<endl;
        out<<s.shine<<endl;
        return out;
    }

};

// Sphere class end here =========================================

// General Class starts here =========================================

class General : public Object{
public:
    double A, B, C, D, E, F, G, H, I, J;

    General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J):Object(){
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;
    }

    General():Object(){
        A = 0;
        B = 0;
        C = 0;
        D = 0;
        E = 0;
        F = 0;
        G = 0;
        H = 0;
        I = 0;
        J = 0;
    }

    void draw(){}

    friend istream& operator>>(istream &in, General &g){
        in>>g.A>>g.B>>g.C>>g.D>>g.E>>g.F>>g.G>>g.H>>g.I>>g.J;
        in>> g.reference_point;
        in >> g.length >> g.width >> g.height;
        in>>g.color;
        in>>g.coefficients[0]>>g.coefficients[1]>>g.coefficients[2]>>g.coefficients[3]>>g.shine;
        return in;
    }

    friend ostream& operator<<(ostream &out, const General &g){
        out<<"General:"<<endl;
        out<<g.A<<" "<<g.B<<" "<<g.C<<" "<<g.D<<" "<<g.E<<" "<<g.F<<" "<<g.G<<" "<<g.H<<" "<<g.I<<" "<<g.J<<endl;
        out<<g.reference_point<<g.length<<" "<<g.width<<" "<<g.height<<endl;
        out<<"Colors:"<<endl;
        out<<g.color<<endl;
        out<<"Coefficients:"<<endl;
        out<<g.coefficients[0]<<" "<<g.coefficients[1]<<" "<<g.coefficients[2]<<" "<<g.coefficients[3]<<endl;
        out<<"Shine:"<<endl;
        out<<g.shine<<endl;
        return out;
    }
};

// General Class ends here =========================================

// Floor class starts here =========================================

class Floor : public Object{
    public:
    Floor():Object(){
        
    }

    Floor(double floorWidth, double tileWidth){
        reference_point = Point(-floorWidth/2.0, -floorWidth/2.0, 0);
        length = tileWidth;
    }

    void draw(){
        // cout<<"Floor drawing"<<endl;
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.r, color.g, color.b);

        for(int i = 0; i < reference_point.x*2; i += length){
            for(int j = 0; j < reference_point.y*2; j += length){
                // check later condition of if
                if((i+j)%2 == 0){
                    glColor3f(0.0, 0.0, 0.0);
                }
                else{
                    glColor3f(1.0, 1.0, 1.0);
                }
                glBegin(GL_QUADS);
                glVertex3f(i, j, 0);
                glVertex3f(i+length, j, 0);
                glVertex3f(i+length, j+length, 0);
                glVertex3f(i, j+length, 0);
                glEnd();
            }
        }
        glPopMatrix();
    }

    friend istream& operator>>(istream &in, Floor &f){
        in>>f.reference_point;
        in>>f.length;
        in>>f.color;
        in>>f.coefficients[0]>>f.coefficients[1]>>f.coefficients[2]>>f.coefficients[3]>>f.shine;
        return in;
    }

    friend ostream& operator<<(ostream &out, const Floor &f){
        out<<"Floor:"<<endl;
        out<<f.reference_point<<f.length<<endl;
        out<<"Colors:"<<endl;
        out<<f.color<<endl;
        out<<"Coefficients:"<<endl;
        out<<f.coefficients[0]<<" "<<f.coefficients[1]<<" "<<f.coefficients[2]<<" "<<f.coefficients[3]<<endl;
        out<<"Shine:"<<endl;
        out<<f.shine<<endl;
        return out;
    }
};

// Floor class ends here =========================================

// Light class starts here =========================================
// Point light class starts here =========================================
class PointLight{
public:
    Point position;
    Color color;

    PointLight(Point position, Color color){
        this->position = position;
        this->color = color;
    }

    PointLight(){
        position = Point();
        color = Color();
    }

    friend ostream& operator<<(ostream &out, const PointLight &p){
        out<<"PointLight position:"<<endl;
        out<<p.position<<endl;
        out<<"Colors:"<<endl;
        out<<p.color<<endl;
        return out;
    }

    friend istream& operator>>(istream &in, PointLight &p){
        in>>p.position;
        in>>p.color;
        return in;
    }
};

// Point light class ends here =========================================

// Spot light class starts here =========================================
class SpotLight: public PointLight{
public:
    Point light_direction;
    double cutoff_angle;

    SpotLight(Point position, Color color, Point light_direction, double cutoff_angle):PointLight(position, color){
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }

    SpotLight():PointLight(){
        light_direction = Point();
        cutoff_angle = 0;
    }

    friend ostream& operator<<(ostream &out, const SpotLight &s){
        out<<"SpotLight position:"<<endl;
        out<<s.position<<endl;
        out<<"Colors:"<<endl;
        out<<s.color<<endl;
        out<<"Light direction:"<<endl;
        out<<s.light_direction<<endl;
        out<<"Cutoff angle:"<<endl;
        out<<s.cutoff_angle<<endl;
        return out;
    }

    friend istream& operator>>(istream &in, SpotLight &s){
        in>>s.position;
        in>>s.color;
        in>>s.light_direction;
        in>>s.cutoff_angle;
        return in;
    }
};

// spot light class ends here =========================================

// Ray Class starts here =========================================
class Ray{
    public:
        Point start;
        Point direction;

        Ray(){
            start = Point();
            direction = Point();
        }
        Ray(Point start, Point direction){
            this->start = start;
            this->direction = direction;
            this->direction.normalize();
        }

};

