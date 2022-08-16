#include "1705027_Point_Color.h"
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
