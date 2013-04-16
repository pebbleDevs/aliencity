#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

// use this string to remove this app 5B0E70CD49774440BB081A223D90C9CF
#define MY_UUID {0x5B, 0x0E, 0x70, 0xCD, 0x49, 0x77, 0x44, 0x40, 0xBB, 0x08, 0x1A, 0x22, 0x3D, 0x90, 0xC9, 0xCF}
PBL_APP_INFO(MY_UUID, "AlienCity", "mapps", 1, 0 /* App version */, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

#define TOP_LABEL "  Pebble "

Window window;

TextLayer text_name_layer;
TextLayer text_date_layer;
TextLayer text_time_layer;

TextLayer text_day_layer; //todo


Layer line_layer;


void line_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_stroke_color(ctx, GColorBlack);
  
  graphics_draw_line(ctx, GPoint(8, 25), GPoint(131, 25));
  graphics_draw_line(ctx, GPoint(8, 26), GPoint(131, 26));
	
  graphics_draw_line(ctx, GPoint(8, 160), GPoint(131, 160));
  graphics_draw_line(ctx, GPoint(8, 161), GPoint(131, 161));

}


void handle_init(AppContextRef ctx) {
  (void)ctx;
// 140 186 068 37 century link ...  bills 
  window_init(&window, "AlienCity");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorWhite);

  resource_init_current_app(&APP_RESOURCES);

 //Title label
  text_layer_init(&text_name_layer, window.layer.frame);
  text_layer_set_text_color(&text_name_layer, GColorBlack);
  text_layer_set_background_color(&text_name_layer, GColorClear);
  layer_set_frame(&text_name_layer.layer, GRect(35, 0, 90, 168-145));
  text_layer_set_font(&text_name_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_name_layer.layer);
  
   //Day label
  text_layer_init(&text_day_layer, window.layer.frame);
  text_layer_set_text_color(&text_day_layer, GColorBlack);
  text_layer_set_background_color(&text_day_layer, GColorClear);
  layer_set_frame(&text_day_layer.layer, GRect(55, 90, 144-20, 168-20));
  text_layer_set_font(&text_day_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_day_layer.layer);

// date label
  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorBlack);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  layer_set_frame(&text_date_layer.layer, GRect(38, 130, 144-8, 168-68));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_date_layer.layer);

// time label
  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorBlack);
  text_layer_set_background_color(&text_time_layer, GColorClear);  
  layer_set_frame(&text_time_layer.layer, GRect(7, 30, 144-7, 168-40));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(&window.layer, &text_time_layer.layer);


  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);


  // TODO: Update display here to avoid blank display on launch?
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";
  static char new_date_text[] = "Xxxxxxxxx 00";
  static char day_text[] = "SUN";
  static char name_text[] = TOP_LABEL;
  
  char *full_day_text; //Holding full name of day

  char *time_format;


  // TODO: Only update the date when it's changed.
  string_format_time(date_text, sizeof(date_text), "%B %e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);
  text_layer_set_text(&text_name_layer, name_text);
  string_format_time(day_text,sizeof(day_text), "%A", t->tick_time);
  // full day name 
  if(strncmp(day_text,"Tue", 3) == 0)
  {
  	full_day_text = "Tuesday";
  }
  
  text_layer_set_text(&text_day_layer, day_text);
 
 
  
 /* if(strncmp(new_date_text, date_text, sizeof(date_text)) != 0)
  {
  	strncpy(date_text, new_date_text, sizeof(date_text));
  	string_format_time(date_text, sizeof(date_text), "%B %e", t->tick_time);
    text_layer_set_text(&text_date_layer, date_text);
    text_layer_set_text(&text_name_layer, name_text);
    string_format_time(day_text, sizeof(day_text), "%A", t->tick_time);
    text_layer_set_text(&text_day_layer, day_text);
	}*/

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
   // memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

 

  text_layer_set_text(&text_time_layer, time_text); //displaying time on peb

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
