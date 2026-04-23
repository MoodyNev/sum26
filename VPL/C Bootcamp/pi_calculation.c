#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "pi_calculation.h"

/* fill the structure with random values between -1 and 1 */
/* the definition of the structure is located in the pi_calculation.h */
void random_coordinates(struct coordinates *coords) {
    return;
}

/* return true if the point given in argument is inside the circle 
of center (0,0) and radius 1” */
bool is_inside_circle(struct coordinates *coords) {
    return false;
}

/* 
returns the pi calculation approximation after n iterations,
the monte carlo method for pi calculation works as follow:
    - draw n random points with their coordinates between -1 and  1 (you should use your random_coordinates function)
    - calculte the ratio of points inside the circle over the total number of points
    - multiply the ratio by 4 and return this value
*/
double monte_carlo_method(double n) {
    return 0.;
}