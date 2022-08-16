#include "1705027_Object.h"
// Ray Class starts here =========================================
class Ray{
    public:
        Point start;
        Point direction;

        Ray(){
            start = Point();
            direction = Point();
        }
        Ray(Point start, Point direction){
            this->start = start;
            this->direction = direction;
            this->direction.normalize();
        }

};

// Ray Class ends here =========================================