
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_DEPTH    32

/** Initialize SDL */
void init_sdl(void);

/** Initialize opengl */
void init_gl(void);

/** Teardown the application */
void teardown(void);

/** Update the application */
void update(Uint32 ticks);

/** Renders the application */
void render(void);

int finished = 0;
SDL_Event event;

Uint32 last = 0, current = 0;

/** Program entry point */
int main(int argc, char *argv[]) {

   /* setup */
   init_sdl();
   init_gl();

   last = SDL_GetTicks();

   /* run the app loop */
   while (!finished) {

      /* process any events */
      while (SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_QUIT:
               finished = 1;
               break;
         }
      }

      /* timing and rendering */
      current = SDL_GetTicks();
      update(current - last);
      last = current;

      render();

      /* swap the buffers over */
      SDL_GL_SwapBuffers();
   }

   /* finished */
   teardown();

   return 0;
}

/** Initialize SDL */
void init_sdl(void) {
   /* initialize sdl */
   if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
      fprintf(stderr, "Unable to start sdl\n");
      exit(1);
   }

   /* initialize video */
   if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH,
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL) == NULL) {
      fprintf(stderr, "Unable to set video mode\n");
      exit(2);
   }
}

/** Initialize opengl */
void init_gl(void) {
   /* setup an orthographic matrix using the viewport
    * dimensions on the projection matrix */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0, -1, 1);

   /* clear out the modelview matrix */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   /* disable depth testing (as we're not using z) */
   glDisable(GL_DEPTH_TEST);

   /* black clear colour */
   glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}


/** Teardown the application */
void teardown(void) {
   SDL_Quit();
}

/** Update the application */
void update(Uint32 ticks) {

}

/** Renders the application */
void render(void) {
   glClear(GL_COLOR_BUFFER_BIT);
}
