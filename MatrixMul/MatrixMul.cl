
__kernel void mmul(
    const int N,
    __global float* A,
    __global float* B,
    __global float* C)
{
    int rowIdx = get_global_id(0);
    int colIdx = get_global_id(1);
    int elementIdx;
    float temp;


    for(elementIdx=0; elementIdx < N; elementIdx++)    
        temp += A[(rowIdx*N)+elementIdx] * B[(elementIdx*N)+colIdx];
    
    C[(rowIdx*N)+colIdx] = temp;
    


}
