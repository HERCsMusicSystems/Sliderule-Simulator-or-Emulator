
#include "SlideRule.h"

//////////
// RULE //
//////////

void Rule :: draw (cairo_t * cr, SlideRule * root) {
	cairo_set_line_width (cr, 1.0);
	cairo_rectangle (cr,
		root -> horizontal_margin - left_margin * root -> length,
		root -> vertical_margin + vertical_position, (left_margin + right_margin + 1.0) * root -> length, height);
	cairo_stroke (cr);
	if (next != 0) next -> draw (cr, root);
}

Rule :: Rule (int ind, Rule * next) {
	slide_id = ind;
	this -> next = next;
	vertical_position = 0.0;
	height = 0.0;
	left_margin = right_margin = 0.2;
}

//////////
// SIDE //
//////////

void SlideRuleSide :: draw (cairo_t * cr, SlideRule * root) {
	Rule * r = rules;
	while (r != 0) {r -> draw (cr, root); r = r -> next;}
}

Rule * SlideRuleSide :: createRule (int ind) {return rules = new Rule (ind, rules);}

SlideRuleSide * SlideRule :: createSide (void) {return root = current = new SlideRuleSide (root);}

SlideRuleSide :: SlideRuleSide (SlideRuleSide * next) {
	rules = 0;
	this -> next = next;
}
SlideRuleSide :: ~ SlideRuleSide (void) {
	if (rules != 0) delete rules; rules = 0;
	if (next != 0) delete next; next = 0;
}

void SlideRule :: draw (cairo_t * cr) {
	if (current != 0) current -> draw (cr, this);
	cairo_set_line_width (cr, 1.0);
	cairo_rectangle (cr, 0.5, 0.5, 4.0, 4.0);
	cairo_rectangle (cr, 20.5, 20.5, 100.0, 200.0);
	cairo_stroke (cr);
}


///////////////
// SLIDERULE //
///////////////

void SlideRule :: setSide (int side) {
	current = root;
	while (side-- > 0 && current != 0) current = current -> next;
}

SlideRule :: SlideRule (void) {
	current = root = 0;
	length = 120.0;
	vertical_margin = 10.5;
	horizontal_margin = 48.5;
}
SlideRule :: ~ SlideRule (void) {
	current = 0;
	if (root != 0) delete root; root = 0;
}

