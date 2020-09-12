#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#define BLACK_RED 1
#define BLACK_GREEN 2
#define BLACK_BLUE 3

int ncurses_setup() {
	// setup ncurses window
	initscr();
	if (has_colors()) {
		start_color();
	} else {
		printf("%s\n", "Colors not supported on this terminal");
		endwin(); // cleanup
		return 1;
	}
	curs_set(0); // hide cursor
	noecho();
	keypad(stdscr, true); // turn on F key listening
	return 0;
}

void ncurses_teardown() {
	// teardown ncurses
	keypad(stdscr,FALSE);
	curs_set(1);
	echo();
	endwin();
}


struct Point {
	int coords[2];
};
struct Triangle {
	struct Point points[3];
};
struct SlopeData {
	double begin;
	double step;
};

void swap(int *xp, int *yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void plot(WINDOW* win, int x, int y, int colorpair) {
	attrset(COLOR_PAIR(colorpair));
	// times 2 plus 1 to render 1 coord as ascii square
	move(y, x * 2);
	addch(' ');
	move(y, x * 2 + 1);
	addch(' ');
}

void draw_scanline(WINDOW* win, int y, int x1, int x2, int cp) {
	for (int x=x1; x < x2; x++) {
		plot(win, x, y, cp);
	}
}

struct SlopeData make_slope(int x0, int y0, int x1, int y1, int numsteps) {
	int begin = x0;
	int end = x1;
	double invsteps = 1.0 / (double)numsteps;
	struct SlopeData s = {begin, (end - begin) * invsteps};
	return s;
}

void rasterize_triangle(WINDOW* win, struct Triangle *t, int colorpair) {
	int x0 = t->points[0].coords[0];
	int y0 = t->points[0].coords[1];
	int x1 = t->points[1].coords[0];
	int y1 = t->points[1].coords[1];
	int x2 = t->points[2].coords[0];
	int y2 = t->points[2].coords[1];

	// order points by y coordinate descending, top point first
	// when y is equal order by xcoords (todo)
	if (y1 < y0) {
		swap(&x0,&x1); swap(&y0,&y1);// swap(p0,p1)
	}
	if (y2 < y0) {
		swap(&x0,&x2); swap(&y0,&y2);// swap(p0,p2)
	}
	if (y2 < y1) {
		swap(&x1,&x2); swap(&y1,&y2);// swap(p1,p2)
	}

	if (y0 == y2) return;

	// determine if short side is left or right
	// false isleft side, true is right side
	bool shortside = (y1-y0) * (x2-x0) < (x1-x0) * (y2-y0);

	// create 2 slopes, one is full height of triangle, other is sides
	// p0-p1 (short) and p0-p2 (long)
	struct SlopeData sides[2];
	sides[!shortside] = make_slope(x0,y0, x2,y2, y2-y0);

	// main raster loop
	if (y0 < y1) {
		sides[shortside] = make_slope(x0,y0, x1,y1, y1-y0);
		for (int y=y0; y < y1; ++y) {
			draw_scanline(win, y, sides[0].begin, sides[1].begin, colorpair);
			sides[0].begin += sides[0].step;
			sides[1].begin += sides[1].step;
		}
	}
	if (y1 < y2) {
		sides[shortside] = make_slope(x1,y1, x2,y2, y2-y1);
		for (int y=y1; y < y2; ++y) {
			draw_scanline(win, y, sides[0].begin, sides[1].begin, colorpair);
			sides[0].begin += sides[0].step;
			sides[1].begin += sides[1].step;
		}
	}
}

int main(void) {
	int exit = ncurses_setup();
	if (exit)
		return exit;

	// colors id, fg, bg
	init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
	init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);

	// build triangle
	struct Triangle t;
	t.points[0].coords[0] = 2;
	t.points[0].coords[1] = 2;

	t.points[1].coords[0] = 19;
	t.points[1].coords[1] = 4;

	t.points[2].coords[0] = 6;
	t.points[2].coords[1] = 22;

	// loop render
	do {
		//plot(stdscr, 1, 1, BLACK_GREEN);
		//draw_scanline(stdscr, 10, 3, 19, BLACK_BLUE);
		rasterize_triangle(stdscr, &t, BLACK_RED);
	} while (getch() != 'x');


	ncurses_teardown();
	return 0;
}
