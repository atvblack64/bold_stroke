#pragma once
void settings_window_init();
void settings_window_deinit();
void settings_window_select(ClickRecognizerRef ref, void *context);
void settings_window_back(ClickRecognizerRef ref, void *context);
void load_settings(bool learnmode_disabled, int X, int Y, int curX, int curY, int defX, int defY, int vibe_lock);
int *save_settings();
bool settings_window_exists();
bool learningmode_enabled;
int posX;
int posY;
int curposX;
int curposY;
int defX;
int defY;
int vibe_lock;