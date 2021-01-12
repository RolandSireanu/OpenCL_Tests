
__kernel void krn()
{

    int globalIdx0 = get_global_id(0);
    int globalIdx1 = get_global_id(1);
    int groupId0 = get_group_id(0);
    int groupId1 = get_group_id(1);
    int localIdx0 = get_local_id(0);
    int localIdx1 = get_local_id(1);
    int localSizeIdx0 = get_local_size(0);
    int localSizeIdx1 = get_local_size(1);
    int globalSizeIdx0 = get_global_size(0);
    int globalSizeIdx1 = get_global_size(1);
    
    //printf("%d , %d , %d , %d , %d ,%d \n", globalIdx0, globalIdx1, groupId0, groupId1, localIdx0, localIdx1);

    printf("Global_id = %d , %d \n", globalIdx0, globalIdx1);
    printf("Group_id = %d , %d \n", groupId0, groupId1);
    printf("Local_id = %d , %d \n", localIdx0, localIdx1);
    printf("Local_size = %d \n", localSizeIdx0);
    printf("Local_size = %d \n", localSizeIdx1);
    printf("Global_size = %d \n", globalSizeIdx0);
    printf("Global_size = %d \n", globalSizeIdx1);
    printf("\t----------------------------------------------------------\n");

}
