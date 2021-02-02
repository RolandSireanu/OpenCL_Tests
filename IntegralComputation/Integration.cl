
__kernel void pi_computation(const int nr_of_ittrs, const float step, 
            __local float* workgroup_sums, __global float* partial_sums)
{

    int globalIdx0 = get_global_id(0);
    
    int groupId0 = get_group_id(0);
    
    int localIdx0 = get_local_id(0);
    
    int localSizeIdx0 = get_local_size(0);
    
    int globalSizeIdx0 = get_global_size(0);
    
    
    //printf("%d %d %d %d %d\n", globalIdx0, groupId0, localIdx0, globalSizeIdx0, localSizeIdx0);

    //printf("%d , %f\n",nr_of_ittrs, step);

    float start = globalIdx0 * nr_of_ittrs * step;
    float sum = 0.00f;
    //printf("start = %f\n",start);
    for(int i=0; i<nr_of_ittrs-1; i++)
    {
        float x = start + ((i+0.5f)*step);
        sum += (4.0f/(1.0f+x*x));        
    }

    workgroup_sums[localIdx0] = sum;
    barrier(CLK_LOCAL_MEM_FENCE);

    //printf("workgroup_sum = %f\n",workgroup_sums[globalIdx0]);
    
    if(localIdx0 == 0)
    {
        float totalSum=0;
        
        for(int i=0; i<localSizeIdx0; i++)
        {
            totalSum += workgroup_sums[i];
        }

        partial_sums[groupId0] = totalSum;
    }

    
}
