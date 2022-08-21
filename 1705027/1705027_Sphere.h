
#include "1705027_Object.h"

// Sphere class starts here =========================================
class Sphere : public Object{
    
public:
    // constructor
    Sphere():Object(){
        
    }
    // constructor with parameters
    Sphere(Point center, double radius){
        reference_point = center;
        length = radius;
    }

    // draw sphere in openGL
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

    // to draw the sphere in openGL
    void draw(){
        // cout<<"Sphere drawing"<<endl;
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.r, color.g, color.b);
        drawSphere(length, 70, 70);
        glPopMatrix();
    }

    // to calculate the intersection of the ray with the sphere
    double intersect(Ray ray, Color& color, int level)
    {
        // for tmin
        double a, b, c;
        double t_pos, t_neg;

        Point dir = ray.rd; // direction of the ray
        // dir.normalize();
        Point origin = ray.r0 - reference_point; // origin of the ray
        
        a = dir.dot(dir);
        b = origin.dot(dir) * 2.0;
        c = origin.dot(origin) - length*length;
        
        double d_sq = (b*b - 4.0*a*c); // b^2 - 4ac

        // if d_sq is less than zero, then no intersection
        if(d_sq<0.0){
            t_neg = INF;
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
        }
        }

        // level is zero, then return the t value
        if (level == 0)
        {
            return t_neg;
        }
        // t calculation done

        // for light part
        Point intersect_point = ray.r0 + ray.rd * t_neg;// intersection point

        //calculate normal
        Point normal = intersect_point - reference_point; // normal
        normal.normalize(); // normalize the normal
        
        double distance = pos.distance(reference_point); // distance from the center of the sphere
        // if distance is less than radius, then the point is inside the sphere
        if(distance < length){
            normal = - normal;
            normal.normalize(); 
            cout<<" here in -normal\n";           
        }
        Color intersectionPointColor = this->color; // color of the intersection point

        // calculate ambient, diffuse and specular part
        computeAmbientDiffuseSpecularComponent(ray, color, level, t_neg, normal, intersect_point, intersectionPointColor);
        // light part done
        
        // recursive reflection starts
        if(level >= levelsOfRecursion)
        {
            return t_neg;
        }
        // recursive reflection
        recursiveReflection(ray, color, level, t_neg, normal, intersect_point);
        
        return t_neg;
    }

    // to read the sphere details
    friend istream& operator>>(istream &in, Sphere &s){
        in>>s.reference_point;
        in>>s.length;
        in>>s.color;
        in>>s.coefficients[0]>>s.coefficients[1]>>s.coefficients[2]>>s.coefficients[3]>>s.shine;
        return in;
    }

    // to print the sphere details
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

    // destructor
    ~Sphere(){
        // cout<<"Sphere destroyed"<<endl;
    }

};

// Sphere class end here =========================================
