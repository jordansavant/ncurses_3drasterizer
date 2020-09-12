# NCurses 3D Rasterizer

I was watching Bisqwits videos on 3d rendering, and I have dabbled in similar experiences. See my Doom in Python project.

Recently I have been doing a lot of C and NCurses for a roguelike game and decided I wanted to put his 3D rasterization work into NCurses for fun.

Of course colors is a trick unless you compile NCurses with 256 colors, so it will be limited to main colors with ascii characters to shade it.

`gcc main_rasterize_triangle.c -lncurses && ./a.out` 

![](https://github.com/jordansavant/ncurses_3drasterizer/raw/master/NCursesTriangles.png)
