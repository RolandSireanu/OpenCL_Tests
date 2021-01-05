
__kernel void rgb_conv(__global char *A) {
 
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    

    // Do the operation
    
    A[138+(i*3+2)] = A[138+(i*3)];
    A[138+(i*3)] = A[138+(i*3+2)];
    



}
