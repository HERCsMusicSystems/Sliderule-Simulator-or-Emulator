
#include "SlideRule.h"
#include <string.h>

//////////
// RULE //
//////////

void Rule :: draw (cairo_t * cr, SlideRule * root) {
	cairo_set_line_width (cr, 1.0);
	cairo_rectangle (cr,
		root -> horizontal_margin - left_margin * root -> length,
		root -> vertical_margin + vertical_position, (left_margin + right_margin + 1.0) * root -> length, height);
	cairo_set_source_rgb (cr, COLOUR (rule_colour));
	cairo_fill (cr);
	cairo_rectangle (cr,
		root -> horizontal_margin - left_margin * root -> length,
		root -> vertical_margin + vertical_position, (left_margin + right_margin + 1.0) * root -> length, height);
	cairo_set_source_rgb (cr, COLOUR (border_colour));
	cairo_stroke (cr);
	if (next != 0) next -> draw (cr, root);
}

Rule :: Rule (int ind, Rule * next) {
	slide_id = ind;
	this -> next = next;
	vertical_position = next == 0 ? 0.0 : next -> vertical_position + next -> height + 1;
	height = 0.0;
	left_margin = right_margin = 0.2;
	rule_colour = colour (1.0, 1.0, 1.0);
	border_colour = colour (0.0, 0.0, 0.0);
}

//////////
// SIDE //
//////////

void SlideRuleSide :: draw (cairo_t * cr, SlideRule * root) {
	cairo_set_source_rgb (cr, COLOUR (background_colour));
	cairo_paint (cr);
	Rule * r = rules;
	while (r != 0) {r -> draw (cr, root); r = r -> next;}
}

Rule * SlideRuleSide :: createRule (int ind) {
	rules = new Rule (ind, rules);
	rules -> rule_colour = rule_colour;
	rules -> border_colour = border_colour;
	return rules;
}

SlideRuleSide :: SlideRuleSide (SlideRuleSide * next) {
	rules = 0;
	background_colour = colour (0.0, 0.0, 1.0);
	rule_colour = colour (1.0, 1.0, 1.0);
	border_colour = colour (0.0, 0.0, 0.0);
	this -> next = next;
}
SlideRuleSide :: ~ SlideRuleSide (void) {
	if (rules != 0) delete rules; rules = 0;
	if (next != 0) delete next; next = 0;
}


///////////////
// SLIDERULE //
///////////////

void SlideRule :: draw (cairo_t * cr) {
	if (current != 0) current -> draw (cr, this);
	//cairo_set_line_width (cr, 1.0);
	//cairo_rectangle (cr, 0.5, 0.5, 4.0, 4.0);
	//cairo_rectangle (cr, 20.5, 20.5, 100.0, 200.0);
	//cairo_stroke (cr);
}

double SlideRule :: totalHeight (void) {
	if (current == 0) return 0.0;
	if (current -> rules == 0) return 0.0;
	return current -> rules -> vertical_position + current -> rules -> height;
}

double SlideRule :: lengthWithMargins (void) {
	if (current == 0) return length;
	if (current -> rules == 0) return length;
	Rule * rp = current -> rules;
	double margin = length;
	while (rp != 0) {
		double l = (rp -> left_margin + rp -> right_margin + 1.0) * length;
		if (l > margin) margin = l;
		rp = rp -> next;
	}
	return margin;
}

void SlideRule :: setSide (int side) {
	current = root;
	while (side-- > 0 && current != 0) current = current -> next;
}

SlideRuleSide * SlideRule :: createSide (void) {return root = current = new SlideRuleSide (root);}

SlideRule :: SlideRule (void) {
	current = root = 0;
	createSide ();
	length = 120.0;
	vertical_margin = 10.5;
	horizontal_margin = 48.5;
	strcpy (name, "Slide Rule Emulator");
}
SlideRule :: ~ SlideRule (void) {
	current = 0;
	if (root != 0) delete root; root = 0;
}

