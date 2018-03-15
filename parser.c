#include"include/parser.h"

void parse_instructions(char *filename, struct Matrix *t,
			struct Matrix *e, Frame f) {
	FILE *file = fopen(filename, "r");
	if (!file) return;
	
	char line[256];
	char name[128];
	float x1, y1, z1, x2, y2, z2;
	char axis;
	
	while (fgets(line, sizeof(line), file) != 0) {
		if (!strncmp(line, "line", strlen(line)-1)) {
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f %f %f %f",
				&x1, &y1, &z1, &x2, &y2, &z2);
			push_edge(e, x1, y1, z1, x2, y2, z2);
		}
		else if (!strncmp(line, "ident", strlen(line)-1)) {
			ident(t);
		}
		else if (!strncmp(line, "scale", strlen(line)-1)) {
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f",
				&x1, &y1, &z1);
			scale(t, x1, y1, z1);
		}
		else if (!strncmp(line, "move", strlen(line)-1)) {
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f",
				&x1, &y1, &z1);
			move(t, x1, y1, z1);
		}
		else if (!strncmp(line, "rotate", strlen(line)-1)) {
			fgets(line, sizeof(line), file);
			sscanf(line, "%c %f",
				&axis, &z1);
			rotate(t, axis, z1);
		}
		else if (!strncmp(line, "apply", strlen(line)-1)) {
			matrix_mult(t, e);
		}
		else if (!strncmp(line, "display", strlen(line)-1)) {
			memset(f, 0, sizeof(Frame));
			struct Pixel p;
			pixel_color(&p, 0, 255, 0);
			draw_lines(f, e, &p);
			display(f);
		}
		else if (!strncmp(line, "save", strlen(line)-1)) {
			fgets(line, sizeof(line), file);
			sscanf(line, "%s", name);
			write_to_file(f);
			save_png(f, name);
		}
		else if (!strncmp(line, "quit", strlen(line)-1)) {
			break;
			fgets(line, sizeof(line), file);
			sscanf(line, "%s", name);
		}
		else if (!strncmp(line, "circle", strlen(line)-1)) {
			float cx, cy, cz, r;
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f %f", &cx, &cy, &cz, &r);
			make_circle(e, cx, cy, cz, r);
		}
		else if (!strncmp(line, "hermite", strlen(line)-1)) {
			float x0, y0, x1, y1, rx0, ry0, rx1, ry1;
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f %f %f %f %f %f",
				&x0, &y0, &x1, &y1, &rx0, &ry0, &rx1, &ry1);
			make_hermite(e, x0, y0, x1, y1, rx0, ry0, rx1, ry1);
		}
		else if (!strncmp(line, "bezier", strlen(line)-1)) {
			float x0, y0, x1, y1, x2, y2, x3, y3;
			fgets(line, sizeof(line), file);
			sscanf(line, "%f %f %f %f %f %f %f %f",
				&x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
			make_bezier(e, x0, y0, x1, y1, x2, y2, x3, y3);
		}
	}
	
	fclose(file);
	file = 0;
}

