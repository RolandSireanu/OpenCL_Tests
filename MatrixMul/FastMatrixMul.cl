
__kernel void fastMmul(
    const int N,
    __global float* A,
    __global float* B,
    __global float* C)
{
    int rowIdx = get_global_id(0);    
    int elementIdx;
    float temp;
    float copyA[1024];

    // coalescing = when multiple threads access in the same time sequential memory location
    // gpu driver will join them in a single read using the entire bandwidth of the memory

    for(int i=0; i<N; i++)
        copyA[i] = A[rowIdx*N+i];
    

    
    for(int j=0; j<N; j++)
    {
        temp=0;
        for(int k=0; k<N; k++)
        {
            temp += copyA[k]*B[k*N+j];
        }
        C[(rowIdx*N)+j] = temp;
    }
}
