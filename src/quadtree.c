#include <stdlib.h>
#include "quadtree.h"

//finds the center and quarter_dimension of the new node:O(1)
bound convert(bound b, int position){
	bound r;
	switch(position){
		case 1://NW
			r.center.X=b.center.X+(b.quarter_dimension.X);
			r.center.Y=b.center.Y+(b.quarter_dimension.Y);
			r.quarter_dimension.X=b.quarter_dimension.X/2;
			r.quarter_dimension.Y=b.quarter_dimension.Y/2;
			break;
		case 2://NE
			r.center.X=b.center.X-(b.quarter_dimension.X);
			r.center.Y=b.center.Y+(b.quarter_dimension.Y);
			r.quarter_dimension.X=b.quarter_dimension.X/2;
			r.quarter_dimension.Y=b.quarter_dimension.Y/2;
			break;
		case 3://SE
			r.center.X=b.center.X-(b.quarter_dimension.X);
			r.center.Y=b.center.Y-(b.quarter_dimension.Y);
			r.quarter_dimension.X=b.quarter_dimension.X/2;
			r.quarter_dimension.Y=b.quarter_dimension.Y/2;
			break;
		case 4://SW
			r.center.X=b.center.X+(b.quarter_dimension.X);
			r.center.Y=b.center.Y-(b.quarter_dimension.Y);
			r.quarter_dimension.X=b.quarter_dimension.X/2;
			r.quarter_dimension.Y=b.quarter_dimension.Y/2;
			break;
	}
	return r;
}

//transfer points from parent to children, non-leaf nodes never keep points:O(V)
void transferpoints(node* nod){
	int i;
	//nod->points_size==capacity
	for(i=0; i<nod->points_size;i++){//insert never calls subdivide
		if(nod->points[i].X>=nod->boundary.center.X && nod->points[i].Y>=nod->boundary.center.Y)
			insert(nod->NW, nod->points[i], nod->points_size, NULL);
		
		else if(nod->points[i].X<nod->boundary.center.X && nod->points[i].Y>=nod->boundary.center.Y)
			insert(nod->NE, nod->points[i], nod->points_size, NULL);
		
		else if(nod->points[i].X<nod->boundary.center.X && nod->points[i].Y<nod->boundary.center.Y)
			insert(nod->SE, nod->points[i], nod->points_size, NULL);
		
		else /*if(nod->points[i].X>=nod->boundary.center.X && nod->points[i].Y<nod->boundary.center.Y)*/
			insert(nod->SW, nod->points[i], nod->points_size, NULL);
	}
	free(nod->points);//only leaf nodes keeps points
}

//constructor:O(1)
void construct(node** q, bound b, int capacity){
	*q=malloc(sizeof(node));
	(*q)->points_size=0;
	(*q)->points=malloc(capacity*sizeof(point));
	(*q)->boundary = b;
	(*q)->NW=(*q)->NE=(*q)->SE=(*q)->SW=NULL;
}

//creates four children:O(V)
void subdivide(node *nod, int capacity){
	construct(&nod->NW, convert(nod->boundary, 1), capacity);
	construct(&nod->NE, convert(nod->boundary, 2), capacity);
	construct(&nod->SE, convert(nod->boundary, 3), capacity);
	construct(&nod->SW, convert(nod->boundary, 4), capacity);
	
	transferpoints(nod);
}


//inserts on leaf, if leaf is full, subdivides
void insert(node *nod, point P, int capacity, int *counter){
		
	if(nod->points_size==capacity){
		
		if(nod->NW==NULL){//if its a leaf:
			subdivide(nod, capacity);//O(V)
			*counter+=3;
		}
		
		if(P.X>=nod->boundary.center.X && P.Y>=nod->boundary.center.Y){
			insert(nod->NW, P, capacity, counter);
		}
		else if(P.X<nod->boundary.center.X && P.Y>=nod->boundary.center.Y){
			insert(nod->NE, P, capacity, counter);
		}
		else if(P.X<nod->boundary.center.X && P.Y<nod->boundary.center.Y){
			insert(nod->SE, P, capacity, counter);
		}
		else /*if(P.X>=nod->boundary.center.X && P.Y<nod->boundary.center.Y)*/{
			insert(nod->SW, P, capacity, counter);
		}
	}
	
	else{
		nod->points[nod->points_size] = P;
		++nod->points_size;
	}			
}

//checks if a certain point is inside a given rectangle:O(1)
int contain(point p, double x1, double y1, double x2, double y2){
	if(p.X>=x1 && p.X<=x2 && p.Y>=y1 && p.Y<=y2)
		return 1;
	//else
	return 0;
}

//puts in "points_in_range" the points that are in the ractangle
void query(node *nod, double x1, double y1, double x2, double y2, point *points_in_range, int *r_index){
	
	if(nod->NW!=NULL){//if it isnt a leaf:
	
		if(x2>=nod->boundary.center.X && y2>=nod->boundary.center.Y){
			query(nod->NW, x1, y1, x2, y2, points_in_range, r_index);
		}
		if(x1<nod->boundary.center.X && y2>=nod->boundary.center.Y){
			query(nod->NE, x1, y1, x2, y2, points_in_range, r_index);
		}
		if(x1<nod->boundary.center.X && y1<nod->boundary.center.Y){
			query(nod->SE, x1, y1, x2, y2, points_in_range, r_index);
		}
		if(x2>=nod->boundary.center.X && y1<nod->boundary.center.Y){
			query(nod->SW, x1, y1, x2, y2, points_in_range, r_index);
		}
	}
	
	//if it is a leaf:
	else{
		int i;
		for(i=0; i<nod->points_size; i++){
			if (contain(nod->points[i],x1,y1,x2,y2)){
				points_in_range[*r_index]=nod->points[i];
				++(*r_index);
			}
			
		}
	}
}

//destructor
void destruct(node *nod){
	
	if(nod->NW==NULL)
		free(nod->points);
	
	else {
		destruct(nod->NW);
		destruct(nod->NE);
		destruct(nod->SE);
		destruct(nod->SW);
	}
	
	//when control flow reaches here, this nod is a leaf
	free(nod->NW);
	free(nod->NE);
	free(nod->SE);
	free(nod->SW);
	
}


//qsort compare function
int point_compare(const void *p1, const void *p2){
	if( ((point*)p1)->X <  ((point*)p2)->X ) return -1;
	if( ((point*)p1)->X >  ((point*)p2)->X ) return 1;
  /*if( ((point*)p1)->X == ((point*)p2)->X )*/ 
		if( ((point*)p1)->Y <  ((point*)p2)->Y )  return -1;
		if( ((point*)p1)->Y >  ((point*)p2)->Y )  return 1;
	  /*if( ((point*)p1)->Y == ((point*)p2)->Y )*/return 0;
}
