#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_digit_container[4];
static GFont s_time_font;

static const char digit_strings[10][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const char* get_digit_string(char c) {
  return digit_strings[c - '0'];
}

static void time_window_load(Window *window) {
  // top hour layers
  s_digit_container[0] = text_layer_create(GRect(0, 0, 72, 84));
  s_digit_container[1] = text_layer_create(GRect(72, 0, 72, 84));
  
  // bottom minute layers
  s_digit_container[2] = text_layer_create(GRect(0, 84, 72, 84));
  s_digit_container[3] = text_layer_create(GRect(72, 84, 72, 84));
  
  text_layer_set_background_color(s_digit_container[0], GColorBlack);
  text_layer_set_background_color(s_digit_container[1], GColorBlack);
  text_layer_set_background_color(s_digit_container[2], GColorBlack);
  text_layer_set_background_color(s_digit_container[3], GColorBlack);
  text_layer_set_text_color(s_digit_container[0], GColorLightGray);
  text_layer_set_text_color(s_digit_container[1], GColorLightGray);
  text_layer_set_text_color(s_digit_container[2], GColorLightGray);
  text_layer_set_text_color(s_digit_container[3], GColorLightGray);
  text_layer_set_text(s_digit_container[0], "0");
  text_layer_set_text(s_digit_container[1], "0");
  text_layer_set_text(s_digit_container[2], "0");
  text_layer_set_text(s_digit_container[3], "0");
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FORCED_SQUARE_42));;

  text_layer_set_font(s_digit_container[0], s_time_font);
  text_layer_set_font(s_digit_container[1], s_time_font);
  text_layer_set_font(s_digit_container[2], s_time_font);
  text_layer_set_font(s_digit_container[3], s_time_font);
  text_layer_set_text_alignment(s_digit_container[0], GTextAlignmentCenter);
  text_layer_set_text_alignment(s_digit_container[1], GTextAlignmentCenter);
  text_layer_set_text_alignment(s_digit_container[2], GTextAlignmentCenter);
  text_layer_set_text_alignment(s_digit_container[3], GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_digit_container[0]));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_digit_container[1]));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_digit_container[2]));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_digit_container[3]));
}

static void time_window_unload(Window *window) {
  text_layer_destroy(s_digit_container[0]);
  text_layer_destroy(s_digit_container[1]);
  text_layer_destroy(s_digit_container[2]);
  text_layer_destroy(s_digit_container[3]);
  fonts_unload_custom_font(s_time_font);
}

static void update_time(struct tm *tick_time) {
  // Create a long-lived buffer
  static char time[] = {'0', '0', '0', '0', '0'};

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(time, 5, "%H%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time, 5, "%I%M", tick_time);
  }

  // Display this time on the TextLayer
  
  for (int i = 0; i < 4; i++) {
    text_layer_set_text(s_digit_container[i], get_digit_string(time[i]));
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}


static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = time_window_load,
    .unload = time_window_unload
  });
  
  //window_set_background_color(s_main_window, GColorBlack);
  // true for animation
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}