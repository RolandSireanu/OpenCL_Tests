
__kernel void rgb_conv(__global char *A, __global char* R) {
 
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    

    // Do the operation
    
    R[i*3+2] = A[i*3];
    R[i*3] = A[(i*3)+2];
    R[i*3+1] = A[i*3+1];
    



}
