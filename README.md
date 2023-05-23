# Convex Polygon Decomposition

This project is an implimentation of the following research paper
```
[1] Fernández, J., Cánovas, L., & Pelegrın, B. (2000). Algorithms for the decomposition of a polygon into ́
convex polygons. European Journal of Operational Research, 121(2), 330-342.
https://doi.org/10.1016/S0377-2217(99)00033-8
```
The provided code is an implementation of a polygon partitioning algorithm based on the concept of decomposing a given non-convex polygon into smaller convex polygons. The algorithm utilizes the Doubly Connected Edge List (DCEL) data structure to represent the polygon and perform the decomposition.

Here is a step-by-step explanation of the algorithm:

1. The code begins with the definition of various data structures such as `Point`, `HalfEdge`, `Face`, and `DCEL`. These structures are used to store the vertices, edges, and faces of the polygon.

2. The `decompose` function is responsible for creating the initial DCEL representation of the polygon. It takes a set of vertices as input and constructs the edges and faces of the polygon based on the vertices' order.

3. The `angle` function determines whether the interior angle at a given point is less than 180 degrees. It calculates the cross product of two vectors formed by three points and checks the sign of the cross product to determine the angle's orientation.

4. The `notch` function determines if a given point is a "notch" or not. A notch is defined as a point in the polygon that has an interior angle greater than 180 degrees. It calls the `angle` function to check if the angle at the given point is less than 180 degrees. If not, it considers the point as a notch.

5. The `next` function returns the point that follows a given point in clockwise order based on the vertices' order.

6. The `side` function is used to determine whether a given point is on the left side, right side, or on the line segment formed by two other points. It calculates the cross product between vectors formed by the three points and checks the sign of the result to determine the relative position.

7. The `rectangle` function is responsible for finding the smallest possible rectangle (parallel to the axes) that encloses a given convex polygon. It iterates over the vertices of the polygon to find the minimum and maximum x and y coordinates, which define the rectangle.

8. The `remove_duplicates` function removes any duplicate vertices from a given list of points. It iterates over the list and removes any points that have the same coordinates as previously encountered points.

9. The `check_if_same` function compares the vertices of the current convex polygon with the original vertices. If they are not the same, it updates the vertices of the convex polygon to match the original vertices. This is necessary to handle the case where the convex polygon becomes the same as the original polygon due to the decomposition process.

10. The main `Decompose_polygon` function implements the polygon partitioning algorithm. It takes the original set of vertices as input and returns a list of decomposed convex polygons.

11. The function starts by initializing variables and data structures. It adds the first vertex of the polygon to the `convex_polygon` list.

12. The algorithm iterates until the number of vertices in the `vertice` list is reduced to 3 or less. This is because a polygon with 3 or fewer vertices is already convex and does not need further decomposition.

13. Inside the iteration, the algorithm checks if the current `convex_polygon` is the same as the original polygon. If so, it adds the `convex_polygon` to the `result` list and returns the result.

14. If the `convex_polygon` has only 2 points, it updates the `p1` and `p2` variables accordingly, clears the `convex_polygon` list, and removes the first vertex from the `vertice` list. This step handles the case where the convex polygon has reduced to a line segment.

15. If the `convex_polygon` has

 more than 2 points, the algorithm finds the next vertex (`p3`) in the `vertice` list based on the current vertex (`p2`) and the previous vertex (`p1`). It checks if `p3` is a notch using the `notch` function.

16. If `p3` is a notch, the algorithm proceeds to check if the notch lies outside the smallest possible enclosing rectangle of the current `convex_polygon`. If it does, the notch is discarded, and the algorithm moves on to the next vertex (`p2` becomes `p3`). Otherwise, the notch is considered as a valid point for further decomposition.

17. If `p3` is not a notch, the algorithm checks if it lies on the correct side of the current `convex_polygon` using the `side` function. If it does, `p3` is added to the `convex_polygon`, and the algorithm moves on to the next vertex (`p1` becomes `p2`, `p2` becomes `p3`).

18. After the iteration is complete, the algorithm adds the remaining `convex_polygon` to the `result` list.

19. Finally, the `Decompose_polygon` function returns the `result` list containing the decomposed convex polygons.

In summary, the algorithm works by iteratively adding vertices to a convex polygon until certain conditions are met. It handles notches by discarding those outside the smallest enclosing rectangle and selecting those on the correct side of the convex polygon. The process continues until the polygon is fully decomposed, and the resulting convex polygons are stored in the `result` list.
