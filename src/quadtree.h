//2D point
typedef struct point{
	double X;
	double Y;
} point;

//determines boundaries
typedef struct bound{
	point center;
	point quarter_dimension; //(X,Y) == (width/4,heigth/4), used to find center of children node
} bound;

//Quadtree node
typedef struct node{
	int points_size; //number of points inserted in the node
	struct point *points;
	struct bound boundary;
	struct node* NW; //north-west
	struct node* NE; //north-east
	struct node* SW; //south-west
	struct node* SE; //south-east
} node;

//constructor:O(1)
void construct(node** q, bound b, int capacity);

//inserts on leaf, if leaf is full, subdivides
void insert(node* nod, point P, int capacity, int *counter);

//finds the center and quarter_dimension of the new node:O(1)
bound convert(bound b, int position);

//transfer points from parent to children, non-leaf nodes never keep points:O(V)
void transferpoints(node* nod);

//destructor
void destruct(node *nod);

//puts in "points_in_range" the points that are in the ractangle
void query(node *nod, double x1, double y1, double x2, double y2, point *points_in_range, int *r_index);

//creates four children:O(V)
void subdivide(node *nod, int capacity);

//checks if a certain point is inside a given rectangle:O(1)
int contain(point p, double x1, double y1, double x2, double y2);

//qsort compare function
int point_compare(const void *p1, const void *p2);
