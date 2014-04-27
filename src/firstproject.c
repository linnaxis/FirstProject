#include <pebble.h>
 
Window *window;
TextLayer *text_layer;
char buffer[] = "00:00";
InverterLayer *inv_layer;
 
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Here we will update the watchface display
  //Format the buffer string using tick_time as the time source
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
 
  //Change the TextLayer text to show the new time!
  text_layer_set_text(text_layer, buffer);
  
  int seconds;
  seconds = (int)tick_time->tm_sec * (72/60); 
  layer_set_frame((Layer *)inv_layer, GRect((72 - seconds), 60, (seconds * 2) , 35));
}

void window_load(Window *window)
{
  //We will add the creation of the Window's elements here soon!
  //Load font
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_FONT_MULI_REG_30);
  text_layer = text_layer_create(GRect(0, 58, 132, 40));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
 
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  
  //Inverter layer
  inv_layer = inverter_layer_create(GRect(0, 0, 0, 0));
  layer_add_child(window_get_root_layer(window), (Layer*) inv_layer);
  
  //Get a time structure so that the face doesn't start blank
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);
   
  //Manually call the tick handler when the window is loading
  tick_handler(t, SECOND_UNIT);

}
 
void window_unload(Window *window)
{
  //We will safely destroy the Window's elements here!
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inv_layer);

}


void init()
{
  //Initialize the app elements here!
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
  window_stack_push(window, true);
  
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  text_layer_destroy(text_layer);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}