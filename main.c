#include <stdio.h>
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

void plot(WINDOW* win, unsigned int x, unsigned int y, unsigned int colorpair) {
	move(x, y);
	attrset(COLOR_PAIR(colorpair));
	addch(' ');
}

int main(void) {
	int exit = ncurses_setup();
	if (exit)
		return exit;

	// colors id, fg, bg
	init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
	init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);

	// loop render
	do {
		plot(stdscr, 1, 1, BLACK_GREEN);
	} while (getch() != 'x');


	ncurses_teardown();
	return 0;
}

