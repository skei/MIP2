
#include "mip.h"

//#include <GL/gl.h>
#include <GL/glew.h>


//// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"


#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_FONT_BAKING
#define NK_IMPLEMENTATION
#include "extern/nuklear.h"

#pragma GCC diagnostic pop

int main() {

  // init gui state

  #define MAX_MEMORY (1024 * 1024)
  nk_user_font font;

  enum {EASY, HARD};
  static int op = EASY;
  static float value = 0.6f;
  static int i =  20;
  struct nk_context ctx;

  nk_init_default(&ctx, &font);

  if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
    // fixed widget pixel width
    nk_layout_row_static(&ctx, 30, 80, 1);
    if (nk_button_label(&ctx, "button")) {
      // event handling
    }
    // fixed widget window ratio width
    nk_layout_row_dynamic(&ctx, 30, 2);
    if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
    if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;
    // custom widget pixel width
    nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
    {
      nk_layout_row_push(&ctx, 50);
      nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
      nk_layout_row_push(&ctx, 110);
      nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
    }
    nk_layout_row_end(&ctx);
  }
  nk_end(&ctx);
  return 0;
}
