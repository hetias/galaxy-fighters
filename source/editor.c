#include"editor.h"

#define DRAW_GRID 0
#define DRAW_KEYFRAME_EDITOR 0
#define DRAW_SPLINE_EDITOR 1
#define DRAW_TIMELINE 0

static scene_t* editor_scene;
static int camera_speed = 2;
static SDL_Point window_size = {600, 600};
static SDL_Point camera = {0, 0};
static SDL_Point mouse = {0, 0};
static bool mouse_pressed = false;
static int selected_spline = -1;

static TTF_Font* ui_font = NULL;

bool editor_start(resources_t* game_resources, SDL_Window* window){
    printf("editor starting...\n");

    if(!game_resources){
	return false;
    }
    
    SDL_SetWindowResizable(window, SDL_TRUE);
    editor_scene = scene_create(game_resources, NULL);
    ui_font = game_resources->font;
    
    printf("spline count: %d\n", editor_scene->spline_count);
}

void editor_running(){
    //get user input

    //mouse
    int mouse_click = SDL_GetMouseState(&mouse.x, &mouse.y);

    //keyboard
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

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

    if(selected_spline >= 0){	
	int point_count = get_splines()[selected_spline].total_points;
	SDL_FPoint* points = (get_splines()[selected_spline].points);       
	
	const int size = 25;
	SDL_Rect r = {0, 0, size, size};
	int selected_point = -1;
	for(int i = 0; i < point_count; i++){
	    r = (SDL_Rect){((points[i].x * WINDOW_WIDTH) - camera.x),
			   ((points[i].y * WINDOW_HEIGHT) - camera.y),
			   size, size};
	    
	    if(is_hovering(mouse, r) && selected_point < 0){
		if(SDL_BUTTON(mouse_click) == 1){
		    points[i].x = ((float)(mouse.x - size / 2) + (float)camera.x) / (float)WINDOW_WIDTH;
		    points[i].y = ((float)(mouse.y - size / 2) + (float)camera.y) / (float)WINDOW_HEIGHT;
		    selected_point = i;
		}else{
		    selected_point = -1;
		}
	    }
	}
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
    SDL_FRect player_hitbox = editor_scene->player->hitbox;
    player_hitbox.x -= camera.x;
    player_hitbox.y -= camera.y;
    SDL_RenderCopyF(renderer, editor_scene->player->sprite, NULL, &player_hitbox);

    spline_t* splines = get_splines();
    for(int i = 0; i < get_spline_count(); i++){
	spline_draw_camera(splines[i], camera, renderer);
    }

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

    if(mouse.x > spline_editor_bg.x &&
       mouse.x < spline_editor_bg.x + spline_editor_bg.w &&
       mouse.y > spline_editor_bg.y &&
       mouse.y < spline_editor_bg.y + spline_editor_bg.h){
	SDL_SetRenderDrawColor(renderer, ui_color.r, ui_color.g,
		       ui_color.b, 0);	
    }else{
        SDL_SetRenderDrawColor(renderer,
			       ui_color_active.r, ui_color_active.g,
			       ui_color_active.b, 0);
    }
    SDL_RenderFillRect(renderer, &spline_editor_bg);

    ///add spline button
    float gap = 5.0f;
    SDL_Rect button = spline_editor_bg;
    button.x += gap * 2;
    button.y += gap * 2;    
    button.w = (button.w / 2) - gap * 2;
    button.h = 25 - gap;
    
    if(ui_button(button, "add", renderer)){
	spline_t s = {4,
		      {{0, 0.5f}, {0.25f, 0.5f}, {0.5f, 0.5f}, {0.75f, 0.5f}},
		      false};
	add_spline(s);
    }

    //delete spline button
    button.x += button.w + gap * 2;
    button.w -= gap * 2;    
    if(ui_button(button, "delete", renderer)){
	remove_spline(selected_spline);
    }
    
    //// spline selectors
    SDL_Rect selector_dimensions = {spline_editor_bg.x,
				    spline_editor_bg.y + button.h + gap * 2,
				    spline_editor_bg.w,
				    spline_editor_bg.h};
    selector_dimensions.x += gap * 2;
    selector_dimensions.y += gap;    
    selector_dimensions.w -= gap * 4;
    selector_dimensions.h = 25;    
    for(int i = 0; i < get_spline_count(); i++ ){
	if(is_hovering(mouse, selector_dimensions)){
	    if(mouse_pressed){
		printf("selected spline: %d\n", i);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		if(selected_spline == i){
		    selected_spline = -1;
		}else{
		    selected_spline = i;
		}
	    }else{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    }
	}else{
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
	SDL_RenderFillRect(renderer, &selector_dimensions);
	selector_dimensions.y += selector_dimensions.h + gap * 2;
    }

    #endif
    
    //keyframe editor
    #if DRAW_KEYFRAME_EDITOR
    SDL_Rect keyframe_editor_bg = {window_size.x - widget_size,0,
				   widget_size, window_size.y};
    if(mouse.x > keyframe_editor_bg.x &&
       mouse.x < keyframe_editor_bg.x + keyframe_editor_bg.w &&
       mouse.y > keyframe_editor_bg.y &&
       mouse.y < keyframe_editor_bg.y + keyframe_editor_bg.h){
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
    SDL_Rect timeline_bg = {0, window_size.y - (window_size.y / 4),
			    window_size.x, (window_size.y / 4)};    
    if(mouse.x > timeline_bg.x &&
       mouse.x < timeline_bg.x + timeline_bg.w &&
       mouse.y > timeline_bg.y &&
       mouse.y < timeline_bg.y + timeline_bg.h){
	SDL_SetRenderDrawColor(renderer, ui_color.r, ui_color.g,
			       ui_color.b, 0);
    }else{
        SDL_SetRenderDrawColor(renderer,
			       ui_color_active.r, ui_color_active.g,
			       ui_color_active.b, 0);
    }
    SDL_RenderFillRect(renderer, &timeline_bg);
    #endif

    mouse_pressed = false;
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

    if(event.type == SDL_MOUSEBUTTONUP && event.button.button == 1
       ){
	mouse_pressed = true;
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

static const keyframe_t* get_keyframes(){
    return editor_scene->keyframes;    
}

static spline_t* get_splines(){
    return editor_scene->splines;    
}

static int get_spline_count(){
    return editor_scene->spline_count;        
}

static void add_keyframe(keyframe_t k){
    
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

//ui elements
static bool ui_button(SDL_Rect rect, const char* text, SDL_Renderer* renderer){
    bool clicked = false;
    bool hover = false;
    SDL_Surface* sf = NULL;
    SDL_Texture* tx = NULL;
    SDL_Rect r = {0};
    
    if(is_hovering(mouse, rect)){
	hover = true;
	if(mouse_pressed){
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
        
    sf = TTF_RenderText_Solid(ui_font, text, (SDL_Color){0, 0, 0,});
    tx = SDL_CreateTextureFromSurface(renderer, sf);
    
    r = (SDL_Rect){rect.x + (rect.w / 2), (rect.h / 2),
		  sf->w, sf->h};

    SDL_RenderCopy(renderer, tx, 0, &r);

    SDL_FreeSurface(sf);
    SDL_DestroyTexture(tx);    
    return clicked;
}
