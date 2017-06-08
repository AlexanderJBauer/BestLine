// Alexander Bauer
// CS 130B Programming Assignment 4
// Last Edited: 6/7/2017

#include <string>   /* std::string */
#include <cmath>  /* std::abs */
#include <limits> /* std::numeric_limits */
#include <math.h> /* sqrt */
#include <vector> /* std::vector */
#include <cstdlib> /* qsort */
#include <iostream> /* std::cout, std::cin */
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */
#include <algorithm>    // std::sort

// Point struct to make things easier
struct Point{

    double x;
    double y;

    //CONSTRUCTOR
    Point( double xVal = 0, double yVal = 0 )
    : x{ xVal }, y{ yVal } { }

};

//DISTANCE FUNCTION
double distanceBetween( Point p1, Point p2 ){
    return sqrt( ( p1.x - p2.x ) * ( p1.x - p2.x )
                +( p1.y - p2.y ) * ( p1.y - p2.y ) );
}

//Create line from seed
std::vector<double> CreateLineFromSeeds( Point p1, Point p2 ){
    double a = ( p1.y - p2.y ) / ( p1.x - p2.x );
    double b = -a*p1.x + p1.y;
    std::vector<double> result;
    result.push_back( a ); result.push_back( b );
    return result;
}

//Calculate distance from Point to a line y = ax + b
double distanceFromLine( Point p, double a, double b ){
    double y = a*p.x +b;
    double distance = std::abs( p.y - y );
    return distance;
}

//Calculate the least-square fit line using a median error
std::vector<double> LeastSquareFitLine( std::vector<Point> P, double a, double b, double median_error ){

    int numPoints = P.size();

    std::vector<double> errors( numPoints );
    for( int j = 0; j < numPoints; j++ ){
        errors[j] = distanceFromLine( P[j], a, b );
    }

    double sumX = 0;
    double sumX2 = 0;
    double sumY = 0;
    double sumXY = 0;
    int pointsUsed = 0;

    for( int i = 0; i < numPoints; i++ ){
        if( errors[i] <= median_error + 1e-12 ){
            sumX += P[i].x;
            sumX2 += P[i].x*P[i].x;
            sumY += P[i].y;
            sumXY += P[i].x*P[i].y;
            pointsUsed++;
        }
    }

    std::vector<double> finalAB( 2 );
    double determinant = 1 / ( sumX2*pointsUsed - sumX*sumX );
    finalAB[0] = determinant*pointsUsed*sumXY + determinant*(-sumX)*sumY;
    finalAB[1] = determinant*(-sumX)*sumXY + determinant*sumX2*sumY;

    return finalAB;
}

int main() {

    /* initialize random seed: */
    srand (time(NULL));

    int numPoints = 0;
    double x;
    double y;
    std::vector<Point> P;

    std::cin >> numPoints;

    for( int i = 0; i < numPoints; i++){
        std::cin >> x;
        std::cin >> y;
        P.push_back( Point( x, y ) );
    }

    std::vector<double> a_and_b( 2 );
    std::vector<double> errors( numPoints );
    double smallest_error = std::numeric_limits<double>::max();
    int i1 = 0;
    int i2 = 0;

    //Iterations of random trials
    //The number 16 comes from k = log(1-p)/log(1-w^2)
    // w is the fraction of inliers
    // p is the probability of finding a set of points free of outliers
    // w = .5  ( min according to assignment )
    // p = .99
    // The reason you don't get exact answer every time is either because
    // 1. One of the chosen points used is not an inlier
    // 2. Or when calculating best square-fit line
    //      a. You dont get a set of all inliers by using median_error as a judge
    //      b. You dont get all of the inliers by using median_error
    //          and are therefore missing certain points.
    for( int i = 0; i < 16; i++ ){
        //Calculate 2 random indicies
        i1 = rand() % numPoints;
        i2 = rand() % numPoints;

        //Create line from seeds
        std::vector<double> ab = CreateLineFromSeeds( P[i1], P[i2] );

        //Calculate errors
        for( int j = 0; j < numPoints; j++ ){
            errors[j] = distanceFromLine( P[j], ab[0], ab[1] );
        }

        //Sort errors using qsort
        std::sort( errors.begin(), errors.end() );

        //Deal with current median_error
        double median_error = errors[numPoints/2];
        if( median_error < smallest_error ){
            smallest_error = median_error;
            a_and_b[0] = ab[0];
            a_and_b[1] = ab[1];
        }
    }

    std::vector<double> finalAB;
    finalAB = LeastSquareFitLine( P, a_and_b[0], a_and_b[1], smallest_error );
    std::cout << "a = " << finalAB[0] << std::endl
              << "b = " << finalAB[1] << std::endl;

    return 0;
}
