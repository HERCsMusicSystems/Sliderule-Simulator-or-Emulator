
#include "SlideRule.h"

void Rule :: draw (cairo_t * cr, SlideRule * root) {
	cairo_set_line_width (cr, 0.5);
	cairo_rectangle (cr,
		0.5 + root -> horizontal_margin - left_margin * root -> length,
		0.5 + root -> vertical_margin + vertical_position, (left_margin + right_margin + 1.0) * root -> length, height);
	cairo_stroke (cr);
}

void SlideRuleSide :: draw (cairo_t * cr, SlideRule * root) {
	Rule * r = rules;
	while (r != 0) {r -> draw (cr, root); r = r -> next;}
}

void SlideRule :: draw (cairo_t * cr) {
	if (current != 0) current -> draw (cr, this);
	cairo_set_line_width (cr, 1.0);
	cairo_rectangle (cr, 0.5, 0.5, 4.0, 4.0);
	cairo_rectangle (cr, 20.5, 20.5, 100.0, 200.0);
	cairo_stroke (cr);
}

void SlideRule :: setSide (int side) {
	current = root;
	while (side-- > 0 && current != 0) current = current -> next;
}

SlideRuleSide * SlideRule :: createSide (void) {return root = current = new SlideRuleSide (root);}

SlideRuleSide :: SlideRuleSide (SlideRuleSide * next) {
	rules = 0;
	this -> next = next;
}
SlideRuleSide :: ~ SlideRuleSide (void) {
	if (rules != 0) delete rules; rules = 0;
	if (next != 0) delete next; next = 0;
}

SlideRule :: SlideRule (void) {current = root = 0;}
SlideRule :: ~ SlideRule (void) {
	current = 0;
	if (root != 0) delete root; root = 0;
}

