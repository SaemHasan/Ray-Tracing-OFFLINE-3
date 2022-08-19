#include "1705027_Sphere.h"

// Triangle class starts here =====================================
class Triangle : public Object
{

public:
    Point points[3]; // three points of the triangle
    Color color; // color of the triangle

    // constructor
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

    // to draw the triangle in the openGL window
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

    // to find the intersection point of the ray and the triangle
    double intersect(Ray ray, Color &color, int level){
        double determinantA, determinantBeta, determinantGamma, determinantT;
        double beta, gamma, t;

        Point a,b,c; // three points of the triangle
        a = points[0];
        b = points[1];
        c = points[2];

        // calculate the determinant of the matrix A
        determinantA = (a.x-b.x) * ( (a.y-c.y)*ray.rd.z - (a.z-c.z)*ray.rd.y);
        determinantA += (a.y-b.y) * ( (a.z-c.z)*ray.rd.x - (a.x-c.x)*ray.rd.z);
        determinantA += (a.z-b.z) * ( (a.x-c.x)*ray.rd.y - (a.y-c.y)*ray.rd.x);

        // calculate the determinant of the matrix Beta
        determinantBeta = (a.x-ray.r0.x) * ( (a.y-c.y)*ray.rd.z - (a.z-c.z)*ray.rd.y);
        determinantBeta += (a.y-ray.r0.y) * ( (a.z-c.z)*ray.rd.x - (a.x-c.x)*ray.rd.z);
        determinantBeta += (a.z-ray.r0.z) * ( (a.x-c.x)*ray.rd.y - (a.y-c.y)*ray.rd.x);

        // calculate the determinant of the matrix Gamma
        determinantGamma = (a.x-b.x) * ( (a.y-ray.r0.y)*ray.rd.z - (a.z-ray.r0.z)*ray.rd.y);
        determinantGamma += (a.y-b.y) * ( (a.z-ray.r0.z)*ray.rd.x - (a.x-ray.r0.x)*ray.rd.z);
        determinantGamma += (a.z-b.z) * ( (a.x-ray.r0.x)*ray.rd.y - (a.y-ray.r0.y)*ray.rd.x);

        // calculate the determinant of the matrix T
        determinantT = (a.x-b.x) * ( (a.y-c.y)*(a.z-ray.r0.z) - (a.z-c.z)*(a.y-ray.r0.y));
        determinantT += (a.y-b.y) * ( (a.z-c.z)*(a.x-ray.r0.x) - (a.x-c.x)*(a.z-ray.r0.z));
        determinantT += (a.z-b.z) * ( (a.x-c.x)*(a.y-ray.r0.y) - (a.y-c.y)*(a.x-ray.r0.x));

        // if the determinant of A is zero, then the ray is parallel to the triangle. So, no intersection. Return INF.
        if(determinantA == 0.0){
            t = INF;
        }
        else{
            // calculate beta, gamma and t
            beta = determinantBeta/determinantA;
            gamma = determinantGamma/determinantA;
            // check if there is an intersection            
            if(beta < 0 || gamma < 0 || beta+gamma > 1){
                t = INF; // no intersection
            }
            else{
                t = determinantT/determinantA; // intersection
            }
        }

        // level of reflection is zero. So, only need to return the t value.
        if(level == 0){
            return t;
        }
        // t calculation done

        // light calculation starts
        Point interestionPoint = ray.r0 + ray.rd*t; // intersection point
        Color interestionPointColor = this->color; // color of the intersection point

        Point normal = (b-a).cross(c-a); // normal of the triangle
        normal.normalize(); // normalize the normal

        // check the normal direction
        if(normal.dot(ray.rd) > 0){
            normal = -normal;
        }
        
        // calculate ambient, diffuse and specular components
        computeAmbientDiffuseSpecularComponent(ray, color, level, t, normal, interestionPoint, interestionPointColor);

        // recursive reflection starts
        // if level is equal to level of reflection, then no need to calculate the reflection
        if(level >= levelsOfRecursion)
        {
            return t;
        }
        // otherwise calculate recursive reflection
        recursiveReflection(ray, color, level, t, normal, interestionPoint);
        // recursive reflection ends
        return t;
    }

    // to print the triangle details
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

    // to read the triangle details
    friend istream& operator>>(istream &in, Triangle &t){
        in>>t.points[0]>>t.points[1]>>t.points[2];
        in>>t.color;
        in>>t.coefficients[0]>>t.coefficients[1]>>t.coefficients[2]>>t.coefficients[3]>>t.shine;
        return in;
    }

    // destructor
    ~Triangle(){

    } 
};

// Triangle class ends here =========================================
