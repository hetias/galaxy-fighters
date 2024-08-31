#include"editor.h"

#define DRAW_GRID 0
#define DRAW_KEYFRAME_EDITOR 1
#define DRAW_SPLINE_EDITOR 1
#define DRAW_TIMELINE 1

static SDL_Point window_size = {600, 600};

static scene_t* editor_scene;

static SDL_Point camera = {0, 0};
static int camera_speed = 2;

static SDL_Point mouse = {0, 0};

static int mwheel_dir = 0;
static bool mwheel_this_frame = false;

static bool lclick_pressed = false;
static bool rclick_pressed = false;
static int selected_spline = -1;

static TTF_Font* ui_font = NULL;

static int timeline_tick = 0;

bool editor_start(resources_t* game_resources, SDL_Window* window){
    printf("editor starting...\n");

    if(!game_resources){
	return false;
    }
    
    SDL_SetWindowResizable(window, SDL_TRUE);
    editor_scene = scene_create(game_resources, NULL);
    ui_font = game_resources->font;

    add_keyframe((keyframe_t){0, KEYFRAME_ENEMY_ADD, (keyframe_params){0, 0, 0}});
    add_keyframe((keyframe_t){60, KEYFRAME_ENEMY_ADD, (keyframe_params){0, 0, 0}});
    add_keyframe((keyframe_t){120, KEYFRAME_ENEMY_ADD, (keyframe_params){0, 0, 0}});
    
    printf("spline count: %d\n", editor_scene->spline_count);
    printf("keyframe count: %d\n", editor_scene->keyframe_count);
}

void editor_running(){
    //get user input
	
    //mouse
    //
    // Documentation says that SDL_BUTTON_RIGHT = 3,
    // but for some reason it retuns 4.
    // passing it to SDL_BUTTON() returns 8 but not 4.
    //    
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
	    r = (SDL_Rect){(s.points[i].x * WINDOW_WIDTH) - camera.x,
			   (s.points[i].y * WINDOW_HEIGHT) - camera.y,
			   size, size};
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	    SDL_RenderDrawRect(renderer, &r);
	}
    }
    
    
    //
    // DRAW GUI ELEMENTS 
    //
    const SDL_Color ui_color = {104, 48, 161};
    const SDL_Color ui_color_active = {143, 77, 209};
    int widget_size = 300;

    //spline editor
#if DRAW_SPLINE_EDITOR
    SDL_Rect spline_editor_bg = {0, 0,
	    widget_size, window_size.y};

    if(is_hovering(mouse, spline_editor_bg)){
	SDL_SetRenderDrawColor(renderer, ui_color.r, ui_color.g,
			       ui_color.b, 0);
	SDL_RenderFillRect(renderer, &spline_editor_bg);
    }else{
        SDL_SetRenderDrawColor(renderer,
			       ui_color_active.r, ui_color_active.g,
			       ui_color_active.b, 0);
	SDL_RenderFillRect(renderer, &spline_editor_bg);	
    }

    ////add spline button
    SDL_Rect add_button = spline_editor_bg;
    add_button.w -= (spline_editor_bg.w / 2);
    add_button.h = 25;
    if(ui_button(add_button, "add", renderer)){
	spline_t s = {4,
		      {{0, 0.5f}, {0.25f, 0.5f}, {0.5f, 0.5f}, {0.75f, 0.5f}},
		      false};
	add_spline(s);
    }

    ////delete spline button
    SDL_Rect delete_button = add_button;
    delete_button.x += delete_button.w;
    if(ui_button(delete_button, "delete", renderer)){
	remove_spline(selected_spline);
    }
    
    //// spline selectors
    SDL_Rect selector_dimensions = {spline_editor_bg.x,
				    spline_editor_bg.y + add_button.h,
				    spline_editor_bg.w,
				    spline_editor_bg.h};
    selector_dimensions.h = 25;    
    char str_buff[16] = {0};
    for(int i = 0; i <get_spline_count(); i++){
	sprintf(str_buff, "%d", i);
	if(ui_button(selector_dimensions, str_buff, renderer)){
	    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	    if(selected_spline == i){
		selected_spline = -1;
	    }else{
		selected_spline = i;
	    }
	}
	selector_dimensions.y += selector_dimensions.h + 25;
    }
    
#endif

    
    //keyframe editor
#if DRAW_KEYFRAME_EDITOR
    SDL_Rect keyframe_editor_bg = {window_size.x - widget_size,0,
				   widget_size, window_size.y};
    if(is_hovering(mouse, keyframe_editor_bg)){
	SDL_SetRenderDrawColor(renderer, ui_color.r, ui_color.g,
			       ui_color.b, 0);	
    }else{
        SDL_SetRenderDrawColor(renderer,
			       ui_color_active.r, ui_color_active.g,
			       ui_color_active.b, 0);	
    }
    SDL_RenderFillRect(renderer, &keyframe_editor_bg);
#endif

    //timeline
#if DRAW_TIMELINE
    SDL_Rect timeline_bg = {widget_size, window_size.y - (window_size.y / 4),
			    window_size.x - widget_size * 2, window_size.y / 4};

    SDL_Rect time_line = {timeline_bg.x + 15, timeline_bg.y + 15,
			  timeline_bg.w - 30, timeline_bg.h - 30};
    
    SDL_Rect more_button = {(timeline_bg.x + timeline_bg.w) - 50, timeline_bg.y - 50, 50, 50};
    SDL_Rect less_button = {(timeline_bg.x + timeline_bg.w) - 50 * 2, timeline_bg.y - 50, 50, 50};

    //bg
    if(is_hovering(mouse, timeline_bg)){
	SDL_SetRenderDrawColor(renderer, ui_color.r, ui_color.g,
			       ui_color.b, 0);	
    }else{
        SDL_SetRenderDrawColor(renderer,
			       ui_color_active.r, ui_color_active.g,
			       ui_color_active.b, 0);
    }
    SDL_RenderFillRect(renderer, &timeline_bg);

    //more button
    if(ui_button(more_button, "+", renderer)){
	
    }

    //less button
    if(ui_button(less_button, "-", renderer)){
	
    }
    
    //timeline
    if(is_hovering(mouse, time_line)){
	SDL_SetRenderDrawColor(renderer, 125, 125, 125, 125);
	
	if(mwheel_this_frame){
	    timeline_tick += mwheel_dir;
	}
	
    }else{
	SDL_SetRenderDrawColor(renderer, 130, 130, 130, 130);
    }
    SDL_RenderFillRect(renderer, &time_line);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 150);

    int keyframe_count = get_keyframe_count();
    const keyframe_t *keyframes = get_keyframes();
    SDL_Rect keyframe_bar = {time_line.x, time_line.y,
			     time_line.w / 16, time_line.h};
    
    for(int i = timeline_tick; i < 16; i++){
	keyframe_bar.x = time_line.x + (keyframes[i].tick / 60) * keyframe_bar.w
	    - (timeline_tick * keyframe_bar.w);
	
	if(keyframe_bar.x >= time_line.x &&
	   keyframe_bar.x < time_line.x + time_line.w){
	    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	    SDL_RenderFillRect(renderer, &keyframe_bar);	    
	}
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderDrawLine(renderer,
		       time_line.x, time_line.y,
		       time_line.x, time_line.y + time_line.h);
#endif

    //this should be placed on it's own function
    //editor_event_start();
    lclick_pressed = false;
    rclick_pressed = false;
    mwheel_this_frame = false;
}

void editor_exit(){
    scene_destroy(editor_scene);
}

void editor_events(SDL_Event event){
    if(event.type == SDL_WINDOWEVENT){
	if(event.window.event == SDL_WINDOWEVENT_RESIZED){
	    window_size.x = event.window.data1;
	    window_size.y = event.window.data2;
	}
    }

    if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_n){
	spline_t s = {4,
		      {{0, 0.5f}, {0.25f, 0.5f}, {0.5f, 0.5f}, {1.0f, 0.5f}},
		      false};
	add_spline(s);
    }

    //we can have multiple calls of SDL_MOUSEWHEEL event perframe
    //so we just check once per frame
    if(event.type == SDL_MOUSEWHEEL){
	mwheel_this_frame = true;
	mwheel_dir = event.wheel.y;
    }else{
	mwheel_this_frame = false;
    }
    
    if(event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_LEFT
       ){
	lclick_pressed = true;
    }

    if(event.type == SDL_MOUSEBUTTONUP &&
       event.button.button == SDL_BUTTON_RIGHT
       ){
	rclick_pressed = true;
    }
    
}

static bool is_hovering(SDL_Point mouse, SDL_Rect rect){
    return (mouse.x > rect.x &&
	    mouse.x < rect.x + rect.w &&
	    mouse.y > rect.y &&
	    mouse.y < rect.y + rect.h);
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

    ////modify points
    const int size = 25;
    SDL_Rect r = {0, 0, size, size};
    int selected_point = -1;
    for(int i = 0; i < point_count; i++){
	r = (SDL_Rect){((points[i].x * WINDOW_WIDTH) - camera.x),
		       ((points[i].y * WINDOW_HEIGHT) - camera.y),
		       size, size};
	if(is_hovering(mouse, r) && selected_point < 0){
	    //if we want to hold, SDL_BUTTON()
	    if(SDL_BUTTON(mouse_click) == 8 && modState == KMOD_LCTRL){
		if(point_count > 4){
		    spline_remove_point(spline, i);		
		}
	    }else if(SDL_BUTTON(mouse_click) == SDL_BUTTON_LEFT){
		points[i].x = ((float)(mouse.x - size / 2) + (float)camera.x) / (float)WINDOW_WIDTH;
		points[i].y = ((float)(mouse.y - size / 2) + (float)camera.y) / (float)WINDOW_HEIGHT;
		selected_point = i;
	    }
	}
    }

    //if we only want mouse press, rclick_pressed/lclicl_pressed...
    if(rclick_pressed && modState == KMOD_NONE){
	SDL_FPoint p;
	p.x = ((float)(mouse.x - size / 2) + (float)camera.x) /
	    (float)WINDOW_WIDTH;
	p.y = ((float)(mouse.y - size / 2) + (float)camera.y) /
	    (float)WINDOW_HEIGHT;

	if(point_count + 1 < MAX_POINTS){
	    spline_add_point(&(get_splines()[selected_spline]), p);
	}else{
	    printf("no more points can be added\n");
	}
    }
}

//ui elements
static bool ui_button(SDL_Rect rect, const char* text, SDL_Renderer* renderer){
    bool clicked = false;
    bool hover = false;
    
    if(is_hovering(mouse, rect)){
	hover = true;
	if(lclick_pressed){
	    clicked = true;
	}
    }

    if(clicked){
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }else{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	
    }

    if(hover){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);	
    }else{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	
    }

    SDL_RenderFillRect(renderer, &rect);

    if(text){
	SDL_Surface* sf = NULL;
	SDL_Texture* tx = NULL;
	SDL_Rect r = {0};
	sf = TTF_RenderText_Solid(ui_font, text, (SDL_Color){0, 0, 0,});
	tx = SDL_CreateTextureFromSurface(renderer, sf);
    
	r = (SDL_Rect){rect.x + (rect.w / 2), rect.y + (rect.h / 2),
		       sf->w, sf->h};

	SDL_RenderCopy(renderer, tx, 0, &r);

	SDL_FreeSurface(sf);
	SDL_DestroyTexture(tx);    
    }
    return clicked;
}
