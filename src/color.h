/*
 * I noticed that many graphics renditions in enum gr do not act as
 * their name (chosen by their listed function on Wikipedia) suggests.
 *
 * e.g. NO_BOLD doesn't seem to actually turn off BOLD,
 *      (However, one can use NORMAL instead).
 *
 * e.g. In some environments, FAINT just makes text gray.
 *
 * Also, BOLD typically significantly changes the color of text.
 *
 * This is a response to that; a way to provide easy and (mostly)
 * consistent color selections in a 1-byte datatype.
 */
#ifndef COLOR_H
#define COLOR_H
enum color {
	BLACK=FG_BLACK,
	DARK_GRAY=(1<<7)+FG_BLACK,
	RED=FG_RED,
	LIGHT_RED=(1<<7)+FG_RED,
	GREEN=FG_GREEN,
	LIGHT_GREEN=(1<<7)+FG_GREEN,
	BROWN=FG_YELLOW,
	YELLOW=(1<<7)+FG_YELLOW,
	BLUE=FG_BLUE,
	LIGHT_BLUE=(1<<7)+FG_BLUE,
	PURPLE=FG_MAGENTA,
	PINK=(1<<7)+FG_MAGENTA,
	TEAL=FG_CYAN,
	CYAN=(1<<7)+FG_CYAN,
	LIGHT_GRAY=FG_GRAY,
	WHITE=(1<<7)+FG_GRAY,
};
typedef unsigned char color_t;
static inline void set_fg(color_t c)
{
	printf("\033[%hhu;%hhum",
			(unsigned char)(c&(1<<7)?BOLD:NORMAL),
			(unsigned char)(c&~(1<<7)));
}
static inline void set_bg(color_t c)
{ // BOLD bit ignored; no standard bold background colors
	printf("\033[%hhum",
			(unsigned char)((10+c)&~(1<<7)));
}
#endif
