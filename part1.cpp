// Assignment 3: Euclidean traveling salesperson problem: exhaustive optimization algorithm
// Duy Do           - CWID: 891242679
// Billy Saysavath  - CWID: 891424020
// CPSC 335 - Section 3 - Fall 2015
// A special case of the classical traveling salesman problem (TSP) where the input is a Euclidean graph
// INPUT: a positive integer n and a list P of n distinct points representing vertices of a Euclidean graph
// OUTPUT: a list of n points from P representing a Hamiltonian cycle of minimum total weight for the graph.

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <math.h>
#include <chrono>
#include <iomanip>

using namespace std;

struct point2D {
  float x; // x coordinate
  float y; // y coordinate
};

void print_cycle(int, point2D*, int*);
// function to print a cyclic sequence of 2D points in 2D plane, given the 
// number of elements and the actual sequence stored as an array of 2D points

float farthest(int, point2D*);
// function to calculate the furthest distance between any two 2D points

void print_perm(int, int *, int, point2D*, int *, float &);
// function to generate the permutation of indices of the list of points

int main() { 
  point2D *P;
  int *bestSet, *A;
  int i, n;
  float bestDist, Dist;
  
  // display the header
  cout << endl << "CPSC 335-x - Programming Assignment #3" << endl;
  cout << "Euclidean traveling salesperson problem: exhaustive optimization algorithm" << endl;
  cout << "Enter the number of vertices (>2) " << endl; 
  
  // read the number of elements
  cin >> n;
  
  // if less than 3 vertices then terminate the program
  if (n <3) 
    return 0;
  
  // allocate space for the sequence of 2D points
  P = new point2D[n];
  
  // read the sequence of distinct points
  cout << "Enter the points; make sure that they are distinct" << endl;
  for( i=0; i < n; i++) {
    cout << "x=";
    cin >> P[i].x;
    cout << "y=";
    cin >> P[i].y;
  }
   
  // allocate space for the best set representing the indices of the points
  bestSet = new int[n];
  // set the best set to be the list of indices, starting at 0
  for(i=0; i<n; i++)
    bestSet[i]=i;
  
  // Start the chronograph to time the execution of the algorithm
  auto start = chrono::high_resolution_clock::now();
  
  // calculate the farthest pair of vertices
  Dist = farthest(n,P);
  bestDist = n*Dist;
  
  // populate the starting array for the permutation algorithm
  A = new int[n];
  // populate the array A with the values in the range 0 .. n-1
  for(i=0; i<n; i++)
    A[i] = i;
  
  // calculate the Hamiltonian cycle of minimum weight
  print_perm(n, A, n, P, bestSet, bestDist);
  
  // End the chronograph to time the loop
  auto end = chrono::high_resolution_clock::now();
  
  // after shuffling them
  cout << "The Hamiltonian cycle of the minimum length " << endl;
  print_cycle(n, P, bestSet);
  cout << "Minimum length is " << bestDist << endl;
  
  // print the elapsed time in seconds and fractions of seconds
  int microseconds = 
    chrono::duration_cast<chrono::microseconds>(end - start).count();
  double seconds = microseconds / 1E6;
  cout << fixed << setprecision(9);
  cout << "elapsed time: " << seconds << " seconds" << endl;
  
  // de-allocate the dynamic memory space
  delete [] P;
  delete [] A;
  delete [] bestSet;

  return EXIT_SUCCESS;
}

void print_cycle(int n, point2D *P, int *seq)
// function to print a sequence of 2D points in 2D plane, given the number of elements and the actual 
// sequence stored as an array of 2D points
// n is the number of points
// seq is a permutation over the set of indices
// P is the array of coordinates
{
  int i;
  
  for(i=0; i< n; i++) 
    cout << "(" << P[seq[i]].x << "," << P[seq[i]].y << ") ";
  cout << "(" << P[seq[0]].x << "," << P[seq[0]].y << ") ";
  cout << endl;
}

float farthest(int n, point2D *P)
// function to calculate the furthest distance between any two 2D points
{
  float max_dist;
  int i;
  float dist;
  
  // assume that the farthest distance is between point 0 and point n-1
  max_dist = sqrt( (P[0].x - P[n-1].x)*(P[0].x - P[n-1].x)  + (P[0].y - P[n-1].y)*(P[0].y - P[n-1].y) );
  for (i=0; i < n-1; i++) {
    dist = sqrt(  (P[i].x - P[i+1].x)*(P[i].x - P[i+1].x) +  (P[i].y - P[i+1].y)*(P[i].y - P[i+1].y) );
    if (max_dist < dist)
      max_dist = dist;
  }
  return max_dist;
}

void print_perm(int n, int *A, int sizeA, point2D *P, int *bestSet, float &bestDist) 
// function to generate the permutation of indices of the list of points
{
  int i;
  float dist;
  
  if (n == 1) {
    // we obtain a permutation so we compare it against the current shortest
    // Hamiltonian cycle
    // calculate the total distance by calculate the distance from the first node to the last node
    // then add the distances from first node to second, second to third, ...last node
    dist = sqrt( (P[A[0]].x - P[A[sizeA-1]].x)*(P[A[0]].x - P[A[sizeA-1]].x)  + (P[A[0]].y - P[A[sizeA-1]].y)*(P[A[0]].y - P[A[sizeA-1]].y) );
    for (i=0; i < sizeA-1; i++) {
      dist += sqrt( (P[A[i]].x - P[A[i+1]].x)*(P[A[i]].x - P[A[i+1]].x) +  (P[A[i]].y - P[A[i+1]].y)*(P[A[i]].y - P[A[i+1]].y) );
    }

    //Compare the total distance with the bestDist. If it is smaller, then replace the bestSet with the current permutation
    if (dist < bestDist ) {
      for (i=0; i < sizeA; i++) bestSet[i] = A[i];
      bestDist = dist;
    }
    
  }
  else {
    for(i = 0 ; i< n-1; i++) {
      print_perm(n - 1, A, sizeA, P, bestSet, bestDist);
      if (n%2 == 0) {
    // swap(A[i], A[n-1])
    int temp = A[i];
    A[i] = A[n-1];
    A[n-1]=temp;
      }
      else
    {
      // swap(A[0], A[n-1])
      int temp = A[0];
      A[0] = A[n-1];
      A[n-1]=temp;
    }
    }
    print_perm(n - 1, A, sizeA, P, bestSet, bestDist);
  }
}
