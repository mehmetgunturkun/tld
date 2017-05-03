#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include "kdtree.h"

unsigned int get_msec(void)
{
	static struct timeval timeval, first_timeval;

	gettimeofday(&timeval, 0);

	if(first_timeval.tv_sec == 0) {
		first_timeval = timeval;
		return 0;
	}
	return (timeval.tv_sec - first_timeval.tv_sec) * 1000 + (timeval.tv_usec - first_timeval.tv_usec) / 1000;
}

int kd_insert2(struct kdtree *tree, double x, double y, void *data)
{
	double buf[2];
	buf[0] = x;
	buf[1] = y;
	return kd_insert(tree, buf, data);
}

struct kdres* kd_nearest_n2(struct kdtree* tree, double x, double y, int num) {
    double pos[2];
    pos[0] = x;
    pos[1] = y;
    return kd_nearest_n(tree, pos, num);
}

struct kdres *kd_nearest_range2(struct kdtree *tree, double x, double y, double range)
{
	double buf[2];
	buf[0] = x;
	buf[1] = y;
	return kd_nearest_range(tree, buf, range);
}


int main(int argc, char **argv) {
    struct kdtree *kd;

    struct kdres *set1;
	unsigned int msec, start;

	kd = kd_create(2);

    kd_insert2(kd, 1, 0, 0);
    kd_insert2(kd, 1, 0, 0);
    kd_insert2(kd, 1, 0, 0);

	kd_insert2(kd, 2, 0, 0);
	kd_insert2(kd, 2, 1, 0);
	kd_insert2(kd, 2, 2, 0);

	start = get_msec();
	set1 = kd_nearest_n2(kd, 0, 0, 10);
	msec = get_msec() - start;
	printf("range query returned %d items in %.5f sec\n", kd_res_size(set1), (float)msec / 1000.0);
	kd_res_free(set1);


	kd_free(kd);
	return 0;
}
