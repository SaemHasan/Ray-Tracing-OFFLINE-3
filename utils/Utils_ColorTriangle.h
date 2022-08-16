#include "UtilsMatrix.h"
#include "bitmap_image.hpp"
#include<time.h>



class Color{
    public:
        int r, g, b;
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

        friend ostream& operator<<(ostream& os, const Color& c){
            os<<c.r<<" "<<c.g<<" "<<c.b<<endl;
            return os;
        }
};


class Line{
    public:
        Point p1, p2;

        Line(){
            p1 = Point(0,0,0);
            p2 = Point(0,0,0);
        }

        Line(Point p1, Point p2){
            this->p1 = p1;
            this->p2 = p2;
        }

        double getSlope(){
            if(p1.x == p2.x){
                //cout<<"Error: Line is vertical"<<endl;
                return INF;
            }
            else if(p1.y == p2.y){
                //cout<<"Error: Line is horizontal"<<endl;
                return 0;
            }
            else{
                return (p2.y - p1.y)/(p2.x - p1.x);
            }
        }

        double getX(double y){
            double m = getSlope();
            if(m == INF){
                // cout<<"Error: Line is vertical"<<endl;
                // cout << p1 << p2;
                return p1.x;
            }
            else if(m == 0){
                // cout<<"Error: Line is horizontal"<<endl;
                // cout << p1 << p2;
                return 0;
            }
            else{
                return (y - p1.y)/m + p1.x;
            }
        }

};

class Triangle{

public:
    Point points[3];
    Color color;
    Line line12, line23, line31;
    Point pMax, pMin, pMid;

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

    Triangle(){
        points[0] = Point();
        points[1] = Point();
        points[2] = Point();
        color = Color();
    }

    void sortPointsAccordingToY(){
        if(points[0].y > points[1].y && points[0].y > points[2].y){
            pMax = points[0];
            if(points[1].y > points[2].y){
                pMid = points[1];
                pMin = points[2];
            }
            else if(points[1].y < points[2].y){
                pMid = points[2];
                pMin = points[1];
            }
            else{
                if(points[1].x > points[2].x){
                    pMid = points[1];
                    pMin = points[2];
                }
                else{
                    pMid = points[2];
                    pMin = points[1];
                }
            }
        }
        else if(points[1].y > points[0].y && points[1].y > points[2].y){
            pMax = points[1];
            if(points[0].y > points[2].y){
                pMid = points[0];
                pMin = points[2];
            }
            else if(points[0].y < points[2].y){
                pMid = points[2];
                pMin = points[0];
            }
            else{
                if(points[0].x > points[2].x){
                    pMid = points[0];
                    pMin = points[2];
                }
                else{
                    pMid = points[2];
                    pMin = points[0];
                }
            }
        }
        else if(points[2].y > points[0].y && points[2].y > points[1].y){
            pMax = points[2];
            if(points[0].y > points[1].y){
                pMid = points[0];
                pMin = points[1];
            }
            else if(points[0].y < points[1].y){
                pMid = points[1];
                pMin = points[0];
            }
            else{
                if(points[0].x > points[1].x){
                    pMid = points[0];
                    pMin = points[1];
                }
                else{
                    pMid = points[1];
                    pMin = points[0];
                }
            }
        }
        else{
            if(points[0].y == points[1].y && points[0].y > points[2].y){
                if(points[0].x > points[1].x){
                    pMax = points[0];
                    pMid = points[1];
                    pMin = points[2];
                }
                else{
                    pMax = points[1];
                    pMid = points[0];
                    pMin = points[2];
                }
            }
            else if(points[0].y == points[2].y && points[0].y > points[1].y){
                if(points[0].x > points[2].x){
                    pMax = points[0];
                    pMid = points[2];
                    pMin = points[1];
                }
                else{
                    pMax = points[2];
                    pMid = points[0];
                    pMin = points[1];
                }
            }
            else if(points[1].y == points[2].y && points[1].y > points[0].y){
                if(points[1].x > points[2].x){
                    pMax = points[1];
                    pMid = points[2];
                    pMin = points[0];
                }
                else{
                    pMax = points[2];
                    pMid = points[1];
                    pMin = points[0];
                }
            }
        }

        if(pMid.length() ==0 && pMin.length() == 0 && pMax.length() == 0){
            cout<<"Error: in sorting points"<<endl;
        }
    }


    void prepareLines(){
        sortPointsAccordingToY();
        line12 = Line(pMax, pMid);
        line23 = Line(pMid, pMin);
        line31 = Line(pMin, pMax);
    }

    double max_y(){
        double max = points[0].y;
        for(int i=1; i<3; i++){
            if(points[i].y > max){
                max = points[i].y;
            }
        }
        return max;
    }

    double min_y(){
        double min = points[0].y;
        for(int i=1; i<3; i++){
            if(points[i].y < min){
                min = points[i].y;
            }
        }
        return min;
    }

    double max_x(){
        double max = points[0].x;
        for(int i=1; i<3; i++){
            if(points[i].x > max){
                max = points[i].x;
            }
        }
        return max;
    }

    double min_x(){
        double min = points[0].x;
        for(int i=1; i<3; i++){
            if(points[i].x < min){
                min = points[i].x;
            }
        }
        return min;
    }

    double min_z(){
        double min = points[0].z;
        for(int i=1; i<3; i++){
            if(points[i].z < min){
                min = points[i].z;
            }
        }
        return min;
    }

    friend ostream& operator<<(ostream &out, Triangle &t){
        out<<"Triangle:"<<endl;
        out<<t.points[0]<<t.points[1]<<t.points[2]<<endl;
        out<<"Colors:"<<endl;
        out<<t.color<<endl;
        return out;
    }

    friend istream& operator>>(istream &in, Triangle &t){
        in>>t.points[0]>>t.points[1]>>t.points[2];
        return in;
    } 
};

