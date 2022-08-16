#include "1705027_Light.h"

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

class Triangle : public Object
{

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

        // glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        // glColor3f(color.r, color.g, color.b);

        for(int i = 0; i < reference_point.x*2; i += length){
            for(int j = 0; j < reference_point.y*2; j += length){
                // check later condition of if
                if((int)((i+j)/length)%2 == 0)
                {
                    glColor3f(0.0, 0.0, 0.0);
                }
                else
                {
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


