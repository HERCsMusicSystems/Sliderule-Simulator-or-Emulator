
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
	Rule * root;
	double vertical_position;
	double h0, h1, h2, h3;
	bool orientation;
	Scale * next;
	virtual void draw (cairo_t * cr, point shift);
	Scale (Rule * root);
	~ Scale (void);
};

class MainScale : public Scale {
public:
	MainScale (Rule * root);
	~ MainScale (void);
};

class Rule {
public:
	double vertical_position, height;
	double left_margin, right_margin;
	int slide_id; // -1 = stator, 0 = slide [0], 1 = slide [1], 2 = slide [2], etc.
	colour rule_colour, border_colour;
	double h0, h1, h2, h3;
	bool orientation;
public:
	SlideRuleSide * root;
	Rule * next;
	Scale * scales;
	void draw (cairo_t * cr);
	Rule (SlideRuleSide * root, int id, Rule * next);
	~ Rule (void);
};

class SlideRuleSide {
public:
	SlideRule * root;
	Rule * rules;
	double left_margin, right_margin;
	colour background_colour, rule_colour, border_colour;
	SlideRuleSide * next;
	void draw (cairo_t * cr);
	Rule * createRule (int id);
	SlideRuleSide (SlideRule * root, SlideRuleSide * next);
	~ SlideRuleSide (void);
};

class SlideRule {
private:
	SlideRuleSide * root;
public:
	SlideRuleSide * current;
	char name [128];
	double cursor_position;
	double slide_positions [128];
	double length, vertical_margin, horizontal_margin;
	double longest_left_margin, longest_right_margin, length_with_margins;
	void draw (cairo_t * cr);
	double totalHeight (void);
	void recalculate_margins (void);
	void setSide (int side);
	SlideRuleSide * createSide (void);
	SlideRule (void);
	~ SlideRule (void);
};

#endif
