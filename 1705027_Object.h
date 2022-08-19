#include "1705027_Ray.h"

#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

Point pos, u, r, l; // pos = camera position, r = right, l = look, u = up

bool verbose = false;

class Object; // forward declaration
class PointLight; // forward declaration
//vectors
vector<Object*> objects; // vector of objects
vector<PointLight*> lights; // vector of lights


// Object class starts here =======================================

class Object{

public:
    Point reference_point; // reference point
    double height, width, length;// height, width, length
    Color color; // color of the object
    double coefficients[4];// ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component

    // constructors
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

    // draw the object in openGL. This is a virtual function. So, it will be overriden in the derived classes
    virtual void draw(){

    }

    // intersect the ray with the object. This is a virtual function. So, it will be overriden in the derived classes
    virtual double intersect(Ray ray, Color &color, int level){
        return -1.0;
    }

    // to set color of the object
    void setColor(Color c){
        color = c;
    }
    // to set shine of the object
    void setShine(int s){
        shine = s;
    }
    // to set coefficients of the object
    void setCoEfficients(double ambient, double diffuse, double specular, double reflection_coefficient){
        coefficients[0] = ambient;
        coefficients[1] = diffuse;
        coefficients[2] = specular;
        coefficients[3] = reflection_coefficient;
    }

    // to calculate ambient component, diffuse component and specular component
    // this method will be called in the intersect method of the derived classes
    void calcLightPart(Ray ray, Color &color, int level, double t_neg, Point normal, Point intersect_point,Color intersectionPointColor){

        // ambient component
        color = intersectionPointColor * coefficients[AMBIENT];
        

        // diffuse and specular component
        for(int i=0; i< lights.size();i++){
            Point light_position = lights[i]->position; // position of the light
            // for point light ==> we will get the color of the light
            // for spot light ==> we will have to check if the point is in the spotlight or not. 
            // if it is not in the spotlight, then Color(0,0,0) will be returned
            Color light_color = lights[i]->getColor(intersect_point); // color of the light
            
            Point light_dir = intersect_point - light_position; // direction of the light
            light_dir.normalize(); // normalize the direction

            // shadow check
            // distance between the light and the intersection point
            double light_distance = light_position.distance(intersect_point);
            // construct a ray from the light to the intersection point 
            Ray light_ray(light_position, light_dir);
            
            // check if the ray intersects with any object
            bool shadow = false;
            double eps = 0.000001;

            double t_min = INF,t;
            // find the nearest intersection point
            for(int j=0; j<objects.size(); j++){
                Color dummyColor;
                t = objects[j]->intersect(light_ray, dummyColor, 0);
                if(t>0.0 && t_min>t){
                    t_min = t;
                }
            }

            // if the nearest intersection point is not the initial intersection point, then there is a shadow. 
            if(t_min < light_distance-eps){
                // cout<<"shadow\n";
                shadow = true;
                // if there is a shadow, then the diffuse and specular component will be zero. So, we will not calculate them.
            }

            // if there is no shadow, then calculate the diffuse and specular component
            if(!shadow){
                // cout<<"no shadow"<<endl;
                // diffuse component
                double diffuse = normal.dot(-light_ray.rd) * 1.0;

                // only need to calculate the diffuse component if it is positive
                if(diffuse>0.0){
                    color = color + intersectionPointColor * light_color * coefficients[DIFFUSE] * diffuse;
                }

                // specular component
                Point ref_ray_dir = light_ray.rd - normal * ((normal.dot(light_ray.rd)) * 2.0);
                double specular = ref_ray_dir.dot(ray.rd);
                // only need to calculate the specular component if it is positive
                if(specular>0.0){
                    specular = pow(specular, shine);
                    color = color + intersectionPointColor * light_color * coefficients[SPECULAR] * specular;
                }
            }
        }
    }


    void recursiveReflection(Ray ray, Color &color, int level, double t_neg, Point normal, Point intersect_point){
        // get reflection direction
        Point reflection_dir = ray.rd - normal * (ray.rd.dot(normal) * 2.0);
        reflection_dir.normalize();
        // construct a ray from the intersection point to the reflection direction.
        // slightly forward from the intersection point (by moving the start a little bit 
        // towards the reflection direction) to avoid self intersection. that's why we are adding reflection_dir to the intersect_point.
        Ray reflection_ray(intersect_point+reflection_dir, reflection_dir);

        // check if the ray intersects with any object
        int nearest_object = -1;
        double tMin = INF, t;

        for(int i=0; i<objects.size(); i++){
            Color tempColor;
            t = objects[i]->intersect(reflection_ray, tempColor, 0);
            if(t>0.0 && t<tMin){
                tMin = t;
                nearest_object = i;
            }
        }

        // if the ray intersects with an object, then calculate the color of the intersection point
        Color reflected_color;
        if(nearest_object != -1){
            tMin = objects[nearest_object]->intersect(reflection_ray, reflected_color, level+1);
        }
        // reflected color will be updated while in the subsequent call
        // update color using the impact of reflection
        color = color + reflected_color * coefficients[REFLECTION];
        // clip the color values
        color.clip();
    }

    // to print the object details in the console or in a file
    friend ostream& operator<<(ostream& os, const Object& o){
        os<<"reference_point: "<<o.reference_point<<endl;
        os<<"height width length:"<<endl;
        os<<o.height<<" "<<o.width<<" "<<o.length<<endl;
        os<<"color: "<<o.color<<endl;
        os<<"coefficients: "<<endl;
        os<<o.coefficients[0]<<" "<<o.coefficients[1]<<" "<<o.coefficients[2]<<" "<<o.coefficients[3]<<" "<<o.shine<<endl;
        return os;
    }

    // destructor
    ~Object(){
        length = 0;
        width = 0;
        height = 0;
        shine = 0;
        coefficients[0] = coefficients[1] = coefficients[2] = coefficients[3] = 0;
    }
};

// Object class ends here =========================================
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
        calcLightPart(ray, color, level, t, normal, interestionPoint, interestionPointColor);

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
        
        double distance = normal.distance(reference_point); // distance from the center of the sphere
        // if distance is less than radius, then the point is inside the sphere
        if(distance < length){
            normal = - normal;
            normal.normalize(); 
            cout<<" here in -normal\n";           
        }
        Color intersectionPointColor = this->color; // color of the intersection point

        // calculate ambient, diffuse and specular part
        calcLightPart(ray, color, level, t_neg, normal, intersect_point, intersectionPointColor);
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
        calcLightPart(ray, color, level, t_neg, normal, intersect_point, intersectionPointColor);
        
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

// Floor class starts here =========================================

class Floor : public Object{
    // private members
    double floorWidth, tileWidth;
    Color tileColors[2];
    
    public:
    // constructor
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

    // to draw the floor in the openGL window
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

    // to check if the ray intersects with the floor
    double intersect(Ray ray, Color &color, int level){
        Point normal = Point(0,0,1.0);

        if (pos.dot(normal) < 0.0){
            normal = -normal;
        }

        double t_min = INF;

        if(ray.rd.dot(normal)!=0.0){
            t_min = - (normal.dot(ray.r0)/ray.rd.dot(normal));
        }

        if(t_min < INF && t_min > 0.0){
            Point intersect_point = ray.r0 + ray.rd*t_min;
            if(intersect_point.x >= reference_point.x && intersect_point.x <= reference_point.x+floorWidth && intersect_point.y >= reference_point.y && intersect_point.y <= reference_point.y+floorWidth){
                // intersection point is within the floor
            }
            else{
                // intersection point is outside the floor
                t_min = INF;
            }
        }

        // return the minimum t value
        if(level == 0){
            return t_min;
        }
        // t calc done

        //light caculation
        Point intersect_point = ray.r0 + ray.rd*t_min;
        Color intersectionPointColor;
        Point floorPoint = intersect_point - reference_point;
        int m = floor(floorPoint.x/tileWidth);
        int n = floor(floorPoint.y/tileWidth);

        // to check if the intersection point is in a black tile or a white tile
        if((m+n)%2 == 0){
            intersectionPointColor = tileColors[0];
        }
        else{
            intersectionPointColor = tileColors[1];
        }
        // ambient, diffuse and specular calculation
        calcLightPart(ray, color, level, t_min, normal, intersect_point, intersectionPointColor);

        // recursive reflection starts
        if(level >= levelsOfRecursion)
        {
            return t_min;
        }
        // reflection calculation
        recursiveReflection(ray, color, level, t_min, normal, intersect_point);

        return t_min;
    }

    // to take input from the input file
    friend istream& operator>>(istream &in, Floor &f){
        in>>f.reference_point;
        in>>f.length;
        in>>f.color;
        in>>f.coefficients[0]>>f.coefficients[1]>>f.coefficients[2]>>f.coefficients[3]>>f.shine;
        return in;
    }

    // to print the floor details
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
    // destructor
    ~Floor(){
        floorWidth = tileWidth = 0.0;
    }
};

// Floor class ends here =========================================


