#include"editor.h"

#define DRAW_GRID 0
#define DRAW_KEYFRAME_EDITOR 1
#define DRAW_SPLINE_EDITOR 1
#define DRAW_TIMELINE 1

static const char* keyframe_type_names[] = {
    "Enemy add",
    "Enemy change path",
    "Enemy destroy",
};

static SDL_Point window_size = {600, 600};

static spline_point_selected_t spline_point_selected;

static scene_t* editor_scene;

static SDL_Point camera = {0, 0};
static int camera_speed = 10;

static SDL_Point mouse = {0, 0};

static int mwheel_dir = 0;
static bool mwheel_this_frame = false;

static bool lclick_pressed = false;
static bool rclick_pressed = false;

static bool lclick_released = false;
static bool rclick_released = false;

static int selected_spline = -1;
static int selected_spline_point = -1;

static TTF_Font* ui_font = NULL;

static int timeline_tick = 0;

extern struct nk_colorf bg;

extern SDL_Window *window;
extern resources_t game_resources;


bool editor_start(){
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_MaximizeWindow(window);

    editor_scene = scene_create(&game_resources, "level.lvl");
    ui_font = game_resources.font;
    
#if 1
    const keyframe_t *keyframes = editor_scene->keyframes;
    const int keyframe_count = editor_scene->keyframe_count;

    printf("spline count: %d\n", editor_scene->spline_count);
    printf("keyframe count: %d\n", editor_scene->keyframe_count);

    for(int i = 0; i < keyframe_count; i++){
	printf("keyframe: %d\n", i);
	printf("---tick: %d\n", keyframes[i].tick);
	printf("---action: %d\n", keyframes[i].action);
	printf("\n");
    }
#endif

}

void editor_running(struct nk_context *gui_context){
    keyframe_t *all_keyframes = get_keyframes();
    const int k_count = get_keyframe_count();

    //get user input
    //mouse
    int mouse_click = SDL_GetMouseState(&mouse.x, &mouse.y);

    //keyboard
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    const SDL_Keymod modState = SDL_GetModState();

    //update
    if(keyboardState[SDL_SCANCODE_A]){
	camera.x -= camera_speed;
    }else if(keyboardState[SDL_SCANCODE_D]){
	camera.x += camera_speed;
    }

    if(keyboardState[SDL_SCANCODE_W]){
	camera.y -= camera_speed;
    }else if(keyboardState[SDL_SCANCODE_S]){
	camera.y += camera_speed;
    }

    if(keyboardState[SDL_SCANCODE_SPACE]){
	camera.x = camera_speed;
	camera.y = camera_speed;
    }

    //spline modifier
    if(selected_spline >= 0){
	spline_editor(mouse_click, modState);
    }

    //GUI
    int widgets_parameters = NK_WINDOW_BORDER| NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE;
    
    int win_x = 0;
    int win_y = 0;
    SDL_GetWindowSize(window, &win_x, &win_y);
    
    //TOP-BAR    
    #if 1
    struct nk_rect top_bar = nk_rect(0, 0, win_x, 100);
    nk_begin(gui_context,
	     "menu",
	     nk_rect(0, 0, win_x, win_y / 24),
	     NK_WINDOW_NO_SCROLLBAR);
    
    nk_layout_row_static(gui_context, 20, 50, 3);
    if(nk_menu_begin_label(gui_context, "File", NK_TEXT_CENTERED, nk_vec2(150, 400))){
	nk_layout_row_dynamic(gui_context, 20, 1);
	if(nk_button_label(gui_context, "New")){
	    editor_scene->current_keyframe = 0;
	    editor_scene->keyframe_count = 0;
	    memset(editor_scene->keyframes, 0, sizeof(keyframe_t) * MAX_KEYFRAMES);
	    
	    editor_scene->spline_count = 0;
	    memset(editor_scene->splines, 0, sizeof(spline_t) * MAX_SPLINES);
	}

	if(nk_button_label(gui_context, "Save")){
	    printf("level save\n");
	    editor_save_to_file();
	}
	
	if(nk_button_label(gui_context, "Load")){
	    editor_scene = scene_create(&game_resources, "editor.lvl");
	}
	
	nk_menu_end(gui_context);
    }

    if(nk_menu_begin_label(gui_context, "View", NK_TEXT_LEFT, nk_vec2(150, 400))){
	nk_layout_row_dynamic(gui_context, 20, 1);
	nk_button_label(gui_context, "1");
	nk_button_label(gui_context, "2");
	nk_button_label(gui_context, "3");
	nk_menu_end(gui_context);
    }    
    nk_end(gui_context);
    #endif
    
    //SPLINE EDITOR
    #if 1
    struct nk_rect spline_editor_rect = nk_rect(0, win_y / 24,
						win_x / 5, win_y);
    if (nk_begin(gui_context, "spline editor", spline_editor_rect, widgets_parameters))
	{
	    nk_layout_row_dynamic(gui_context, 30, 2);
	    if(nk_button_label(gui_context, "add")){
		add_default_spline();
	    }

	    if(nk_button_label(gui_context, "delete")){
		if(selected_spline >= 0){
		    remove_spline(selected_spline);
		}
	    }

	    //spline selection
	    nk_layout_row_dynamic(gui_context, 25, 1);
	    for(int i = 0; i < get_spline_count(); i++){
		if (nk_button_label(gui_context, "spline")) {
		    if(selected_spline != i){
			selected_spline = i;
		    }else{
			selected_spline = -1;
		    }
		}

	    }

	}
    nk_end(gui_context);
    #endif

    
    //KEYFRAME EDITOR
    #if 1
    struct nk_rect keyframe_editor_rect = nk_rect(win_x - (win_x / 5), win_y / 24,
						  win_x / 5, win_y);

    if(nk_begin(gui_context, "keyframe editor", keyframe_editor_rect, widgets_parameters)){

	if(on_keyframe()){
	    char text_buffer[64];

	    nk_layout_row_dynamic(gui_context, 25, 1);

	    //frame tick
	    snprintf(text_buffer, 64, "tick: %d", timeline_tick);
	    nk_label(gui_context, text_buffer, NK_TEXT_LEFT);
	    
	    //frame action
	    keyframe_t *current_keyframe = NULL;
	    for(int i = 0; i < k_count; i++){
		/* if(timeline_tick * 60 == all_keyframes[i].tick){ */
		/* 		current_keyframe = all_keyframes[i]; */
		/* 		break; */
		/* 	} */

		//we now work on vsync
		if(timeline_tick == all_keyframes[i].tick){
		    current_keyframe = &all_keyframes[i];
		    break;
		}
	    }
	    
	    if(current_keyframe != NULL){
		struct nk_vec2 size = {100, 100};
		int type_selection = nk_combo(gui_context, keyframe_type_names, 3, current_keyframe->action, 12, size);
		if(type_selection != current_keyframe->action){
		    current_keyframe->action = type_selection;
		    printf("change\n");
		}

		//frame parameters
		//we show parameters only if their supposed to be
		//usable for the current keyframe type
		nk_label(gui_context, "paramaters ", NK_TEXT_CENTERED);
		static int dnc_edit_text_len = 0;
		static char dnc_edit_text[32] = "";

		static int anc_edit_text_len = 0;
		static char anc_edit_text[32] = "";

		static int a_edit_text_len = 0;
		static char a_edit_text[32] = "";
		
		
		if(current_keyframe->action == KEYFRAME_ENEMY_DESTROY ||
		   current_keyframe->action == KEYFRAME_ENEMY_CHANGE_PATH){
		    
		    nk_layout_row_dynamic(gui_context, 24, 3);
		    nk_label(gui_context, "id: ", NK_TEXT_LEFT);
		    
		    nk_flags flags = nk_edit_string(gui_context, NK_EDIT_SIMPLE,
						    dnc_edit_text, &dnc_edit_text_len,
						    5, nk_filter_decimal);
		    
		    if(flags == 2){
			//inactivo
			snprintf(dnc_edit_text, 32, "%d", current_keyframe->params.id);
			dnc_edit_text_len = strlen(dnc_edit_text);
		    }
		    
		    if(nk_button_label(gui_context, "Ok")){
			memset(dnc_edit_text + dnc_edit_text_len, 0, 32 - dnc_edit_text_len);
			int numbero = atoi(dnc_edit_text);
			printf("numbero: %d\n", numbero);
			
			current_keyframe->params.id = numbero;
		    }
		}

		if(current_keyframe->action == KEYFRAME_ENEMY_ADD){
		    nk_layout_row_dynamic(gui_context, 24, 3);
		    nk_label(gui_context, "Enemy type:", NK_TEXT_LEFT);

		    nk_flags flag = nk_edit_string(gui_context, NK_EDIT_SIMPLE,
						   a_edit_text, &a_edit_text_len,
						   5, nk_filter_decimal);
		    if(flag == 2){
			snprintf(a_edit_text, 32, "%d", current_keyframe->params.enemy_type);
			a_edit_text_len = strlen(a_edit_text);
		    }

		    if(nk_button_label(gui_context, "Ok")){
			memset(a_edit_text + a_edit_text_len, 0, 32 - a_edit_text_len);
			int numbero = atoi(a_edit_text);

			current_keyframe->params.enemy_type = numbero;
		    }
		    
		}
		
		if(current_keyframe->action == KEYFRAME_ENEMY_ADD ||
		   current_keyframe->action == KEYFRAME_ENEMY_CHANGE_PATH){
		    nk_layout_row_dynamic(gui_context, 24, 3);
		    nk_label(gui_context, "Path id:", NK_TEXT_LEFT);

		    nk_flags flag = nk_edit_string(gui_context, NK_EDIT_SIMPLE,
						   anc_edit_text, &anc_edit_text_len,
						   5, nk_filter_decimal);
		    if(flag == 2){
			snprintf(anc_edit_text, 32, "%d", current_keyframe->params.pathid);
			anc_edit_text_len = strlen(anc_edit_text);
		    }

		    if(nk_button_label(gui_context, "Ok")){
			memset(anc_edit_text + anc_edit_text_len, 0, 32 - anc_edit_text_len);
			int numbero = atoi(anc_edit_text);

			current_keyframe->params.pathid = numbero;
		    }
		}
	    }
	}

    }
    nk_end(gui_context);
    #endif

    //TIMELINE
    #if 1
    struct nk_rect timeline_editor_rect = nk_rect(win_x / 5, win_y - (win_y / 4),
						  (win_x / 5) * 3, win_y / 4);
    if(nk_begin(gui_context, "timeline", timeline_editor_rect, widgets_parameters)){
	//draw timeline
	draw_timeline(gui_context);
    }
    nk_end(gui_context);
    #endif 
}

void editor_draw(SDL_Renderer* renderer){
    //draw bg
    SDL_Rect bg = {-camera.x, -camera.y, 600, 600};
    SDL_RenderCopy(renderer, editor_scene->resources->textures[TXT_BG_PURPLE], NULL, &bg);

    //draw grid
#if DRAW_GRID
    int divider = 300;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    for(int i = 0; i < divider; i++){
	SDL_RenderDrawLine(renderer,
			   (i * divider) - camera.x, -camera.y,
			   i * divider - camera.x,     600 - camera.y);

	SDL_RenderDrawLine(renderer,
			   -camera.x, (i * divider) - camera.y,
			   600 - camera.x, (i * divider) - camera.y);
    }
#endif

    //
    //DRAW GAME ELEMENTS
    //

    //player
    SDL_FRect player_hitbox = editor_scene->player->hitbox;
    player_hitbox.x -= camera.x;
    player_hitbox.y -= camera.y;
    SDL_RenderCopyF(renderer, editor_scene->player->sprite, NULL, &player_hitbox);

    //splines
    spline_t* splines = get_splines();
    for(int i = 0; i < get_spline_count(); i++){
	spline_draw_camera(splines[i], camera, renderer);
    }

    if(selected_spline >= 0){
	spline_debug_draw(splines[selected_spline], renderer, camera);
    }

    ////splines points
    if(selected_spline >= 0){
	spline_t s = splines[selected_spline];

	const int size = 25;
	SDL_Rect r = {0, 0, size, size};
	for(int i = 0; i < s.total_points; i++){
	    r = (SDL_Rect){(s.points[i].x * WINDOW_WIDTH) - camera.x - size / 2,
			   (s.points[i].y * WINDOW_HEIGHT) - camera.y - size / 2,
			   size, size};
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	    SDL_RenderDrawRect(renderer, &r);
	}
    }

    //this should be placed on it's own function
    //editor_event_start() or something like that...;
    lclick_pressed = false;
    rclick_pressed = false;
    lclick_released = false;
    rclick_released = false;    
    mwheel_this_frame = false;
}

void editor_exit(){
    scene_destroy(editor_scene);
}

void editor_events(SDL_Event event){
    //nk_sdl_handle_event(&event);
    if(event.type == SDL_WINDOWEVENT){
	if(event.window.event == SDL_WINDOWEVENT_RESIZED){
	    window_size.x = (int)event.window.data1;
	    window_size.y = (int)event.window.data2;
	}
    }

    if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_n){
	add_default_spline();
    }

    //we can have multiple calls of SDL_MOUSEWHEEL event perframe
    //so we just check once per frame
    if(event.type == SDL_MOUSEWHEEL){
	mwheel_this_frame = true;
	mwheel_dir = event.wheel.y;
    }else{
	mwheel_this_frame = false;
    }

    if(event.type == SDL_MOUSEBUTTONDOWN &&
       event.button.button == SDL_BUTTON_LEFT)
	{
	    lclick_pressed = true;
	}
    
    if(event.type == SDL_MOUSEBUTTONDOWN &&
       event.button.button == SDL_BUTTON_RIGHT)
	{
	    rclick_pressed = true;
	}

    if(event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_LEFT)
	{
	    lclick_released = true;
	}
    
    if(event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_RIGHT)
	{
	    rclick_released = true;
	}
    
    
}

static bool is_hovering(SDL_Point mouse, SDL_Rect rect){

    if((mouse.x > rect.x && (mouse.x < (rect.x + rect.w))) &&
       (mouse.y > rect.y && (mouse.y < (rect.y + rect.y)))){
	return true;
    }

    return false;
}

//indirection
static int get_current_keyframe(){
    return editor_scene->current_keyframe;
}

static int get_keyframe_count(){
    return editor_scene->keyframe_count;
}

static keyframe_t* get_keyframes(){
    return editor_scene->keyframes;
}

static spline_t* get_splines(){
    return editor_scene->splines;
}

static int get_spline_count(){
    return editor_scene->spline_count;
}

static void add_keyframe(keyframe_t k){
    keyframe_t *keyframes = get_keyframes();
    int count = get_keyframe_count();

    keyframes[count] = k;
    editor_scene->keyframe_count += 1;
}

static void add_default_spline(){
    spline_t s = {4,
		  {{0, 0.5f}, {0.25f, 0.5f}, {0.5f, 0.5f}, {1.0f, 0.5f}},
		  false};
    add_spline(s);
}

static void add_spline(spline_t s){
    if(get_spline_count() < MAX_SPLINES){
	editor_scene->splines[editor_scene->spline_count] = s;
	editor_scene->spline_count += 1;
    }
}

static void remove_spline(int remove_id){
    spline_t* splines = get_splines();

    for(int i = remove_id; i < get_spline_count(); i++){
	splines[i] = splines[i + 1];
    }

    editor_scene->spline_count -= 1;
    selected_spline = -1;
}

static void spline_editor(int mouse_click, SDL_Keymod modState){
    spline_t* spline = &(get_splines()[selected_spline]);
    int point_count = spline->total_points;
    SDL_FPoint* points = spline->points;
    
    int hovering_point = -1;
    int size = 25;
    
    //find the point being hovered
    for(int i = 0; i < point_count; i++){
	SDL_Rect r = {0, 0, size, size};
	r.x = (points[i].x * WINDOW_WIDTH) - camera.x - size / 2;
	r.y = (points[i].y * WINDOW_WIDTH) - camera.y - size / 2;	
	
	if(is_hovering(mouse, r)){
	    hovering_point = i;
	}
    }

    //given a point being hovered
    //we want to save it if we click it
    //or forget it if we release
    if(hovering_point >= 0 && lclick_pressed){
	selected_spline_point = hovering_point;	
    }else if(hovering_point >= 0 && lclick_released){
	selected_spline_point = -1;	
    }

    //we update the hovered point's position if there's such selected
    if(selected_spline_point >= 0){
	spline->points[selected_spline_point].x = ((float)(mouse.x) + (float)camera.x) / (float)WINDOW_WIDTH;
	spline->points[selected_spline_point].y = ((float)(mouse.y) + (float)camera.y) / (float)WINDOW_HEIGHT;
    }
    
}

static void draw_timeline(struct nk_context *gui_context){
    int k_count = get_keyframe_count();
    const keyframe_t *all_keyframes = get_keyframes();

    char buff[4];
    snprintf(buff, 4, "%d", timeline_tick);

    nk_layout_row_static(gui_context, 30, 120, 3);
    if(nk_button_label(gui_context, "previous" )){
	for(int i = 0; i < k_count-1; i++){

	    /* int first_tick = all_keyframes[i].tick / 60; */
	    /* int second_tick = all_keyframes[i+1].tick / 60; */
	    /* int max_tick = all_keyframes[k_count-1].tick / 60; */

	    //we now work on vsync
	    int first_tick = all_keyframes[i].tick;
	    int second_tick = all_keyframes[i+1].tick;
	    int max_tick = all_keyframes[k_count-1].tick;

	    //we're further to the right than the last tick
	    if(timeline_tick > max_tick){
		timeline_tick = max_tick;
		break;
	    }

	    //we're between the range of the first and last tick
	    if(timeline_tick > first_tick &&
	       timeline_tick <= second_tick){
		timeline_tick = first_tick;
		break;
	    }
	}
    }
    
    if(nk_button_label(gui_context, "next")){
	for(int i = 0; i < k_count-1; i++){

	    /* int first_tick = all_keyframes[i].tick / 60; */
	    /* int second_tick = all_keyframes[i+1].tick / 60; */
	    /* int min_tick = all_keyframes[0].tick / 60; */

	    //we work with vsync now
	    int first_tick = all_keyframes[i].tick;
	    int second_tick = all_keyframes[i+1].tick;
	    int min_tick = all_keyframes[0].tick;

	    //we're further to the left than the first tick
	    if(timeline_tick < min_tick){
		timeline_tick = min_tick;
		break;
	    }

	    //we're between the range of the first and last tick
	    if(timeline_tick >= first_tick &&
	       timeline_tick < second_tick){
		timeline_tick = second_tick;
		break;
	    }

	}
    }

    if(nk_button_label(gui_context, "add")){
	if(!on_keyframe()){
	    printf("adding a new keyframe\n");
	    keyframe_params kp = {.id = -1, .enemy_type = -1, .pathid = -1, };
	    keyframe_t k = {.tick = timeline_tick, .action = KEYFRAME_ENEMY_ADD, kp};

	    if(k_count < MAX_KEYFRAMES){
		editor_scene->keyframes[k_count] = k;
		editor_scene->keyframe_count += 1;
		k_count = get_keyframe_count();

		#if 0
		printf("ticks order: \n");
		for(int i = 0; i < k_count; i++){
		    printf("%d;", all_keyframes[i].tick);
		}
		printf("\n");
		#endif
		
		//we gotta sort'em out
		int i = 0;
		bool swaped = false;
		while(true){
		    keyframe_t *current = &editor_scene->keyframes[i];
		    
		    //printf("%d", current->tick);
		    //printf("[");
		    for(int j = i ; j < k_count-1; j++){
			keyframe_t *next = &editor_scene->keyframes[j+1];
			//printf("%d;", next->tick);

			if(current->tick > next->tick){
			    //we gotta swap them
			    //printf("\npre swap:\n");
			    //printf("current: %d - next: %d\n", current->tick,
			    //next->tick);
			    
			    swaped = true;
			    keyframe_t tmp;
			    tmp = *current;
			    *current = *next;
			    *next = tmp;
			    //printf("\npost swap:\n");
			    //printf("current: %d - next: %d\n", current->tick,
			    //next->tick);			    
			}
		    }
		    //printf("]\n");
		    
		    if(swaped == true){
			swaped = false;
			//printf("swaped\n");
		    }else{
			swaped = false;
			i++;
			//printf("not swaped\n");
		    }
		    if(i == k_count || i > MAX_KEYFRAMES * 2){
			break;
		    }
		}

		#if 0
		printf("ticks: \n");
		for(int i = 0; i < k_count; i++){
		    printf("%d;", all_keyframes[i].tick);
		}
		printf("\n");
		#endif
	    }
	    
	    
	}else{
	    printf("cant add more keyframes on same tick\n");
	}
	
    }

    if(nk_button_label(gui_context, "remove")){
	if(!on_keyframe()){
	    printf("cleaning keyframe\n");
	}else{
	    printf("no keyframe selected\n");
	}
	
    }    

    nk_layout_row_begin(gui_context, NK_DYNAMIC, 25, 2);

    nk_layout_row_push(gui_context, 0.1f);
    nk_label(gui_context, buff, NK_TEXT_LEFT);

    nk_layout_row_push(gui_context, 0.9f);
    nk_slider_int(gui_context, 0, &timeline_tick, 1000, 1);

    nk_layout_row_end(gui_context);

}

static bool on_keyframe(){
    const keyframe_t *all_keyframes = get_keyframes();
    const int k_count = get_keyframe_count();

    for(int i = 0; i < k_count; i++){
	//vsync n stuff
	/* 	if(timeline_tick == all_keyframes[i].tick/60){ */
	/* 	return true; */
	/* } */
	if(timeline_tick == all_keyframes[i].tick){
	    return true;
	}

    }

    return false;
}

bool editor_save_to_file(){
    FILE* level_file = fopen("level.lvl", "wb");
    
    if(level_file == NULL){
	printf("failed saving file\n");
    }

    fputs("PATHS\n", level_file);
    for(int i = 0; i < editor_scene->spline_count; i++){
	spline_t current_spline = editor_scene->splines[i];

	//P mark for starting point	
	fputs("P", level_file);
	
	//Write total points of spline
	fwrite(&current_spline.total_points, sizeof(size_t), 1, level_file);
	
	//Write all points
	fwrite(current_spline.points, sizeof(SDL_FPoint), current_spline.total_points, level_file );
	
	//Write if loop
	fwrite(&current_spline.loop, sizeof(bool), 1, level_file);
    }
    
    fputs("\nKEYFRAMES\n", level_file);
    for(int i = 0; i < editor_scene->keyframe_count; i++){
	keyframe_t current_keyframe = editor_scene->keyframes[i];
	
	//K mark for starting keyframe
	fputs("K", level_file);

	//Keyframe
	fwrite(&current_keyframe, sizeof(keyframe_t), 1, level_file);
    }
    
    fclose(level_file);
    
    return true;    
}
