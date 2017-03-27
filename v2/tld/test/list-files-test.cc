#define _GNU_SOURCE
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    struct dirent **namelist;
    int n,i;

    n = scandir("/home/gunturkun/MasterThesis/resources/04_pedestrian2/sequence", &namelist, 0, versionsort);
    if (n < 0)
        perror("scandir");
    else
    {
        for(i =0 ; i < n; ++i)
        {
            printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }
    return 0;
}
