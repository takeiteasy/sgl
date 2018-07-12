#include "graphics.h"

int invert(int x, int y, int c) {
  return RGB(255 - R(c), 255 - G(c), 255 - B(c));
}

int greyscale(int x, int y, int c) {
  int gc = 0.2126 * R(c) + 0.7152 * G(c) + 0.0722 * B(c);
  return RGB(gc, gc, gc);
}

#define RND_255 (rand() % 256)

int rnd(int x, int y, int c) {
  return RGB(RND_255, RND_255, RND_255);
}

int remove_lime(int x, int y, int c) {
  return (c == LIME ? 0 : c);
}

static surface_t win;
static int win_w = 575, win_h = 500, mx = 0, my = 0, running = 1;

#define DEBUG_NATIVE_RESIZE 0
#define SKIP_PRINTF 0

void update_mxy() {
  mouse_xy(&mx, &my);
#if DEBUG_NATIVE_RESIZE
  mx = (int)((mx / win_w) * win.w);
  my = (int)((my / win_h) * win.h);
#endif
}

void on_resize(int w, int h) {
  win_w = w;
  win_h = h;
#if !DEBUG_NATIVE_RESIZE
  reset(&win, w, h);
#else
  cls(&win);
#endif
  writelnf(&win, 4, 5, WHITE, -1, "%dx%d\n", w, h);
}

#if !defined(RES_PATH)
#if defined(__APPLE__)
#define RES_PATH "/Users/roryb/Documents/git/graphics.h/tests/"
#elif defined(_WIN32)
#define RES_PATH "C:\\Users\\DESKTOP\\Documents\\graphics.h\\tests\\"
#else
#define RES_PATH "/home/reimu/Desktop/graphics.h/tests/"
#endif
#endif
#define RES_JOIN(X,Y) (X Y)
#define RES(X) (RES_JOIN(RES_PATH, X))

#if SKIP_PRINTF
#define printf(fmt, ...) (0)
#endif

void on_error(ERRPRIO pri, const char* msg, const char* file, const char* func, int line) {
  fprintf(stderr, "ERROR ENCOUNTERED: %s\nFrom %s, in %s() at %d\n", msg, file, func, line);
  if (pri == PRIO_HIGH || pri == PRIO_NORM)
    abort();
}

#define D2R(a) ((a) * M_PI / 180.0)
#define R2D(a) ((a) * 180.0 / M_PI)

int blit_rotate_test(surface_t* dst, point_t* p, surface_t* src, float theta) {
  int offset_x = 0, offset_y = 0,
  from_x = 0, from_y = 0,
  width = src->w, height = src->h;
  if (p) {
    offset_x = p->x;
    offset_y = p->y;
  }
  
  if (offset_x < 0) {
    from_x += abs(offset_x);
    width -= abs(offset_x);
    offset_x = 0;
  }
  if (offset_y < 0) {
    from_y += abs(offset_y);
    height -= abs(offset_y);
    offset_y = 0;
  }
  
  int to_x = offset_x + width, to_y = offset_y + height;
  if (to_x > dst->w)
    width += (dst->w - to_x);
  if (to_y > dst->h)
    height += (dst->h - to_y);
  
  if (offset_x > dst->w || offset_y > dst->h || to_x < 0 || to_y < 0)
    return 0;
  
  
//  p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
//  p'y = sin(theta) * (px-ox) + cos(theta) * (py-oy) + oy
  
  int ox = offset_x + (src->w / 2);
  int oy = offset_y + (src->h / 2);
  theta = D2R(theta);
  
  int x, y, c;
  for (x = 0; x < width; ++x)
    for (y = 0; y < height; ++y) {
      int px = offset_x + x;
      int py = offset_y + y;
      int rx = cosf(theta) * (px-ox) - sinf(theta) * (py-oy) + ox;
      int ry = sinf(theta) * (px-ox) + cosf(theta) * (py-oy) + oy;
      pset(dst, rx, ry, pget(src, x, y));
    }
  return 1;
}

int main(int argc, const char* argv[]) {
  screen("test", &win, &win_w, &win_h, RESIZABLE);
  resize_callback(on_resize);
  error_callback(on_error);

  surface_t s[10];
  surface(&s[0], 50, 50);
  
  image(&s[1], RES("test_alpha.png"));
  
  bmp(&s[6], RES("lena.bmp"));
  resize(&s[6], s[6].w / 2, s[6].h / 2, &s[2]);
  destroy(&s[6]);
  
  // BDF font from tewi-font: https://github.com/lucy/tewi-font
  bdf_t tewi;
  bdf(&tewi, RES("tewi.bdf"));
  
  copy(&s[2], &s[4]);
  filter(&s[4], invert);
  
  point_t points[9] = {
    { 10,  150 },
    { 5,   227 },
    { 350, 125 },
    { 0,   0   },
    { 10,  110 },
    { 425, 110 },
    { 482, 170 },
    { 530, 370 },
    { 100, 20  }
  };
  points[3].x = points[1].x + s[2].w + 5;
  points[3].y = points[1].y;

  rect_t cutr  = { 125, 120, 50, 50 };
#if !defined(GRAPHICS_DISABLE_RGBA)
  stringf(&s[3], RED, 0, "cut from the\nimage below\nx: %d y: %d\nw: %d h: %d", cutr.x, cutr.y, cutr.w, cutr.h);
#else
  stringf(&s[3], RED, LIME, "cut from the\nimage below\nx: %d y: %d\nw: %d h: %d", cutr.x, cutr.y, cutr.w, cutr.h);
#endif
  
  surface(&s[9], 50, 50);
  fill(&s[9], BLACK);
  writeln(&s[9], 13, 20, LIME, BLACK, "WOW");
#if !defined(GRAPHICS_DISABLE_RGBA)
  filter(&s[9], remove_lime);
#endif

  surface(&s[5], 100, 100);
  rect(&s[5], 0,  0,  50, 50, RGBA(255, 0, 0, 128), 1);
  rect(&s[5], 50, 50, 50, 50, RGBA(0, 255, 0, 128), 1);
  rect(&s[5], 50, 0,  50, 50, RGBA(0, 0, 255, 128), 1);
  rect(&s[5], 0,  50, 50, 50, RGBA(255, 255, 0, 128), 1);
  
  float theta = 1.f;
  int ytest = 1;
  
  int col = 0, grey = 0;
  long sine_i = 0;
  event_t e;
  long prev_frame_tick;
  long curr_frame_tick = ticks();
  while (!closed() && running) {
    prev_frame_tick = curr_frame_tick;
    curr_frame_tick = ticks();

    while (poll(&e)) {
      switch (e.type) {
        case WINDOW_CLOSED:
          running = 0;
          break;
        case KEYBOARD_KEY_DOWN:
          switch (e.sym) {
#if defined(__APPLE__)
            case KB_KEY_Q:
              if (e.mod & KB_MOD_SUPER)
                running = 0;
              break;
#else
            case KB_KEY_F4:
              if (ue.mod & KB_MOD_ALT)
                running = 0;
              break;
#endif
            case KB_KEY_SPACE:
              grey = 1;
              break;
            default:
              break;
          }
          break;
        case KEYBOARD_KEY_UP:
          switch (e.sym) {
            case KB_KEY_SPACE:
              grey = 0;
              break;
            case KB_KEY_F1:
              save_bmp(&win, "test.bmp");
              break;
            case KB_KEY_F2:
              save_image(&win, "test.png");
              break;
            default:
              break;
          }
        default:
          break;
      }
    }
    
    long speed = curr_frame_tick - prev_frame_tick;

//    cls(&win);
    fill(&win, WHITE);
    
    for (int x = 32; x < win.w; x += 32)
      vline(&win, x, 0, win.h, GRAY);
    for (int y = 32; y < win.h; y += 32)
      hline(&win, y, 0, win.w, GRAY);

    blit(&win, &points[8], &s[1], NULL);

    writeln(&win, 10, 10, RED, -1, "Hello World");
    writeln(&win, 10, 22, MAROON, -1, "こんにちは");
    bdf_writeln(&win, &tewi, 10, 34, WHITE, BLACK, "ΔhelloΔ bdf!");

    int last_x = 0, last_y = 200;
    for (long i = sine_i; i < (sine_i + win.w); ++i) {
      float x = (float)(i - sine_i);
      float y = 200.f + (75.f * sinf(i * (3.141f / 180.f)));
      line(&win, last_x, last_y, x, y, col);
      last_x = x;
      last_y = y;
    }
    sine_i += (int)(speed * .2);

    blit(&win, &points[4], &s[3], NULL);
    blit(&win, &points[0], &s[2], &cutr);

    blit(&win, &points[1], &s[2], NULL);
    blit(&win, &points[3], &s[4], NULL);

    blit_rotate_test(&win, &points[5], &s[5], theta);
//    blit(&win, &points[5], &s[5], NULL);

    filter(&s[0], rnd);
    blit(&s[0], NULL, &s[9], NULL);
    blit(&win, &points[2], &s[0], NULL);

    circle(&win, 352, 32, 30, RED,    1);
    circle(&win, 382, 32, 30, ORANGE, 1);
    circle(&win, 412, 32, 30, YELLOW, 1);
    circle(&win, 442, 32, 30, LIME,   1);
    circle(&win, 472, 32, 30, BLUE,   1);
    circle(&win, 502, 32, 30, INDIGO, 1);
    circle(&win, 532, 32, 30, VIOLET, 1);

    update_mxy();
    writelnf(&win, 400, 88, BLACK, -1, "mouse x,y: (%d, %d)\ntheta: %f", mx, my, theta);
    col = pget(&win, mx, my);

    line(&win, 0, 0, mx, my, col);
    circle(&win, mx, my, 30, col, 0);

    if (grey)
      filter(&win, greyscale);
    
    float t = D2R(theta);
    point_t a = { 200, 200 };
    point_t b = { 200, 150 };
    point_t c = {
      cosf(t) * (b.x - a.x) - sinf(t) * (b.y - a.y) + a.x,
      sinf(t) * (b.x - a.x) + cosf(t) * (b.y - a.y) + a.y
    };
    
    circle(&win, a.x, a.y, 3, RED, 1);
    circle(&win, b.x, b.y, 3, BLUE, 1);
    circle(&win, c.x, c.y, 3, LIME, 1);
    
    theta += (.05f * speed);
    if (theta >= 360.f)
      theta = 0.f;

    flush(&win);
  }

  bdf_destroy(&tewi);
  destroy(&win);
  for (int i = 0; i < (int)(sizeof(s) / sizeof(s[0])); ++i)
    destroy(&s[i]);
  release();
  return 0;
}
