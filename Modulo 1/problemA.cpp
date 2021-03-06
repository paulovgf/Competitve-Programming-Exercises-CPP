
// C program to show segment tree operations like construction, query 
// and update 
#include <stdio.h> 
#include <math.h> 

/*There are two separate tasks here:

Efficiently generate an array c where ci is the number of recipes that recommend temperature i.
Efficiently answer queries "how many numbers ca, ca + 1, ca + 2, ..., cb" are at least k?" where k is fixed across all queries.
There are some solutions to this task using advanced data structures or algorithms. For example, a conceptually straightforward idea is the following: create a segment tree c. We can treat all recipes as range update queries which we can do efficiently in (where m is the largest ai or bi) time using lazy propagation.

After all recipes, we replace all ci by 1 if it's at least k, and 0 otherwise. Afterwards, each of the next q queries is a basic range sum query, which can be done simply in  time.

Other solutions exist, too: Fenwick trees with range updates, event sorting, sqrt-decomposition with binary search, Mo's algorithm, and so on. These solutions all pass, but they are all overkill for this task.

A very simple solution is as follows. Initialize c with all zeroes. For recipe that recommends temperatures between li and ri, we should increment cli and decrement cri + 1.

Cumulate all values. That is, set ci to c1 + c2 + c3 + ... + ci. This can be done with one pass through the array.

Now, magically, ci is now the number of recipes that recommend temperature i. If ci is at least k, set it to 1, otherwise, set it to 0.

Cumulate all values again.

Now, every query that asks for the number of admissible temperatures between a and b can be answered simply as cb - ca - 1.

This runs in O(n + q + m), which is really fast.

Note that if your solution does this and still runs quite slow, chances are your solution is using slower input methods. We raised the time limit to 2.5 seconds in this problem in order to avoid failing slow input solutions.
  */
// A utility function to get the middle index from corner indexes. 

int binarySearch(int arr[], int l, int r, int x) 
{ 
    while (l <= r) 
    { 
        int m = l + (r-l)/2; 
  
        // Check if x is present at mid 
        if (arr[m] == x) 
            return m; 
  
        // If x greater, ignore left half 
        if (arr[m] < x) 
            l = m + 1; 
  
        // If x is smaller, ignore right half 
        else
            r = m - 1; 
    } 
  
    // if we reach here, then element was 
    // not present 
    return -1; 
} 
  
int main(void) 
{ 
    int arr[] = {2, 3, 4, 10, 40}; 
    int n = sizeof(arr)/ sizeof(arr[0]); 
    int x = 10; 
    int result = binarySearch(arr, 0, n-1, x); 
    (result == -1)? printf("Element is not present"
                                       " in array") 
               : printf("Element is present at "
                                "index %d", result); 
    return 0; 
} 

int getMid(int s, int e) {  return s + (e -s)/2;  } 
  
/*  A recursive function to get the sum of values in given range 
    of the array. The following are parameters for this function. 
  
    st    --> Pointer to segment tree 
    si    --> Index of current node in the segment tree. Initially 
              0 is passed as root is always at index 0 
    ss & se  --> Starting and ending indexes of the segment represented 
                 by current node, i.e., st[si] 
    qs & qe  --> Starting and ending indexes of query range */
int getSumUtil(int *st, int ss, int se, int qs, int qe, int si) 
{ 
    // If segment of this node is a part of given range, then return 
    // the sum of the segment 
    if (qs <= ss && qe >= se) 
        return st[si]; 
  
    // If segment of this node is outside the given range 
    if (se < qs || ss > qe) 
        return 0; 
  
    // If a part of this segment overlaps with the given range 
    int mid = getMid(ss, se); 
    return getSumUtil(st, ss, mid, qs, qe, 2*si+1) + 
           getSumUtil(st, mid+1, se, qs, qe, 2*si+2); 
} 
  
/* A recursive function to update the nodes which have the given  
   index in their range. The following are parameters 
    st, si, ss and se are same as getSumUtil() 
    i    --> index of the element to be updated. This index is  
             in the input array. 
   diff --> Value to be added to all nodes which have i in range */
void updateValueUtil(int *st, int ss, int se, int i, int diff, int si) 
{ 
    // Base Case: If the input index lies outside the range of  
    // this segment 
    if (i < ss || i > se) 
        return; 
  
    // If the input index is in range of this node, then update  
    // the value of the node and its children 
    st[si] = st[si] + diff; 
    if (se != ss) 
    { 
        int mid = getMid(ss, se); 
        updateValueUtil(st, ss, mid, i, diff, 2*si + 1); 
        updateValueUtil(st, mid+1, se, i, diff, 2*si + 2); 
    } 
} 
  
// The function to update a value in input array and segment tree. 
// It uses updateValueUtil() to update the value in segment tree 
void updateValue(int arr[], int *st, int n, int i, int new_val) 
{ 
    // Check for erroneous input index 
    if (i < 0 || i > n-1) 
    { 
        printf("Invalid Input"); 
        return; 
    } 
  
    // Get the difference between new value and old value 
    int diff = new_val - arr[i]; 
  
    // Update the value in array 
    arr[i] = new_val; 
  
    // Update the values of nodes in segment tree 
    updateValueUtil(st, 0, n-1, i, diff, 0); 
} 
  
// Return sum of elements in range from index qs (quey start) 
// to qe (query end).  It mainly uses getSumUtil() 
int getSum(int *st, int n, int qs, int qe) 
{ 
    // Check for erroneous input values 
    if (qs < 0 || qe > n-1 || qs > qe) 
    { 
        printf("Invalid Input"); 
        return -1; 
    } 
  
    return getSumUtil(st, 0, n-1, qs, qe, 0); 
} 
  
// A recursive function that constructs Segment Tree for array[ss..se]. 
// si is index of current node in segment tree st 
int constructSTUtil(int arr[], int ss, int se, int *st, int si) 
{ 
    // If there is one element in array, store it in current node of 
    // segment tree and return 
    if (ss == se) 
    { 
        st[si] = arr[ss]; 
        return arr[ss]; 
    } 
  
    // If there are more than one elements, then recur for left and 
    // right subtrees and store the sum of values in this node 
    int mid = getMid(ss, se); 
    st[si] =  constructSTUtil(arr, ss, mid, st, si*2+1) + 
              constructSTUtil(arr, mid+1, se, st, si*2+2); 
    return st[si]; 
} 
  
/* Function to construct segment tree from given array. This function 
   allocates memory for segment tree and calls constructSTUtil() to 
   fill the allocated memory */
int *constructST(int arr[], int n) 
{ 
    // Allocate memory for the segment tree 
  
    //Height of segment tree 
    int x = (int)(ceil(log2(n)));  
  
    //Maximum size of segment tree 
    int max_size = 2*(int)pow(2, x) - 1;  
  
    // Allocate memory 
    int *st = new int[max_size]; 
  
    // Fill the allocated memory st 
    constructSTUtil(arr, 0, n-1, st, 0); 
  
    // Return the constructed segment tree 
    return st; 
} 
  
// Driver program to test above functions 
int main() 
{ 
    int arr[] = {1, 3, 5, 7, 9, 11}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
  
    // Build segment tree from given array 
    int *st = constructST(arr, n); 
  
    // Print sum of values in array from index 1 to 3 
    printf("Sum of values in given range = %dn",  
            getSum(st, n, 1, 3)); 
  
    // Update: set arr[1] = 10 and update corresponding  
    // segment tree nodes 
    updateValue(arr, st, n, 1, 10); 
  
    // Find sum after the value is updated 
    printf("Updated sum of values in given range = %dn", 
             getSum(st, n, 1, 3)); 
    return 0; 
} 