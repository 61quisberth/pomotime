/**
 * Based on the example implementation of the dialog choice UI pattern.
 */

#include <pebble.h>
#include "dialog_choice_window.h"
#include "timer/timer.h"

#define MAX_BUFF_LEN 8
#define TRUE 1
#define FALSE 0

static Window *s_main_window;
static TextLayer *s_label_layer;
static BitmapLayer *s_icon_layer;
static ActionBarLayer *s_action_bar_layer;
static char* curr_text;
char buffer[MAX_BUFF_LEN];
int timer_mode;

static GBitmap *s_icon_bitmap, *s_replay_bitmap, *s_play_bitmap, *s_pause_bitmap;
pomo_t timer;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  int is_time_out = update_timer(&timer);
  update_text(timer.min, timer.sec, is_time_out);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (has_started(timer) == FALSE) {
    reset_timer(&timer);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "timer started");
    set_started(&timer, TRUE);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timer resumed");
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  tick_timer_service_unsubscribe();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timer stopped");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  tick_timer_service_unsubscribe();
  reset_timer(&timer);
  update_text(timer.min, timer.sec, FALSE);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timer restarted");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TOMATO);

  const GEdgeInsets icon_insets = {.top = 7, .right = 28, .bottom = 56, .left = 14};
  s_icon_layer = bitmap_layer_create(grect_inset(bounds, icon_insets));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  const GEdgeInsets label_insets = {
    .top = 112,
    .right = ACTION_BAR_WIDTH,
    .left = ACTION_BAR_WIDTH / 2
  };
  s_label_layer = text_layer_create(grect_inset(bounds, label_insets));

  init_timer(&timer, timer_mode);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init timer min: %d, sec: %d", timer.min, timer.sec);
  update_text(timer.min, timer.sec, 0);

#if defined(PBL_COLOR)
  text_layer_set_text_color(s_label_layer, GColorWhite);
#else
  text_layer_set_text_color(s_label_layer, GColorBlack);
#endif
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));

  s_replay_bitmap = gbitmap_create_with_resource(RESOURCE_ID_REPLAY);
  s_play_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PLAY);
  s_pause_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PAUSE);

  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_background_color(s_action_bar_layer, GColorWhite);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_pause_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_play_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_replay_bitmap);
  action_bar_layer_add_to_window(s_action_bar_layer, window);

  action_bar_layer_set_click_config_provider(s_action_bar_layer, click_config_provider);
}

static void window_unload(Window *window) {
  if (has_started(timer) == TRUE)
    tick_timer_service_unsubscribe();

  text_layer_destroy(s_label_layer);
  action_bar_layer_destroy(s_action_bar_layer);
  bitmap_layer_destroy(s_icon_layer);

  gbitmap_destroy(s_icon_bitmap);
  gbitmap_destroy(s_replay_bitmap);
  gbitmap_destroy(s_play_bitmap);
  gbitmap_destroy(s_pause_bitmap);

  window_destroy(window);
  s_main_window = NULL;
}

void dialog_choice_window_push(int init_timer_mode) {
  if(!s_main_window) {
    timer_mode = init_timer_mode;
    s_main_window = window_create();
    window_set_background_color(s_main_window, PBL_IF_COLOR_ELSE(
      GColorOxfordBlue, GColorWhite));
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

void update_text(unsigned int min, unsigned int sec, int is_time_over) {
  if (is_time_over)
    snprintf(buffer, MAX_BUFF_LEN, "Done!");
  else {
    if (sec == 0)
      snprintf(buffer, MAX_BUFF_LEN, "%u:00", min);
    else if (sec < 10)
      snprintf(buffer, MAX_BUFF_LEN, "%u:0%u", min, sec);
    else
      snprintf(buffer, MAX_BUFF_LEN, "%u:%u", min, sec);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, buffer);
  text_layer_set_text(s_label_layer, buffer);
}
