#define MAP_WIDTH 10
#define MAP_HEIGHT 10

struct terrain {
	float heightmap[MAP_WIDTH][MAP_HEIGHT];
};

void init(struct terrain *);
void generate(struct terrain *);
void get_height(int, int, struct terrain *, float*);