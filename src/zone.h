#ifndef ZONE_H
#define ZONE_H
#include <stdbool.h>
#include "color.h"
#define WIDTH 80
#define HEIGHT 24
#define AREA (WIDTH*HEIGHT)
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

#endif