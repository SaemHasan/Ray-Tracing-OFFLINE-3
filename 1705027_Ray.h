#include "1705027_Light.h"
// Ray Class starts here =========================================
class Ray{
    public:
        Point r0;//start
        Point rd;//direction

        // constructors
        Ray(){
            r0 = Point();
            rd = Point();
        }
        Ray(Point start, Point direction){
            this->r0 = start; // start point
            this->rd = direction.normalize(); // normalize the direction
        }

};

// Ray Class ends here =========================================