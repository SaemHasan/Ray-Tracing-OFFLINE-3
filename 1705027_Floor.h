#include "1705027_General.h"
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
        computeAmbientDiffuseSpecularComponent(ray, color, level, t_min, normal, intersect_point, intersectionPointColor);

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
