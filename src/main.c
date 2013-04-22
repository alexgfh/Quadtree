#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"

int main(int argc, char **argv){
	int K, X, Y, V, N, M;
	double x, y, x1, y1, x2, y2;
	int i, j, k;
	int counter;//counts squares
	int r_index; //number of points in "points_in_range"
		
	node *q;
	bound boundary; //bound of root
	point *points_in_range;
	point P;
	
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
	
	scanf("%d", &K);
	for(i = 0; i<K; i++){
		if(i!=0) printf("\n");
		counter=1;
		scanf("%d %d", &X, &Y);
		scanf("%d", &V);
		scanf("%d", &N);
		
		boundary.center.X=(double)X/2;
		boundary.center.Y=(double)Y/2;
		boundary.quarter_dimension.X=(double)X/4;
		boundary.quarter_dimension.Y=(double)Y/4;
		
		construct(&q, boundary, V);
		
		for(j = 0; j< N; j++){
			scanf("%lf %lf", &x, &y);
			P.X=x; P.Y=y;
			insert(q, P, V, &counter);
		}
		if(N==0) counter=0;
		printf("%d", counter);
		
		scanf("%d", &M);
		
		points_in_range=malloc(sizeof(point)*N);
		
		for(j = 0; j<M; j++){
			r_index=0;
			
			printf("\n");
			scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
			query(q, x1, y1, x2, y2, points_in_range, &r_index);
			
			qsort(points_in_range, r_index, sizeof(point), point_compare);
						
			
			if(r_index!=0){
				printf("%.1f %.1f", points_in_range[0].X, points_in_range[0].Y);
			for(k = 1; k<r_index; k++){
				printf(", %.1f %.1f", points_in_range[k].X, points_in_range[k].Y);
			}
			}
		}
		free(points_in_range);
		destruct(q);
		free(q);
		
	}
	
	fclose(stdin);
	fclose(stdout);
	return 0;
		
}
