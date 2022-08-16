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
