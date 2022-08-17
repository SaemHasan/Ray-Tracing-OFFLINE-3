#include "1705027_Light.h"
// Ray Class starts here =========================================
class Ray{
    public:
        Point r0;//start
        Point rd;//direction

        Ray(){
            r0 = Point();
            rd = Point();
        }
        Ray(Point start, Point direction){
            this->r0 = start;
            this->rd = direction;
            this->rd.normalize();
        }

};

// Ray Class ends here =========================================