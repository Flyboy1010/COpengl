#include "app.h"

int main(int nargs, char** args)
{
	AppConfig config;

	config.window_fullscreen = 0;
	config.window_width = 1280;
	config.window_height = 720;
	config.window_vsync = 1;

	// load_game_config("config.cf", &config);

	App app;

	if (app_init(&app, &config) != -1)
	{
		app_run(&app);

		app_destroy(&app);
	}

    return 0;
}
