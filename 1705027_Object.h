#include "1705027_Ray.h"

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
Point pos, u, r, l;

bool verbose = false;

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

    virtual double intersect(Ray ray, Color &color, int level){
        return -1.0;
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
// Triangle class starts here =====================================
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
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);{
        glVertex3f(points[0].x, points[0].y, points[0].z);
        glVertex3f(points[1].x, points[1].y, points[1].z);
        glVertex3f(points[2].x, points[2].y, points[2].z);
        }
        glEnd();
    }

    double intersect(Ray ray, Color &color, int level){
        double determinantA, determinantBeta, determinantGamma, determinantT;
        double beta, gamma, t;

        Point a,b,c;
        a = points[0];
        b = points[1];
        c = points[2];

        determinantA = (a.x-b.x) * ( (a.y-c.y)*ray.rd.z - (a.z-c.z)*ray.rd.y);
        determinantA += (a.y-b.y) * ( (a.z-c.z)*ray.rd.x - (a.x-c.x)*ray.rd.z);
        determinantA += (a.z-b.z) * ( (a.x-c.x)*ray.rd.y - (a.y-c.y)*ray.rd.x);

        determinantBeta = (a.x-ray.r0.x) * ( (a.y-c.y)*ray.rd.z - (a.z-c.z)*ray.rd.y);
        determinantBeta += (a.y-ray.r0.y) * ( (a.z-c.z)*ray.rd.x - (a.x-c.x)*ray.rd.z);
        determinantBeta += (a.z-ray.r0.z) * ( (a.x-c.x)*ray.rd.y - (a.y-c.y)*ray.rd.x);

        determinantGamma = (a.x-b.x) * ( (a.y-ray.r0.y)*ray.rd.z - (a.z-ray.r0.z)*ray.rd.y);
        determinantGamma += (a.y-b.y) * ( (a.z-ray.r0.z)*ray.rd.x - (a.x-ray.r0.x)*ray.rd.z);
        determinantGamma += (a.z-b.z) * ( (a.x-ray.r0.x)*ray.rd.y - (a.y-ray.r0.y)*ray.rd.x);

        determinantT = (a.x-b.x) * ( (a.y-c.y)*(a.z-ray.r0.z) - (a.z-c.z)*(a.y-ray.r0.y));
        determinantT += (a.y-b.y) * ( (a.z-c.z)*(a.x-ray.r0.x) - (a.x-c.x)*(a.z-ray.r0.z));
        determinantT += (a.z-b.z) * ( (a.x-c.x)*(a.y-ray.r0.y) - (a.y-c.y)*(a.x-ray.r0.x));

        if(determinantA == 0.0){
            t = INF;
        }
        else{
            beta = determinantBeta/determinantA;
            gamma = determinantGamma/determinantA;            
            if(beta < 0 || gamma < 0 || beta+gamma > 1){
                t = INF;
            }
            else{
                t = determinantT/determinantA;
            }
        }

        if(level == 0){
            return t;
        }
        return t;//delete later
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

    void drawSphere(double radius,int slices,int stacks)
    {
        Point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }
    }

    void draw(){
        // cout<<"Sphere drawing"<<endl;
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.r, color.g, color.b);
        drawSphere(length, 70, 70);
        glPopMatrix();
    }

    double intersect(Ray ray, Color &color, int level){
        double a, b, c;

        double t_pos, t_neg;

        Point dir = ray.rd;
        Point origin = ray.r0 - reference_point;
        
        a = dir.dot(dir);
        b = origin.dot(dir) * 2.0;
        c = origin.dot(origin) - length*length;


        
        double d_sq = (b*b - 4*a*c); // b^2 - 4ac

        
        if(verbose){
            cout<<"dir : "<<dir;
            cout<<"origin: ";  
            cout << origin;
            cout<<"a: "<<a<<" b: "<<b<<" c: "<<c<<endl;
            cout<<"d_sq: "<<d_sq<<endl;
            verbose = false;
        }

        if(d_sq<0.0){
            t_neg = INF;
        }
        else if(d_sq>0.0){
            // cout<<"d_sq: "<<d_sq<<endl;
            t_pos = (-b+sqrt(d_sq))/(2.0*a);
            t_neg = (-b-sqrt(d_sq))/(2.0*a);
            if(t_neg<0.0){
                t_neg = t_pos;
            }
        }
        else{
            t_neg = - b / (2.0*a);
        }

        if (level == 0)
        {
            return t_neg;
        }
        return t_neg;//delete it later
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

    double intersect(Ray ray, Color &color, int level){
        return -1.0;
    }

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
    double floorWidth, tileWidth;
    Color tileColors[2];
    public:
    Floor():Object(){
        floorWidth = 0;
        tileWidth = 0;
        tileColors[0] = Color(0, 0, 0);
        tileColors[1] = Color(1, 1, 1);
    }

    Floor(double floorWidth, double tileWidth){
        reference_point = Point(-floorWidth/2.0, -floorWidth/2.0, 0);
        length = tileWidth;
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        tileColors[0] = Color(0, 0, 0);
        tileColors[1] = Color(1, 1, 1);
    }

    void draw(){
        // cout<<"Floor drawing"<<endl;
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z); 
        for(int i=0; i<floorWidth/tileWidth; i++){
            for(int j=0; j<floorWidth/tileWidth; j++){
                if((i+j)%2 == 0){
                    glColor3f(tileColors[0].r, tileColors[0].g, tileColors[0].b);
                }
                else{
                    glColor3f(tileColors[1].r, tileColors[1].g, tileColors[1].b);
                }
                glBegin(GL_QUADS);{
                    glVertex3f(i*tileWidth, j*tileWidth, 0);
                    glVertex3f(i*tileWidth, (j+1)*tileWidth, 0);
                    glVertex3f((i+1)*tileWidth, (j+1)*tileWidth, 0);
                    glVertex3f((i+1)*tileWidth, j*tileWidth, 0);
                }
                glEnd();
            }
        }

        glPopMatrix();
    }

    double intersect(Ray ray, Color &color, int level){
        return INF;
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


