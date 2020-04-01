#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NUM_COLS 100
#define NUM_ROWS 50

bool grid[NUM_COLS][NUM_ROWS];
bool temp[NUM_COLS][NUM_ROWS];

void swapGrids() {
	for (int i = 0; i < NUM_COLS; i++) {
		for (int j = 0; j < NUM_ROWS; j++) {
			grid[i][j] = temp[i][j];
		}
	}
}

void initGrid() {
	/*/
	temp[1][3] = true;
	temp[2][3] = true;
	temp[3][3] = true;
	temp[3][2] = true;
	temp[2][1] = true;

	/*/
	srandom(time(NULL));
	for (int i = 0; i < NUM_COLS; i++) {
		for (int j = 0; j < NUM_ROWS; j++) {
			if (random()%100 < 25) {
				temp[i][j] = true;
			}
			else {
				temp[i][j] = false;
			}
		}
	}
	//*/

	swapGrids();
}

void wrap(const int i, const int j, int &ni, int &nj) {
	ni = i;
	nj = j;
	if (ni < 0) {
		ni += NUM_COLS;
	}
	else if (ni >= NUM_COLS) {
		ni -= NUM_COLS;
	}
	if (nj < 0) {
		nj += NUM_ROWS;
	}
	else if (nj >= NUM_ROWS) {
		nj -= NUM_ROWS;
	}
}

int countNeighbors(int i, int j) {
	int count = 0;
	int x=0, y=0;

	//Moore neighboorhood
	for (int li = i-1; li <= i+1; li++) {
		for (int lj = j-1; lj <= j+1; lj++) {
			if (li == i && lj == j) {
				continue;
			}

			wrap(li, lj, x, y);
			if (grid[x][y] == true) {
				count++;
			}
		}
	}
	return count;
}

void update() {
	int numNeighbors;
	for (int i = 0; i < NUM_COLS; i++) {
		for (int j = 0; j < NUM_ROWS; j++) {
			numNeighbors = countNeighbors(i, j);

			//Starvation
			if (numNeighbors < 2) {
				temp[i][j] = false;
			}
			//Reproduction
			if (numNeighbors == 3) {
				temp[i][j] = true;
			}
			//Overcrowding
			else if (numNeighbors > 3) {
				temp[i][j] = false;
			}
		}
	}
	swapGrids();
}

//Maps cell to spacial coordinate
void torus (const int i, const int j, float &x, float &y, float &z) {
	const float a = 3.0;
	const float b = 2.0;

	float theta = 2*M_PI * i/NUM_COLS;
	float phi = 2*M_PI * j/NUM_ROWS;

	float cx, cy, rx, ry, rz;
	cx = cos(theta);
	cy = sin(theta);

	rx = cos(phi) * cx;
	ry = cos(phi) * cy;
	rz = sin(phi);

	x = a*cx + b*rx;
	y = a*cy + b*ry;
	z = b*rz;
}


void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float x=0, y=0, z=0;
	for (int i = 0; i < NUM_COLS; i++) {
		for (int j = 0; j < NUM_ROWS; j++) {
			//Gridlines
			glColor3f(0.25, 0.25, 0.25);
			glBegin(GL_LINES);
				torus(i, j, x, y, z);
				glVertex3f(x, y, z);
				torus(i, j+1, x, y, z);
				glVertex3f(x, y, z);

				torus(i, j, x, y, z);
				glVertex3f(x, y, z);
				torus(i+1, j, x, y, z);
				glVertex3f(x, y, z);
			glEnd();

			//Cells
			if (grid[i][j] == true) {
				glColor3f(0.0, 1.0, 0.0);
			}
			else {
				glColor3f(0.0, 0.0, 0.0);
			}

			glBegin(GL_QUADS);
				torus(i, j, x, y, z);
				glVertex3f(x, y, z);
				torus(i+1, j, x, y, z);
				glVertex3f(x, y, z);
				torus(i+1, j+1, x, y, z);
				glVertex3f(x, y, z);
				torus(i, j+1, x, y, z);
				glVertex3f(x, y, z);
			glEnd();
		}
	}
	glFlush();
}

void initGL() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glViewport(0.0, 0.0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate a maximally sized grid of squares
	// R/C <?> H/W
	/*
	if (NUM_ROWS * SCREEN_WIDTH > NUM_COLS * SCREEN_HEIGHT) {
		double TOTAL_COLS = NUM_ROWS * SCREEN_WIDTH / (double)SCREEN_HEIGHT;
		glOrtho( (NUM_COLS-TOTAL_COLS)/2.0, (NUM_COLS+TOTAL_COLS)/2.0, 0, NUM_ROWS, -1, 1);
	}
	else {
		double TOTAL_ROWS = NUM_COLS * SCREEN_HEIGHT / (double)SCREEN_WIDTH;
		glOrtho( 0, NUM_COLS, (NUM_ROWS-TOTAL_ROWS)/2.0, (NUM_ROWS+TOTAL_ROWS)/2.0, -1, 1);
	}
	*/

	GLfloat ratio = (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT;
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	SDL_Event event;
	const char * title = "SDL2 OpenGL";
	const int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	bool done = false;

	time_t time, utime, dirtime;
	int dirx=0, diry=1, dirz=0;
	const float speed = 0.5;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, flags);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	context = SDL_GL_CreateContext(window);

	initGL();
	initGrid();

	utime = dirtime = SDL_GetTicks();
	render();
	SDL_GL_SwapWindow(window);

	while (!done) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				done = true;
			}
		}

		time = SDL_GetTicks();
		if (time - utime > 100) {
			update();
			utime = time;
		}
		else if (time - dirtime > 5000) {
			dirx = random();
			diry = random();
			dirz = random();
			dirtime = time;
		}

		glRotatef(speed, dirx, diry, dirz);
		render();
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
