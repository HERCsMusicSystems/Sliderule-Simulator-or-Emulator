
#ifndef _SLIDE_RULE
#define _SLIDE_RULE

#include "gtk/gtk.h"
#include "graphics2d.h"

class Scale;
class Rule;
class SlideRuleSide;
typedef SlideRuleSide * SlideRuleSidePointer;
class SlideRule;

class Scale {
public:
	Scale * next;
	Scale (void);
};

class Rule {
public:
	double vertical_position, height;
	double left_margin, right_margin;
	int slide_id; // 0 = stator, 1 = slide [1], 2 = slide [2], etc.
	colour rule_colour, border_colour;
public:
	Rule * next;
	void draw (cairo_t * cr, SlideRule * root);
	Rule (int id, Rule * next);
};

class SlideRuleSide {
public:
	Rule * rules;
	colour background_colour, rule_colour, border_colour;
	SlideRuleSide * next;
	void draw (cairo_t * cr, SlideRule * root);
	Rule * createRule (int id);
	SlideRuleSide (SlideRuleSide * next);
	~ SlideRuleSide (void);
};

class SlideRule {
private:
	SlideRuleSide * root;
public:
	SlideRuleSide * current;
	char name [128];
	double length, vertical_margin, horizontal_margin;
	void draw (cairo_t * cr);
	double totalHeight (void);
	double lengthWithMargins (void);
	void setSide (int side);
	SlideRuleSide * createSide (void);
	SlideRule (void);
	~ SlideRule (void);
};

#endif
