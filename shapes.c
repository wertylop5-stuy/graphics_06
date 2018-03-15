#include"include/shapes.h"

void scale(struct Matrix *t, float a, float b, float c) {
	struct Matrix *temp = new_matrix(4, 4);
	ident(temp);
	
	temp->m[0][0] = a;
	temp->m[1][1] = b;
	temp->m[2][2] = c;
	
	matrix_mult(temp, t);
	free_matrix(temp);
}

void move(struct Matrix *t, float a, float b, float c) {
	struct Matrix *temp = new_matrix(4, 4);
	ident(temp);
	
	temp->m[0][3] = a;
	temp->m[1][3] = b;
	temp->m[2][3] = c;
	
	matrix_mult(temp, t);
	free_matrix(temp);
}

void rotate(struct Matrix *t, char axis, float deg) {
	struct Matrix *temp = new_matrix(4, 4);
	ident(temp);
	float rad = deg * (M_PI/180.0f);
	
	switch(axis) {
		case 'z':
			temp->m[0][0] = cosf(rad);
			temp->m[0][1] = -sinf(rad);
			temp->m[1][0] = sinf(rad);
			temp->m[1][1] = cosf(rad);
		break;
		
		case 'x':
			temp->m[1][1] = cosf(rad);
			temp->m[1][2] = -sinf(rad);
			temp->m[2][1] = sinf(rad);
			temp->m[2][2] = cosf(rad);
		break;
		
		case 'y':
			temp->m[0][0] = cosf(rad);
			temp->m[2][0] = -sinf(rad);
			temp->m[0][2] = sinf(rad);
			temp->m[2][2] = cosf(rad);
		break;
	}

	matrix_mult(temp, t);
	free_matrix(temp);
}

//the p0, p1, etc is just the x or y coord
void parametric_exec(struct Matrix *m,
		float ax, float bx, float cx, float dx,
		float ay, float by, float cy, float dy) {
	float t;
	for (t = 0; t <= 1; t += STEP) {
		push_edge(m,
				CUBIC(ax, bx, cx, dx, t),
				CUBIC(ay, by, cy, dy, t),
				0,
				CUBIC(ax, bx, cx, dx, t+STEP),
				CUBIC(ay, by, cy, dy, t+STEP),
				0);
	}
}

void make_circle(struct Matrix *m, float cx, float cy, float cz, float r) {
	float t;
	for (t = 0; t <= 1; t += STEP) {
		push_edge(m,
				r*cosf(2*M_PI*t) + cx,
				r*sinf(2*M_PI*t) + cy,
				cz,
				r*cosf(2*M_PI*(t+STEP)) + cx,
				r*sinf(2*M_PI*(t+STEP)) + cy,
				cz);
	}
}

void make_hermite(struct Matrix *m, float x0, float y0, float x1, float y1,
		float rx0, float ry0, float rx1, float ry1) {
	struct Matrix *h = new_matrix(4, 4);
	
	float temp[4][4] = {
		{2, -2, 1, 1},
		{-3, 3, -2, -1},
		{0, 0, 1, 0},
		{1, 0, 0, 0}
	};
	
	int x;
	for (x = 0; x < 4; x++) {
		memcpy(h->m[x], temp[x], sizeof(temp[x]));
	}
	h->back = 4;
	
	struct Matrix *xpoints = new_matrix(4, 1);
	struct Matrix *ypoints = new_matrix(4, 1);
	
	xpoints->m[0][0] = x0;
	xpoints->m[1][0] = x1;
	xpoints->m[2][0] = rx0;
	xpoints->m[3][0] = rx1;
	xpoints->back = 1;

	ypoints->m[0][0] = y0;
	ypoints->m[1][0] = y1;
	ypoints->m[2][0] = ry0;
	ypoints->m[3][0] = ry1;
	ypoints->back = 1;

	matrix_mult(h, xpoints);
	matrix_mult(h, ypoints);
	
	parametric_exec(m,
		xpoints->m[0][0],xpoints->m[1][0],
		xpoints->m[2][0],xpoints->m[3][0],
		ypoints->m[0][0],ypoints->m[1][0],
		ypoints->m[2][0],ypoints->m[3][0]);
	
	free_matrix(xpoints);
	free_matrix(ypoints);
	free_matrix(h);
}

void make_bezier(struct Matrix *m, float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3) {
	struct Matrix *h = new_matrix(4, 4);
	float temp[4][4] = {
		{-1, 3, -3, 1},
		{3, -6, 3, 0},
		{-3, 3, 0, 0},
		{1, 0, 0, 0}
	};
	int x;
	for (x = 0; x < 4; x++) {
		memcpy(h->m[x], temp[x], sizeof(temp[x]));
	}
	h->back = 4;
	
	struct Matrix *xpoints = new_matrix(4, 1);
	struct Matrix *ypoints = new_matrix(4, 1);
	
	xpoints->m[0][0] = x0;
	xpoints->m[1][0] = x1;
	xpoints->m[2][0] = x2;
	xpoints->m[3][0] = x3;
	xpoints->back = 1;

	ypoints->m[0][0] = y0;
	ypoints->m[1][0] = y1;
	ypoints->m[2][0] = y2;
	ypoints->m[3][0] = y3;
	ypoints->back = 1;

	matrix_mult(h, xpoints);
	matrix_mult(h, ypoints);
	
	parametric_exec(m,
		xpoints->m[0][0],xpoints->m[1][0],
		xpoints->m[2][0],xpoints->m[3][0],
		ypoints->m[0][0],ypoints->m[1][0],
		ypoints->m[2][0],ypoints->m[3][0]);
	
	free_matrix(xpoints);
	free_matrix(ypoints);
	free_matrix(h);
}

