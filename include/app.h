#ifndef _APP_H_
#define _APP_H_

#include <core/core.h>

typedef struct AppConfig
{
    int window_width;
    int window_height;
    int window_fullscreen;
    int window_vsync;
} AppConfig;

typedef struct App
{
    AppConfig config;
    GLFWwindow *window;
    int running;
} App;

void app_load_config(const char *path, AppConfig *config);
void app_save_config(const char *path, const AppConfig *config);

int app_init(App *app, const AppConfig *config);
void app_destroy(App *app);

void app_run(App *app);

#endif // _APP_H_ 
