#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>
#include "Vector3.h"


void testVector3() {
    Vector3 v1(1, 2, 3);
    Vector3 v2(4, 5, 6);
    
    std::cout << "Testing Vector3 class:" << std::endl;
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "Distance between v1 and v2: " << v1.distanceTo(v2) << std::endl;
}


int main() {
    testVector3();
    return 0;
}
