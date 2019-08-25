
#include "visualizerClass.hpp"
#include <chrono>
#include <iostream>
#include <vector>

void fillVec(std::vector<std::vector<pnt3D>> &vect){

    pnt3D point;

    vect[0].push_back(point);

    vect[1].push_back(point.newData( 1, 1, 1));
    vect[1].push_back(point.newData( 1, 1,-1));
    vect[1].push_back(point.newData(-1, 1,-1));
    vect[1].push_back(point.newData(-1, 1, 1));

    vect[2].push_back(point.newData( 1,-1, 1));
    vect[2].push_back(point.newData( 1,-1,-1));
    vect[2].push_back(point.newData(-1,-1, 1));
    vect[2].push_back(point.newData(-1,-1,-1));
}

int main(void)
{   
    float arr[12][3] = {
        { 0,  0,  0},
        { 1,  1, -1},
        { 1,  1,  1},
        { 1, -1, -1},
        { 1, -1,  1},
        {-1,  1, -1},
        {-1,  1,  1},
        {-1, -1, -1},
        {-1, -1,  1}
    };

    std::vector<std::vector<pnt3D>> vec(3);
    fillVec(vec);

    visualizerClass obj;
    obj.open_window();

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        obj.send_data_as_vector(vec);
        //obj.send_data_as_array(9, (&arr)[0][0]);
    }

	return 0;
}

