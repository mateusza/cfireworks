#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define FIREWORKS 100
#define G 1.8

#define a_sin(x) ( SINE[(int)( (720 + (x)) % 360 )] )
#define a_cos(x) ( SINE[(int)( (720+(x)+90) % 360 )] )

double SINE[360] = { 0.000, 0.017, 0.035, 0.052, 0.070, 0.087, 0.105, 0.122, 0.139, 0.156, 0.174, 0.191, 0.208, 0.225, 0.242, 0.259, 0.276, 0.292, 0.309, 0.326, 0.342, 0.358, 0.375, 0.391, 0.407, 0.423, 0.438, 0.454, 0.469, 0.485, 0.500, 0.515, 0.530, 0.545, 0.559, 0.574, 0.588, 0.602, 0.616, 0.629, 0.643, 0.656, 0.669, 0.682, 0.695, 0.707, 0.719, 0.731, 0.743, 0.755, 0.766, 0.777, 0.788, 0.799, 0.809, 0.819, 0.829, 0.839, 0.848, 0.857, 0.866, 0.875, 0.883, 0.891, 0.899, 0.906, 0.914, 0.921, 0.927, 0.934, 0.940, 0.946, 0.951, 0.956, 0.961, 0.966, 0.970, 0.974, 0.978, 0.982, 0.985, 0.988, 0.990, 0.993, 0.995, 0.996, 0.998, 0.999, 0.999, 1.000, 1.000, 1.000, 0.999, 0.999, 0.998, 0.996, 0.995, 0.993, 0.990, 0.988, 0.985, 0.982, 0.978, 0.974, 0.970, 0.966, 0.961, 0.956, 0.951, 0.946, 0.940, 0.934, 0.927, 0.921, 0.914, 0.906, 0.899, 0.891, 0.883, 0.875, 0.866, 0.857, 0.848, 0.839, 0.829, 0.819, 0.809, 0.799, 0.788, 0.777, 0.766, 0.755, 0.743, 0.731, 0.719, 0.707, 0.695, 0.682, 0.669, 0.656, 0.643, 0.629, 0.616, 0.602, 0.588, 0.574, 0.559, 0.545, 0.530, 0.515, 0.500, 0.485, 0.469, 0.454, 0.438, 0.423, 0.407, 0.391, 0.375, 0.358, 0.342, 0.326, 0.309, 0.292, 0.276, 0.259, 0.242, 0.225, 0.208, 0.191, 0.174, 0.156, 0.139, 0.122, 0.105, 0.087, 0.070, 0.052, 0.035, 0.017, 0.000, -0.017, -0.035, -0.052, -0.070, -0.087, -0.105, -0.122, -0.139, -0.156, -0.174, -0.191, -0.208, -0.225, -0.242, -0.259, -0.276, -0.292, -0.309, -0.326, -0.342, -0.358, -0.375, -0.391, -0.407, -0.423, -0.438, -0.454, -0.469, -0.485, -0.500, -0.515, -0.530, -0.545, -0.559, -0.574, -0.588, -0.602, -0.616, -0.629, -0.643, -0.656, -0.669, -0.682, -0.695, -0.707, -0.719, -0.731, -0.743, -0.755, -0.766, -0.777, -0.788, -0.799, -0.809, -0.819, -0.829, -0.839, -0.848, -0.857, -0.866, -0.875, -0.883, -0.891, -0.899, -0.906, -0.914, -0.921, -0.927, -0.934, -0.940, -0.946, -0.951, -0.956, -0.961, -0.966, -0.970, -0.974, -0.978, -0.982, -0.985, -0.988, -0.990, -0.993, -0.995, -0.996, -0.998, -0.999, -0.999, -1.000, -1.000, -1.000, -0.999, -0.999, -0.998, -0.996, -0.995, -0.993, -0.990, -0.988, -0.985, -0.982, -0.978, -0.974, -0.970, -0.966, -0.961, -0.956, -0.951, -0.946, -0.940, -0.934, -0.927, -0.921, -0.914, -0.906, -0.899, -0.891, -0.883, -0.875, -0.866, -0.857, -0.848, -0.839, -0.829, -0.819, -0.809, -0.799, -0.788, -0.777, -0.766, -0.755, -0.743, -0.731, -0.719, -0.707, -0.695, -0.682, -0.669, -0.656, -0.643, -0.629, -0.616, -0.602, -0.588, -0.574, -0.559, -0.545, -0.530, -0.515, -0.500, -0.485, -0.469, -0.454, -0.438, -0.423, -0.407, -0.391, -0.375, -0.358, -0.342, -0.326, -0.309, -0.292, -0.276, -0.259, -0.242, -0.225, -0.208, -0.191, -0.174, -0.156, -0.139, -0.122, -0.105, -0.087, -0.070, -0.052, -0.035, -0.017 };

int huetocolor( int hue ){
    int color;
    int x;
    hue %= 360;
    x = (int)( ((double)( hue % 60 ))/60.0*255.0 );
    
    if ( hue < 60 )  return 0xff0000 | ( x << 8 );
    if ( hue < 120 ) return 0x00ff00 | ( ( 255 - x ) << 16 );
    if ( hue < 180 ) return 0x00ff00 | ( x << 0 );
    if ( hue < 240 ) return 0x0000ff | ( ( 255 - x ) << 8 );
    if ( hue < 300 ) return 0x0000ff | ( x << 16 );
    if ( 1 )         return 0xff0000 | ( ( 255 - x ) << 0 );
}

typedef enum { INACTIVE, FLYING, FALLING } fw_state;

struct firework {
    fw_state state;
    int flares;
    int subflares;
    int subflare_levels;
    double hue;
    double huestep;
    double x;
    double y;
    double vx;
    double vy;
    double ax;
    double ay;
    double t_expl;
    double t_fade;
};

struct acell {
    int ch;
    int fg;
    int bg;
};

struct screen {
    int rows;
    int cols;
    struct acell *cells;
};

struct firework FW[FIREWORKS];
struct screen sc;

int mix_colors( int c1, int c2, int prop ){

    int r1,g1,b1,r2,g2,b2;

    r1 = c1 >> 16;
    r2 = c2 >> 16;
    g1 = (c1 >> 8)  & 0xff;
    g2 = (c2 >> 8)  & 0xff;
    b1 = (c1 )  & 0xff;
    b2 = (c2 )  & 0xff;

    return (
        (( prop * r1 + ( 0xff - prop ) * r2 ) / 510 )<<16 |
        (( prop * g1 + ( 0xff - prop ) * g2 ) / 510 )<<8 |
        (( prop * b1 + ( 0xff - prop ) * b2 ) / 510 )<<0
    );

}

int clear_screen(){
   int x,y;
   for ( y = 0; y < sc.rows; y++ ){
       for ( x = 0; x < sc.cols; x++ ){
            sc.cells[ y * sc.cols + x ].ch = ' ';
            sc.cells[ y * sc.cols + x ].fg = 0xffffff;
            sc.cells[ y * sc.cols + x ].bg = mix_colors( 0x000000, sc.cells[ y * sc.cols + x ].bg, 0x08 );
       }
   }
}

int flash_screen(){
   int x,y;
   for ( y = 0; y < sc.rows; y++ ){
       for ( x = 0; x < sc.cols; x++ ){
            sc.cells[ y * sc.cols + x ].bg = 0xffffff;
       }
   }
}

int draw_fireworks(){
    int i;
    for ( i = 0; i < FIREWORKS; i++ ){
        if ( INACTIVE == FW[i].state ) continue;
        int x,y,c,ch;
        x = (int)((FW[i].x));
        y = (int)((FW[i].y));
        c = huetocolor( FW[i].hue );
        if ( FLYING == FW[i].state ) ch = ':';
        if ( FALLING == FW[i].state ) ch = '*';
        if ( x < 0 ) continue;
        if ( x >= sc.cols ) continue;
        if ( y < 0 ) continue;
        if ( y >= sc.rows ) continue;
        sc.cells[ y * sc.cols + x ].ch = ch;
        sc.cells[ y * sc.cols + x ].fg = 0xffffff;
        sc.cells[ y * sc.cols + x ].bg = c;
    }
}

int draw_screen(){
   int x,y;
   int fg, fg2, bg, bg2;
   int cellid;
   printf("\e[H\e[3J");
   for ( y = 0; y <= sc.rows-1; y++ ){
       for ( x = 0; x <= sc.cols-1; x++ ){
           cellid = y * sc.cols + x;
           fg = sc.cells[ cellid ].fg;
           if ( cellid > 0 ) fg2 = sc.cells[ cellid - 1 ].fg;

           bg = sc.cells[ cellid ].bg;
           if ( cellid > 0 ) bg2 = sc.cells[ cellid - 1 ].bg;

           int ch = sc.cells[ cellid ].ch;
           if ( cellid > 0 && fg != fg2 ) printf( "\e[38;2;%d;%d;%dm", 
            fg >> 16, (fg >> 8) & 0xff, fg & 0xff
           );
           if ( cellid > 0 && bg != bg2 ) printf( "\e[48;2;%d;%d;%dm",
            bg >> 16, (bg >> 8) & 0xff, bg & 0xff
           );
           putchar(ch);
       }
       printf("\n");
   }
   printf("\n");
}

int free_firework(){
    int i;
    for ( i = 0; i < FIREWORKS; i++ ){
        if ( INACTIVE == FW[i].state ) return i;
    }
    for ( i = 0; i < FIREWORKS; i++ ){
        if ( FALLING == FW[i].state && FW[i].t_fade < 2.0 ) return i;
    }
    return rand() % FIREWORKS;
}

int main(){
    int i;

    for ( i=0; i<FIREWORKS; i++ ){
        FW[i].state = INACTIVE;
        FW[i].x = FW[i].y = FW[i].vx = FW[i].vy = FW[i].ax = FW[i].ay = 0.0;
    }

    struct timeval tv;
    double fps;

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    char timestr[1000];

    sc.rows = w.ws_row;
    sc.cols = w.ws_col;

    sc.cells = malloc ( sizeof( struct acell ) * sc.rows * sc.cols );

    double nowtime;
    double lasttime;

    gettimeofday( &tv, NULL );
    nowtime = tv.tv_sec + tv.tv_usec/1000000.0;
    lasttime = nowtime;
    int flash;
    while( 1 ){
        flash = 0;
        double dtime;
        gettimeofday( &tv, NULL );
        nowtime = tv.tv_sec + tv.tv_usec/1000000.0;
        dtime = ( nowtime - lasttime ) + 0.000001;
        if ( 0 == rand() % 10 ){
            int fnum = free_firework();
            FW[fnum].state = FLYING;
            FW[fnum].x = rand() % sc.cols;
            FW[fnum].y = sc.rows;
            FW[fnum].vx = 0.0;
            FW[fnum].vy = 0.0;
            FW[fnum].ax = (rand()%100)/100.0 - 0.5;
            FW[fnum].ay = 10.0;
            FW[fnum].hue = 20.0 + rand() % 20;
            FW[fnum].huestep = 7.0;
            FW[fnum].t_expl = 2.0 + (rand() % 100)/100.0;
            FW[fnum].flares = 6 + (rand() % 16);
        }

        for ( i=0; i<FIREWORKS; i++ ){
            if ( INACTIVE == FW[i].state ) continue;
            FW[i].x += FW[i].vx * dtime;
            FW[i].y += FW[i].vy * dtime;
            FW[i].vx += FW[i].ax * dtime;
            FW[i].vy -= ( FW[i].ay - G ) * dtime;
            FW[i].t_expl -= dtime;
            FW[i].t_fade -= dtime;
            FW[i].hue += FW[i].huestep;

            if ( FLYING == FW[i].state && FW[i].t_expl < 0.0 ){
                FW[i].state = INACTIVE;
                int j;
                double flarehue = rand()%360;
                double flarehuestep = 10.0* (( rand() % 100 ) / 50.0 - 1.0);
                flash = 1;
                for ( j=0; j<FW[i].flares; j++ ){
                    int fnum = free_firework();
                    double v_expl;
                    v_expl = 2.0 + ( rand() % 100 ) / 50.0;
                    FW[fnum].state = FALLING;
                    FW[fnum].x = FW[i].x; 
                    FW[fnum].y = FW[i].y;
                    FW[fnum].vx = v_expl * a_sin( 360*j/FW[i].flares );
                    FW[fnum].vy = v_expl * a_cos( 360*j/FW[i].flares );
                    FW[fnum].ax = 0.0;
                    FW[fnum].ay = 0.0;
                    FW[fnum].hue = flarehue;
                    FW[fnum].huestep = flarehuestep;
                    FW[fnum].t_fade = 5.0;
                }
            }

            if ( FALLING == FW[i].state && FW[i].t_fade < 0.0 ){
                FW[i].state = INACTIVE;
            }
        }

        lasttime = nowtime;
        /*
        printf("\e[H\e[J");
        printf("t: %d\n", tv.tv_sec );
        printf("u: %d\n", tv.tv_usec );
        printf("now: %lf\n", nowtime );
        printf("dtime: %lf (FPS: %d)\n", dtime, (int)( 1.0/dtime ) );
        for ( i=0; i<FIREWORKS; i++){
            if ( INACTIVE == FW[i].state ) continue;
            printf("[%3d] st=%d pos=(%8.3lf,%8.3lf) v=(%7.3lf,%7.3lf) a=(%7.3lf,%7.3lf) hue=%3d t_expl=%5.2lf t_fade=%5.2lf\n", 
                i, FW[i].state, FW[i].x, FW[i].y, FW[i].vx, FW[i].vy, FW[i].ax, FW[i].ay, FW[i].hue, FW[i].t_expl > 0.0 ? FW[i].t_expl : 0.0, FW[i].t_fade
            );
        }
        */
        clear_screen();
        draw_fireworks();
        if ( flash ){
            flash_screen();
        }
        draw_screen();

        fflush( stdout );
        usleep( 1000000 / 20 );
    }


}
