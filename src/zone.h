#ifndef ZONE_H
#define ZONE_H
#include <stdlib.h>
#include <stdbool.h>
#include "color.h"
#include "diffdraw.h"
#define ZONE_WIDTH 80
#define ZONE_HEIGHT 24
#define ZONE_AREA (ZONE_WIDTH*ZONE_HEIGHT)
struct task {
	enum {ATTACK,DEFEND,CAPTURE} type;
	struct entity *target;
	int duration;
	struct task *next_task;
};
struct effect {
	int duration;
	void (*start)(void *);
	void (*turn)(void *);
	void (*end)(void *);
};
struct creature {
	char *name;
	struct task *next_task;
	char sym;
	color_t color;
	//struct faction *faction;
	// TODO: Stats
	// TODO: Effects
};
struct item {
	char *name;
	unsigned int value;
	char sym;
	color_t color;
	void (*use)(void);
	bool unique;
	// TODO: Stats
};
struct entity {
	enum {CREATURE,ITEM} type;
	union {
		struct creature *c;
		struct item *i;
	} ptr;
};
struct tile {
	char fg,bg;
	color_t fg_c,bg_c;
	// TODO: Entities
	struct entity *e; // Temporary
	// TODO: Effects
};

void scroll_zone(int);
void draw_entity(struct entity *);
void draw_tile(struct tile *);
void draw_zone_pos(struct tile *,int);
void draw_zone(struct tile *);
int rand_tile(struct tile *,bool (*)(struct tile *));
bool empty_tile(struct tile *);
int spawn(struct tile *,struct entity *);
struct tile *new_zone(void);
#endif
