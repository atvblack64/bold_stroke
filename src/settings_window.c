#include <pebble.h>
#include "settings_window.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

static MenuLayer *menu_layer;
static Window *settings_window;
bool settings_exists = 0;

bool settings_window_exists(){
	return settings_exists;
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
	return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	switch (section_index) {
		case 0:
			return NUM_FIRST_MENU_ITEMS;
		default:
			return 0;
	}
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	switch (section_index) {
		case 0:
			menu_cell_basic_header_draw(ctx, cell_layer, "Settings");
			break;
	}
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	static char pos_text[32];
	static char vibe_text[32];
	switch (cell_index->section) {
		case 0:
			switch (cell_index->row) {
				case 0:
					if(learningmode_enabled){
						menu_cell_basic_draw(ctx, cell_layer, "Learning Mode", "Enabled", NULL);
					}
					else{
						menu_cell_basic_draw(ctx, cell_layer, "Learning Mode", "Disabled", NULL);
						posX = defX;
						posY = defY;
					}
					break;
				case 1:
					if(learningmode_enabled){
						snprintf(pos_text, sizeof(pos_text), "X= %d  Y= %d", posX, posY);
					}
					else{
						snprintf(pos_text, sizeof(pos_text), "X= %d  Y= %d", defX, defY);
					}
					menu_cell_basic_draw(ctx, cell_layer, "Learning Position",  pos_text, NULL);
					break;
				case 2:
					snprintf(vibe_text, sizeof(vibe_text), "%d", vibe_lock);
					menu_cell_basic_draw(ctx, cell_layer, "Vibes Lock", vibe_text, NULL);
					break;
			}
			break;
	}
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	switch (cell_index->row) {
		case 0:
			learningmode_enabled = learningmode_enabled ? false : true;
			layer_mark_dirty(menu_layer_get_layer(menu_layer));
			break;
		case 1:
			if(learningmode_enabled){
				posX = curposX;
				posY = curposY;
				layer_mark_dirty(menu_layer_get_layer(menu_layer));
			}
			break;
		case 2:
			vibe_lock+=5;
			if(vibe_lock>30){
				vibe_lock = 1;
			}
			else if(vibe_lock==6){
				vibe_lock = 5;
			}
			layer_mark_dirty(menu_layer_get_layer(menu_layer));
			break;
	}
}

void load_settings(bool learnmode_disabled, int x, int y, int curx, int cury, int defaultX, int defaultY, int vibelock){
	learningmode_enabled = learnmode_disabled;
	if(!posX){posX = x;}
	if(!posY){posY = y;}
	curposX = curx;
	curposY = cury;
	defX = defaultX;
	defY = defaultY;
	vibe_lock = vibelock;
}

int *save_settings(){
	static int b[4];
	b[0] = learningmode_enabled;
	b[1] = posX;
	b[2] = posY;
	b[3] = vibe_lock;
	return b;
}

void settings_window_select(ClickRecognizerRef ref, void *context){
	window_stack_push(settings_window, true);
}


void settings_window_back(ClickRecognizerRef ref, void *context){
	//Locked
}

void window_load_settings(Window *w){
	settings_exists = true;
	Layer *window_layer = window_get_root_layer(w);
	GRect bounds = layer_get_frame(window_layer);
	menu_layer = menu_layer_create(bounds);
	menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
	.get_num_sections = menu_get_num_sections_callback,
	.get_num_rows = menu_get_num_rows_callback,
	.get_header_height = menu_get_header_height_callback,
	.draw_header = menu_draw_header_callback,
	.draw_row = menu_draw_row_callback,
	.select_click = menu_select_callback,
	});
	menu_layer_set_click_config_onto_window(menu_layer, w);
	layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload_settings(Window *w){
	menu_layer_destroy(menu_layer);
	settings_exists = false;
}

void settings_window_init(){
	settings_window = window_create();
	window_set_window_handlers(settings_window, (WindowHandlers){
		.load = window_load_settings,
		.unload = window_unload_settings,
	});
}

void settings_window_deinit(){
	window_destroy(settings_window);
}