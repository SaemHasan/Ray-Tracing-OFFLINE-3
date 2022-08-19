#include "1705027_Triangle.h"
// General Class starts here =========================================

class General : public Object{
public:
    double A, B, C, D, E, F, G, H, I, J; // coefficients of the general equation

    // constructor
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
        A = B = C = D = E = F = G = H = I = J = 0.0;
    }

    // no need to draw the general equation
    void draw(){}

    // to calculate the intersection of the ray with the general equation
    double intersect(Ray ray, Color &color, int level){
        // want to compute ray general quadric surface intersection
        // general quadric surface equation is Ax^2 + By^2 + Cz^2 + Dxy + Eyz + Fxz + Gx + Hy + Iz + J = 0
        // ray equation is P = r0 + t*rd

        // for tmin
        double a, b, c;
        double t_pos, t_neg;
        
        Point dir = ray.rd;
        dir.normalize();

        Point origin = ray.r0;
        
        // calculate a, b, c
        a = A*dir.x*dir.x + B*dir.y*dir.y + C*dir.z*dir.z + D*dir.x*dir.y + E*dir.y*dir.z + F*dir.x*dir.z;
        b = 2.0*(A*origin.x*dir.x + B*origin.y*dir.y + C*origin.z*dir.z + D*origin.x*dir.y + D*origin.y*dir.x + E*origin.y*dir.z + E*origin.z*dir.y + F*origin.x*dir.z + F*origin.z*dir.x + G*dir.x + H*dir.y + I*dir.z);
        c = A*origin.x*origin.x + B*origin.y*origin.y + C*origin.z*origin.z + D*origin.x*origin.y + E*origin.y*origin.z + F*origin.x*origin.z + G*origin.x + H*origin.y + I*origin.z + J;

        if(a == 0.0){
            if(b!=0.0)
                t_neg = (-c)/b;
            else
                t_neg = INF;
            t_pos = INF;
        }
        else{
            double d_sq = (b*b - 4.0*a*c); // b^2 - 4ac
            if(d_sq<0.0){
                t_neg = INF;
                t_pos = INF;
            }
            else{
                if(d_sq>0.0){
                    // cout<<"d_sq: "<<d_sq<<endl;
                    t_pos = (-b+sqrt(d_sq))/(2.0*a);
                    t_neg = (-b-sqrt(d_sq))/(2.0*a);
                    if(t_neg<0.0){
                        t_neg = t_pos;
                    }
                }
                else{
                    t_neg = - b / (2.0*a);
                    t_pos = INF;
                }
            }
        }

        // clip general quadric surface
        Point intersect_point = ray.r0 + ray.rd * t_neg;
        intersect_point = intersect_point - reference_point;
        double x = intersect_point.x;
        double y = intersect_point.y;
        double z = intersect_point.z;

        // if the point is outside the general quadric surface, then return infinity
        // boundary box is assumed to be in the both sides of the floor
        // remove abs from the below condition if you want to check only the positive side of the floor
        if(abs(x)>length || abs(y)>width || abs(z)>height){
            t_neg = INF;
        }
        
        // need to check for t_max also if t_min is infinity
        if(t_neg == INF){
            intersect_point = ray.r0 + ray.rd * t_pos;
            intersect_point = intersect_point - reference_point;
            x = intersect_point.x;
            y = intersect_point.y;
            z = intersect_point.z;

            if(abs(x)>length || abs(y)>width || abs(z)>height){
                t_pos = INF;
            }
            else{
                t_neg = t_pos;
            }
        }

        // return for level 0
        if (level == 0)
        {
            return t_neg;
        }

        // calculate normal
        // to get the normal, we need to differentiate the general equation
        // need to calculate the partial derivatives. dx, dy, dz
        Point normal = Point(2.0*A*x + D*y + F*z + G, 2.0*B*y + D*x + E*z + H, 2.0*C*z + E*y + F*x + I);
        normal.normalize(); // normalize the normal

        // calculate color
        Color intersectionPointColor = this->color; // color of the intersection point

        // calculate ambient, diffuse and specular
        computeAmbientDiffuseSpecularComponent(ray, color, level, t_neg, normal, intersect_point, intersectionPointColor);
        
        // light part done
        
        // recursive reflection starts
        
        if(level >= levelsOfRecursion)
        {
            return t_neg;
        }
        recursiveReflection(ray, color, level, t_neg, normal, intersect_point);

        return t_neg;
    }

    // to take input from the file or the user
    friend istream& operator>>(istream &in, General &g){
        in>>g.A>>g.B>>g.C>>g.D>>g.E>>g.F>>g.G>>g.H>>g.I>>g.J;
        in>> g.reference_point;
        in >> g.length >> g.width >> g.height;
        in>>g.color;
        in>>g.coefficients[0]>>g.coefficients[1]>>g.coefficients[2]>>g.coefficients[3]>>g.shine;
        if(g.length==0.0) g.length = INF; // if length is 0, then set it to infinity. This is done to be able to clip the general quadric surface.
        if(g.width==0.0) g.width = INF; // if width is 0, then set it to infinity
        if(g.height==0.0) g.height = INF; // if height is 0, then set it to infinity
        return in;
    }

    // to print the general equation details
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
    // destructor
    ~General(){
        A = B = C = D = E = F = G = H = I = J = 0.0;
    }
};

// General Class ends here =========================================
