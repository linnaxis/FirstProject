#include <pebble.h>
 
Window *window;
TextLayer *time_text_layer, *meridian_text_layer, *date_text_layer, *day_text_layer;
InverterLayer *InvLayers[5];
char time_buffer[] = "00:00";
char meridian_buffer[] = "AM";
char day_buffer[] = "Wednesday";
char date_buffer[] = "Apr 30";


 
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Here we will update the watchface display
  //Format the buffer string using tick_time as the time source
  strftime(time_buffer, sizeof(time_buffer), "%l:%M", tick_time);
  strftime(meridian_buffer, sizeof(meridian_buffer), "%p", tick_time);
  strftime(day_buffer, sizeof(day_buffer), "%a", tick_time);
  strftime(date_buffer, sizeof(date_buffer), "%b %e", tick_time);
  
  text_layer_set_text(time_text_layer, time_buffer);
  text_layer_set_text(meridian_text_layer, meridian_buffer);
  text_layer_set_text(day_text_layer, day_buffer);
  text_layer_set_text(date_text_layer, date_buffer);
  
  int seconds;
  //seconds = (int)(tick_time->tm_sec * (72/60)); 
  //layer_set_frame((Layer *)inv_layer, GRect((72 - seconds), 60, (seconds * 2) , 35));
  seconds = tick_time->tm_sec; 
  switch (seconds)
  {
    case 1:
      for (int i=0;i<1;i++)
      {
        layer_set_hidden((Layer *) InvLayers[i], false);
      } 
      break;
    
    case 12:
      for (int i=0;i<2;i++)
      {
        layer_set_hidden((Layer *) InvLayers[i], false);
      } 
      break;
    
    case 24:
      for (int i=0;i<3;i++)
      {
        layer_set_hidden((Layer *) InvLayers[i], false);
      } 
      break;
    
    case 36:
      for (int i=0;i<4;i++)
      {
        layer_set_hidden((Layer *) InvLayers[i], false);
      } 
      break;
    
    case 48:
      for (int i=0;i<5;i++)
      {
        layer_set_hidden((Layer *) InvLayers[i], false);
      } 
      break;
    
    case 0:
      for (int i=0;i<5;i++)
      {
        layer_set_hidden((Layer*) InvLayers[i], true); 
      }
      break;
  }
}

void window_load(Window *window)
{
  //Load font
  ResHandle time_font_handle = resource_get_handle(RESOURCE_ID_FONT_MULI_REG_47);
  time_text_layer = text_layer_create(GRect(0, 30, 116, 55));
  text_layer_set_background_color(time_text_layer, GColorClear);
  text_layer_set_text_color(time_text_layer, GColorWhite);
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentLeft);
  text_layer_set_font(time_text_layer, fonts_load_custom_font(time_font_handle));
  
  ResHandle meridian_font_handle = resource_get_handle(RESOURCE_ID_FONT_MULI_REG_15);
  meridian_text_layer = text_layer_create(GRect(117, 60, 40, 35));
  text_layer_set_background_color(meridian_text_layer, GColorClear);
  text_layer_set_text_color(meridian_text_layer, GColorWhite);
  text_layer_set_text_alignment(meridian_text_layer, GTextAlignmentLeft);
  text_layer_set_font(meridian_text_layer, fonts_load_custom_font(meridian_font_handle));
  
  ResHandle day_font_handle = resource_get_handle(RESOURCE_ID_FONT_MULI_REG_20);
  day_text_layer = text_layer_create(GRect(0, 90, 50, 35));
  text_layer_set_background_color(day_text_layer, GColorClear);
  text_layer_set_text_color(day_text_layer, GColorWhite);
  text_layer_set_text_alignment(day_text_layer, GTextAlignmentLeft);
  text_layer_set_font(day_text_layer, fonts_load_custom_font(day_font_handle));
  
  ResHandle date_font_handle = resource_get_handle(RESOURCE_ID_FONT_MULI_REG_20);
  date_text_layer = text_layer_create(GRect(50, 90, 80, 35));
  text_layer_set_background_color(date_text_layer, GColorClear);
  text_layer_set_text_color(date_text_layer, GColorWhite);
  text_layer_set_text_alignment(date_text_layer, GTextAlignmentLeft);
  text_layer_set_font(date_text_layer, fonts_load_custom_font(date_font_handle));
  

  layer_add_child(window_get_root_layer(window), (Layer*) time_text_layer);
  layer_add_child(window_get_root_layer(window), (Layer*) meridian_text_layer);
  layer_add_child(window_get_root_layer(window), (Layer*) day_text_layer);
  layer_add_child(window_get_root_layer(window), (Layer*) date_text_layer);
  
  //Inverter layer
  int xloc = 3;
  int yloc = 5;
  
  for (int i=0;i<5;i++)
  {
    InvLayers[i] = inverter_layer_create(GRect(xloc, yloc, 25, 20));
    layer_set_hidden((Layer*) InvLayers[i], true); 
    layer_add_child(window_get_root_layer(window), (Layer*) InvLayers[i]);
    xloc +=28;
  }
      
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
  text_layer_destroy(time_text_layer);
  text_layer_destroy(meridian_text_layer);
  text_layer_destroy(day_text_layer);
  text_layer_destroy(date_text_layer);
  for (int i=0;i<5;i++)
  {
    inverter_layer_destroy(InvLayers[i]); 
  }

}


void init()
{
  //Initialize the app elements here!
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_set_background_color(window,GColorBlack);
  
  tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
  window_stack_push(window, true);
  
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  tick_timer_service_unsubscribe();
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}