#include <stdio.h>
#include <ncurses.h>

#define BLACK_RED 1

void plot(WINDOW* win, unsigned int x, unsigned int y, unsigned int colorpair)
{
	move(x, y);
	attrset(COLOR_PAIR(colorpair));
	addch(' ');
}

int main(void)
{
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

	// colors id, fg, bg
	init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);

	plot(stdscr, 1, 1, 1);

	getch();

	// teardown ncurses
	keypad(stdscr,FALSE);
	curs_set(1);
	echo();
	endwin();

	return 0;
}

