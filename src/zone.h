#ifndef ZONE_H
#define ZONE_H
#include <stdlib.h>
#include <stdbool.h>
#include "color.h"
#include "diffdraw.h"
#ifndef ZONE_WIDTH
#define ZONE_WIDTH 80
#endif
#ifndef ZONE_HEIGHT
#define ZONE_HEIGHT 24
#endif
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
	struct entity *next_entity;
};
struct tile {
	char fg,bg;
	color_t fg_c,bg_c;
	// TODO: Entities
	struct entity *next_entity;
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
bool in_bounds(int,int);
int move_entity(struct tile *,int,int);
#endif
