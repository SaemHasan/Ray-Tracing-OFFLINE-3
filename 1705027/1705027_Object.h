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
    void computeAmbientDiffuseSpecularComponent(Ray ray, Color &color, int level, double t_neg, Point normal, Point intersect_point,Color intersectionPointColor){

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
            if(t_min < light_distance-eps || light_ray.rd.dot(-normal)<=0.0){
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
