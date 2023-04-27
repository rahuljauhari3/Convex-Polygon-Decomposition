
/**
* @author      
*Suraj Nair 2020A7PS0051H               Rahul Jauhari 2020A7PS0106H 
*
*/
#include <chrono>
#include <ctime>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
/** 
 * @mainpage DAA Assignment 1
 * @section intro_sec Introduction
 * Here we are trying to replicate the research paper on decomposing a given polygon into smaller convex polygons using DCEL and finally merging the smaller polygons if they have a diagonal in common
 * @subsection Brief_Description Brief Description
 * Abstract
*Decomposing a non-convex polygon into simpler subsets has been a recurrent theme in the literature due to its many
*applications. In this paper, we present dierent algorithms for decomposing a polygon into convex polygons without
*adding new vertices as well as a procedure, which can be applied to any partition, to remove the unnecessary edges of a
*partition by merging the polygons whose union remains a convex polygon. Although the partitions produced by the
*algorithms may not have minimal cardinality, their easy implementation and their quick CPU times even for polygons
*with many vertices make them very suitable to be used when optimal decompositions are not required, as for instance,
*in constrained optimization problems having as feasible set a non-convex polygon (optimization problems are usually
*easier to solve in convex regions and making use of a branch and bound process or other techniques, it is not necessary
*to find the optimal solution in all the subsets, so finding convex decomposition with minimal cardinality may be time-
*wasting). Computational experiments are presented and analyzed. 
*/
using namespace std::chrono;
using namespace std;
/**
 * @detail  A  structure to hold vertices in the format (x,y) 
 * @brief  A  structure to hold vertices in the format (x,y)
*/
struct Point
{   //A  structure to hold vertices in the format (x,y)
    float x, y;
};

/**
 * @detail  A  structure to hold half edges of the polygon 
 * @brief  A  structure to hold half edges of the polygon
*/
struct HalfEdge
{
    Point start;
    Point end;
    HalfEdge *twin;
    HalfEdge *next;
    HalfEdge *prev;
    int face;
};
/**
 * @detail  A  structure to hold faces of the polygon 
 * @brief  A  structure to hold faces of the polygon
*/
struct Face
{
    HalfEdge *vertice;
};
/**
 * @detail  A data structure to hold the input polygon  
 * @brief Used for storing faces ,edges and vertices of the polygon
*/
struct DCEL
{
    vector<Point> vertice;
    vector<HalfEdge> edges;
    vector<Face> faces;
};
/*!
     * @brief This code is implementing a polygon partitioning algorithm. The algorithm takes a polygon with a set of vertices, and then finds the convex polygon from the set of vertices. It then splits the polygon into convex polygons by finding notches. 
     * @return The convex polygon from the set of vertices,  then splits the polygon into convex polygons by finding notches. 
     */ 
void decompose(DCEL &d, vector<Point> &vertice)
{
    /// Create the convex hull
    d.vertice = vertice;
    int n = vertice.size();
    int currFace = 0;

    /// Add all the edges
    for (int i = 0; i < n; i++)
    {
        HalfEdge e;
        e.start = vertice[i];
        e.end = vertice[(i + 1) % n];
        e.face = currFace;
        d.edges.push_back(e);
    }

    /// Add all the faces
    for (int i = 0; i < n; i++)
    {
        Face f;
        f.vertice = &d.edges[i];
        d.faces.push_back(f);
        currFace++;
    }

    /// Link the edges
    for (int i = 0; i < n; i++)
    {
        HalfEdge *e1 = &d.edges[i];
        HalfEdge *e2 = &d.edges[(i + 1) % n];

        e1->next = e2;
        e2->prev = e1;

        e1->twin = e2;
        e2->twin = e1;
    }
}
/**
     * @brief This function checks whether the interior angle of a given point is less than 180 degrees or not.
     * @return Boolean value. True if the angle at Point B is less than 180 degrees and false otherwise.
     */ 
bool angle(Point p1, Point p2, Point p3)
{
    /// vectors from p1 to p2 and p3 to p2
    float v1x =  p1.x-p2.x;
    float v1y =  p1.y-p2.y;
    float v2x =  p3.x-p2.x;
    float v2y =   p3.y-p2.y;
    float cross = v1x * v2y - v1y * v2x;
    if (cross>=0)return true;
    return false;
}
/**
     * @brief It is used to check whether a given point is a notch or not. (Note that a notch is a point which has
     *  interior angle greater than 180 degrees.)
     * @return  Boolean value. True if the point Point B is a notch, False otherwise.
     */ 
bool notch(const Point &p1, const Point &p2, const Point &p3)
{
    if (angle(p1, p2, p3) ==false)
    {
        return true;
    }
    return false;
}
/**
     * @brief This function takes as input the set of vertices and the current point of the polygon
     * @return Returns the point consecutive to the current point ( clockwise order)
     */ 
Point next(const vector<Point> &vertice, Point p)
{
    int k;
    for (int i = 0; i < vertice.size(); i++)
    {
        if (vertice[i].x == p.x && vertice[i].y == p.y)
        {
            k = i;
            break;
        }
    }
    return vertice[(k + 1) % vertice.size()];
}
/**
     * @brief It is used to check whether a given point is on the left side or right side of a line segment.
     *   Inputs: Point A, Point B, Point C.
     * @return Output: Integer value. 1 if the point Point C is on the left side of the line segment AB, -1 if the
     *  point <Point C is on the right side of the line segment AB, 0 if the point Point C is on the line
     *  segment AB.
     */ 
int side(const Point &p1, const Point &p2, const Point &p3)
{
    float val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    if (val >= 0)
        return 0;
    else
        return -1;
}
/**
     * @brief This function is used to check if the notches found in the given polygon lie inside the smallest possible rectangle ( parallel to the axes) enclosing the smaller convex polygon
     * Inputs: The convex polygon obtained after decomposition in nth iteration
     * @return OUTPUT : Coordinates of the smallest possible rectangle that encloses the given polygon .
     */ 
vector<float> rectangele(const vector<Point> convex_polygon)
{
    vector<float> ans;
    float min_x = convex_polygon[0].x;
    float min_y = convex_polygon[0].y;
    float max_x = convex_polygon[0].x;
    float max_y = convex_polygon[0].y;
    for (int i = 1; i < convex_polygon.size(); i++)
    {
        if (convex_polygon[i].x < min_x)
            min_x = convex_polygon[i].x;
        if (convex_polygon[i].x > max_x)
            max_x = convex_polygon[i].x;
        if (convex_polygon[i].y < min_y)
            min_y = convex_polygon[i].y;
        if (convex_polygon[i].y > max_y)
            max_y = convex_polygon[i].y;
    }
    ans.push_back(min_x);
    ans.push_back(min_y);
    ans.push_back(max_x);
    ans.push_back(max_y);
    return ans;
}
/// @cond DEV
vector<Point> remove_duplicates(vector<Point> &asd)
{
    for (int i = 0; i < asd.size(); i++)
    {
        for (int j = i + 1; j < asd.size(); j++)
        {
            if (asd[i].x == asd[j].x && asd[i].y == asd[j].y)
            {
                asd.erase(asd.begin() + i);
                i--;
            }
        }
    }
    return asd;
}

void check_if_same(vector<Point>&convex_polygon,vector<Point>&vertice)
{
    while(convex_polygon[0].x!=vertice[0].x&&convex_polygon[0].y!=vertice[0].y)
    {
        for(int i=0;i<convex_polygon.size();i++)
        {
            convex_polygon[i].x=vertice[i].x;
            convex_polygon[i].y=vertice[i].y;
        }
    }
}
/// @endcond
/**
     * @brief This code is implementing a polygon partitioning algorithm. The algorithm takes a polygon with a set of vertices, and then finds the convex polygon from the set of vertices. It then splits the polygon into convex polygons by finding notches. 
     * Input: Edges 
     * @return OUTPUT : Decomposed convex polygon.
     */ 
vector<vector<Point> > Decompose_polygon(vector<Point> &vertice)
{
    vector<vector<Point> > result;
    vector<Point> convex_polygon, notch_list;
    // L0 fv1g; m 1.
    convex_polygon.push_back(vertice[0]);   // adding the first vertex of the polygon, our algorithm starts from here
    int q = 1;
    int edge_size = vertice.size();
    Point p2,p1;
    int m = 0;
    int flag= 0,f=0;
    while ( vertice.size() > 3)  
    {
        q=1;
        m++;
        check_if_same(convex_polygon,vertice);
        // p1 last of convex_polygon and p2 next in vertice
        if(flag == 0) // adding the first 2 vertices in list
        {
            p1 = convex_polygon[0];
            p2 = next(vertice, p1);
            convex_polygon.push_back(p2);
            convex_polygon = remove_duplicates(convex_polygon);
        }
        Point p3 = next(vertice, vertice[q]); // next point
        while (angle(vertice[(q - 1) % vertice.size()], vertice[q], vertice[(q + 1) % vertice.size()])  && angle(vertice[q], vertice[(q + 1) % vertice.size()], p1)  && angle(vertice[(q + 1) % vertice.size()], p1, p2) ) //checking if adding the next point satisfies the following 3 conditions
        {
            convex_polygon.push_back(vertice[(q+1) % vertice.size()]);
            convex_polygon = remove_duplicates(convex_polygon);
            q++;
            p3 = next(vertice, vertice[q]); //
         
        }

        if (convex_polygon.size() == vertice.size()) // if the given polygon is a convex polygonn itself
        {
            result.push_back(convex_polygon);
            return result;
        }
        if(convex_polygon.size()==2) // if here are only 2 points in the list
        {
            p1=next(vertice, p1);
            p2= next(vertice, p1);
            convex_polygon.clear();
            convex_polygon.push_back(p1);
            convex_polygon.push_back(p2);f=-1;
            Point temporary = vertice[0];
            vertice.erase(vertice.begin());
            vertice.push_back(temporary);
        }
        else f=0;   
        if(f==0){
            if (convex_polygon.size() != vertice.size()&&convex_polygon.size()>2) // check 3.4.1
            {
                for (int i = q-1; i < vertice.size(); i++)  // find the notches in the polygon
                {
                    if (notch(vertice[i], vertice[(i + 1) % vertice.size()], vertice[(i + 2) % vertice.size()]))
                    {
                        // vector<float> temp = rectangele(convex_polygon);
                        // if (vertice[i].x >= temp[0] && vertice[i].x <= temp[2] && vertice[i].y >= temp[1] && vertice[i].y <= temp[3])
                            {
                                notch_list.push_back(vertice[(i + 1) % vertice.size()]);
                            }
                    }
                }
                vector<float> temp = rectangele(convex_polygon);
                for (int i = 0; i < notch_list.size(); i++)  // check if the notch lies outside the rectangle formed, if yes then remove such notches
                {
                    if (notch_list[i].x < temp[0] )
                    {
                        notch_list.erase(notch_list.begin() + i);
                        i--;
                    }
                    else if ( notch_list[i].x > temp[2] )
                    {
                        notch_list.erase(notch_list.begin() + i);
                        i--;
                    }
                    else if (notch_list[i].y < temp[1])
                    {
                        notch_list.erase(notch_list.begin() + i);
                        i--;
                    }
                    else if (notch_list[i].y > temp[3])
                    {
                        notch_list.erase(notch_list.begin() + i);
                        i--;
                    }
                }
                if (notch_list.size() > 0) // if after removing notches outside we are still left with some notches then check if those notches lie inside 
                {
                    // convex_polygon notch_list
                    Point vert = convex_polygon[1];
                    int side_of_convex = side(convex_polygon[0], convex_polygon[convex_polygon.size() - 1], vert); // define the side containing the first,lst and second point of the decomposed polygon

                    for (int i = 0; i < notch_list.size(); i++) // if the notch is not on the same side as defined above then remove it from the list
                    {
                        if (side_of_convex != side(convex_polygon[0], convex_polygon[convex_polygon.size() - 1], notch_list[i]))
                        {

                            notch_list.erase(notch_list.begin() + i); // check
                            i--;
                        }
                    }
                    for (int i = 0; i < notch_list.size(); i++) // ifthe still notches are present then that means that the notches lie inside the decomposed polygon and hence we have to remove certain points from our convex polygon
                    {
                        side_of_convex = side(convex_polygon[0], notch_list[i], vert);
                        for (int j = 1; j < convex_polygon.size(); j++)
                        {
                            if (side_of_convex != side(convex_polygon[0], notch_list[i], convex_polygon[j]))
                            {
                                convex_polygon.erase(convex_polygon.begin() + j); // check
                                // break;
                                j--;
                            }
                        }
                    }
                    
                }
            }
        
        
            if (convex_polygon[convex_polygon.size() - 1].x != p2.x || convex_polygon[convex_polygon.size() - 1].y != p2.y) // if the polygon obtained has more than 2 points after all the above operations then push it into result
            {
                if(convex_polygon.size()>2)
                {
                    result.push_back(convex_polygon);
                }
                Point first = convex_polygon[0], last = convex_polygon[convex_polygon.size() - 1]; // reset  points
                // int size_convex_polygon = convex_polygon.size();
                // remove convex polygon from vertcies
                
                for (int i = 0; i < convex_polygon.size(); i++)
                {
                    for (int j = 0; j < vertice.size(); j++)
                    {
                        if (convex_polygon[i].x == vertice[j].x && convex_polygon[i].y == vertice[j].y)
                        {
                            vertice.erase(vertice.begin() + j);q--;
                            i=0;
                            break;
                        }
                    }
                }
                vertice.insert(vertice.begin(), last);q++;
                vertice.insert(vertice.begin(), first);q++;
                vertice = remove_duplicates(vertice); // remove points from the original list
                
                //clearing and resetting all the lists below

            convex_polygon.clear(); 
            convex_polygon.push_back(first);
            convex_polygon = remove_duplicates(convex_polygon);
            edge_size = vertice.size();
            notch_list.clear();
            }
            // if the list has only 2 points then we have to restart the algorithm with a new starting point
            else 
            {
                    Point temp = vertice[0];
                    vertice.erase(vertice.begin());
                    vertice.push_back(temp);
                    p1 = p2;
                    p2= next(vertice,p1);  
                    convex_polygon = remove_duplicates(convex_polygon);//error aasaktha hai same vertice wala left shift
            }
        }
    }
    if(vertice.size()==3)
        {result.push_back(vertice);}
    
    return result;
}
/**
     * @brief Driver Function
     */
int main()
{
    vector<Point> vertice;
    int n;
    Point p;
    ifstream inFile("input.txt");
    inFile >> n;
    for (int i = 0; i < n; i++)
    {
        inFile >> p.x >> p.y;
        vertice.push_back(p);
    }
    inFile.close();

    DCEL d;
    decompose(d, vertice);
    // print decomposition
    cout << "Vertices: ";
    for (auto v : d.vertice)
    {
        cout << "(" << v.x << ", " << v.y << ") ";
    }
    cout<<endl;
    cout<<endl;

    cout << "Edges: ";
    for (auto e : d.edges)
    {
        cout << "((" << e.start.x << ", " << e.start.y << "), (" << e.end.x << ", " << e.end.y << ")) ";
    }
    
    cout<<endl;
    cout<<endl;

    cout << "Faces: ";
    for (auto f : d.faces)
    {
        cout << "( " << f.vertice->start.x << ", " << f.vertice->start.y << " ) ";
    }
    cout<<endl;
    cout<<endl;
    
    auto start = high_resolution_clock::now();
    vector<vector<Point> > result = Decompose_polygon(vertice);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    // vector<vector<Point> > result = Decompose_polygon(vertice);
    ofstream outFile("output.txt");
    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[i].size(); j++)
        {
            outFile << result[i][j].x << " " << result[i][j].y << endl;
        }
        outFile << endl;
    }
    outFile.close();
    
    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[i].size(); j++)
        {
            cout << "x " << result[i][j].x << " y " << result[i][j].y << endl;
        }
        cout << endl;
    }
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    return 0;
}
