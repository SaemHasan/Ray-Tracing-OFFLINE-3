#include "1705027_Point_Color.h"
// Light class starts here =========================================
// Point light class starts here =========================================
class PointLight{
public:
    Point position; // position of the light
    Color color; // color of the light

    // constructors
    PointLight(Point position, Color color){
        this->position = position;
        this->color = color;
    }

    PointLight(){
        position = Point();
        color = Color();
    }

    // to get the color of the light
    virtual Color getColor(Point intersectionPoint){
        return color;
    }
    
    // to draw the light in openGL
    virtual void draw(){
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(color.r, color.g, color.b);
        glutSolidSphere(1, 10, 10);
        glPopMatrix();
    }

    // to print the details of the light
    friend ostream& operator<<(ostream &out, const PointLight &p){
        out<<"PointLight position:"<<endl;
        out<<p.position<<endl;
        out<<"Colors:"<<endl;
        out<<p.color<<endl;
        return out;
    }

    // to read the details of the light from file or console
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
    Point light_direction; // direction of the light
    double cutoff_angle; // cutoff angle of the light

    // constructors
    SpotLight(Point position, Color color, Point light_direction, double cutoff_angle):PointLight(position, color){
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }

    SpotLight():PointLight(){
        light_direction = Point();
        cutoff_angle = 0;
    }

    // draw sqaure in openGL
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

    // to draw a square shape spot light in openGL
    void draw(){
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(color.r, color.g, color.b);
        drawSquare(10);
        glPopMatrix();
    }

    // to get the color of the light if the intersection point is in the light
    Color getColor(Point intersectionPoint){
        light_direction.normalize();
        Point light_to_intersectPoint = intersectionPoint - position;
        light_to_intersectPoint.normalize();
        double angle = acos(light_direction.dot(light_to_intersectPoint));
        angle = rad2deg(angle);

        if(angle > cutoff_angle) 
            return Color(0, 0, 0);
        
        return color;
    }

    // to print the details of the light
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

    // to read the details of the light from file or console
    friend istream& operator>>(istream &in, SpotLight &s){
        in>>s.position;
        in>>s.color;
        in>>s.light_direction;
        in>>s.cutoff_angle;
        return in;
    }
};

// spot light class ends here =========================================
