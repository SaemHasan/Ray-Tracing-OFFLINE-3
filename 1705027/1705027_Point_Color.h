#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<vector>
#include <time.h>
// #include <bits/stdc++.h>


#if __WIN32__
	#include <windows.h>
	#include<glut.h>
#elif __APPLE__
	#include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#define pi (2*acos(0.0))
#define INF 999999999

using namespace std;

double deg2rad(double deg){
    return deg * M_PI / 180.0;
}

double rad2deg(double rad){
    return (rad * 180.0) / M_PI;
}


// variables
int levelsOfRecursion = 0;
int pixelsAlongBothAxis = 0;
int numberOfObjects = 0;
int numberOfPointLights = 0;
int numberOfSpotLights = 0;
int numberOfCapturedImages = 11;

//================================================
// Point Class

class Point{


public:
    double x, y, z; // coordinates
    // constructors
    Point(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(double x, double y){
        this->x = x;
        this->y = y;
        this->z = 0.0;
    }

    // to add two points
    Point operator+(const Point &p){
        Point p1;
        p1.x = x + p.x;
        p1.y = y + p.y;
        p1.z = z + p.z;
        return p1;
    }

    // to subtract two points
    Point operator-(const Point &p){
        Point p1;
        p1.x = x - p.x;
        p1.y = y - p.y;
        p1.z = z - p.z;
        return p1;
    }

    // to multiply a point with a scalar
    Point operator*(const double &d){
        Point p1;
        p1.x = x * d * 1.0;
        p1.y = y * d * 1.0;
        p1.z = z * d * 1.0;
        return p1;
    }

    // to divide a point with a scalar
    Point operator/(const double &d){
        Point p1;
        p1.x = x / d * 1.0;
        p1.y = y / d * 1.0;
        p1.z = z / d * 1.0;
        return p1;
    }

    // copy constructor
    Point operator=(const Point &p){
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    // normalize a point
    Point normalize(){
        double norm = sqrt(x*x + y*y + z*z) * 1.0;
        x /= norm;
        y /= norm;
        z /= norm;
        return *this;
    }

    // negate a point
    Point operator-(){
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    // distance between two points
    double distance(Point p){
        return sqrt((x-p.x)*(x-p.x) *1.0 + (y-p.y)*(y-p.y)*1.0 + (z-p.z)*(z-p.z)*1.0);
    }

    // dot product of two points
    double dot(const Point &p){
        return (x*p.x + y*p.y + z*p.z) * 1.0;
    }

    // cross product of two points
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

    // to print a point
    friend ostream &operator<<( ostream &output, const Point &p ) { 
        output<< p.x << " " << p.y << " " << p.z << endl;
         return output;            
      }

    // to read a point from file or console
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

    // destructor
    ~Point(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
};

// point class ends here =========================================


// Color class starts here =======================================

class Color{
    public:
        double r, g, b; // has to be between 0 and 1 for openGL

        // constructors
        Color(int r, int g, int b){
            this->r = r;
            this->g = g;
            this->b = b;
        }
        Color(){
            r = 0.0;
            g = 0.0;
            b = 0.0;
        }

        // to clip between 0 and 1
        Color clip(){
            if(r > 1.0) r = 1.0;
            if(g > 1.0) g = 1.0;
            if(b > 1.0) b = 1.0;
            if(r < 0.0) r = 0.0;
            if(g < 0.0) g = 0.0;
            if(b < 0.0) b = 0.0;
            return *this;
        }
        
        // to add two colors
        Color operator+(const Color &c){
            Color c1;
            c1.r = r + c.r;
            c1.g = g + c.g;
            c1.b = b + c.b;
            return c1;
        }

        // copy constructor
        Color operator=(const Color &c){
            r = c.r;
            g = c.g;
            b = c.b;
            return *this;
        }

        // to multiply a color with a scalar
        Color operator*(const double &d){
            Color c;
            c.r = r * d * 1.0;
            c.g = g * d * 1.0;
            c.b = b * d * 1.0;
            return c;
        }

        // to multiply two colors
        Color operator*(const Color &c){
            Color c1;
            c1.r = r * c.r * 1.0;
            c1.g = g * c.g * 1.0;
            c1.b = b * c.b * 1.0;
            return c1;
        }
        
        // to take input from the file. 
        friend istream &operator>>(istream  &input, Color &c ) { 
            input >> c.r >> c.g >> c.b;
            return input;            
        }

        // to output the color's details
        friend ostream& operator<<(ostream& os, const Color& c){
            os<<c.r<<" "<<c.g<<" "<<c.b<<endl;
            return os;
        }

        // destructor
        ~Color(){
            r = 0.0;
            g = 0.0;
            b = 0.0;
        }
};


// Color class ends here =========================================
