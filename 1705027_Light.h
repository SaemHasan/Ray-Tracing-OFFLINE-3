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

    virtual Color getColor(){
        return color;
    }

    virtual void draw(){
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(color.r, color.g, color.b);
        glutSolidSphere(2, 50, 50);
        glPopMatrix();
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
    void drawSquare(double a)
    {
        //glColor3f(1.0,0.0,0.0);
        glBegin(GL_QUADS);{
            glVertex3f( a, a,0);
            glVertex3f( a,-a,0);
            glVertex3f(-a,-a,0);
            glVertex3f(-a, a,0);
        }glEnd();
    }

    void draw(){
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(color.r, color.g, color.b);
        drawSquare(10);
        glPopMatrix();
    }

    Color getColor(){
        
        return color;
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
