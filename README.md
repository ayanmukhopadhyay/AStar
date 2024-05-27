# Motion Planning
## Installation
```
chmod 777 install_c_toolkits.sh
source install_c_toolkits.sh
```


## NavMesh with 3D A*
### Vector3.h
defines 3D vector <br>
test_Vector3.cpp contains code to test its functions <br>
To test functions in Vector3.h:
```
g++ -o test_Vector3 test_Vector3.cpp
./test_Vector3
```

### Polygon.h
decomposing the walkable surfaces of the environment into convex polygons <br>
test_Polygon.cpp contains code to test its functions <br>
To test functions in Polygon.h:
```
g++ -o test_Polygon test_Polygon.cpp
./test_Polygon
```

### NavMesh.h
a navmesh for motion planning using 3D version A* <br>
test_NavMesh.cpp contains code to test its functions <br>
To test functions in Polygon.h:
```
g++ -o test_NavMesh test_NavMesh.cpp
./test_NavMesh
```
