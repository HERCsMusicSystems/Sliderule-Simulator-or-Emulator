
#include "SlideRule.h"
#include <string.h>

///////////
// Scale //
///////////

void Scale :: draw (cairo_t * cr, point shift) {
	cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
	cairo_move_to (cr, shift . x, shift . y);
	cairo_line_to (cr, shift . x, shift . y + h0);
	cairo_stroke (cr);
}

Scale :: Scale (Rule * root) {
	this -> root = root;
	if (root != 0) {
		next = root -> scales;
		root -> scales = this;
		vertical_position = root -> vertical_position;
		h0 = root -> h0;
		h1 = root -> h1;
		h2 = root -> h2;
		h3 = root -> h3;
		orientation = root -> orientation;
	} else {
		next = 0;
		vertical_position = 0.0;
		h0 = 24.0; h1 = 12.0; h2 = 6.0; h3 = 3.0;
		orientation = true;
	}
	h0 = 4.0;
}

Scale :: ~ Scale (void) {
	if (next != 0) delete next;
}

MainScale :: MainScale (Rule * root) : Scale (root) {
}

MainScale :: ~ MainScale (void) {}

//////////
// RULE //
//////////

void Rule :: draw (cairo_t * cr) {
	SlideRule * rr = root -> root;
	cairo_set_line_width (cr, 1.0);
	double index_position = rr -> horizontal_margin + rr -> longest_left_margin;
	cairo_rectangle (cr,
		index_position - left_margin * rr -> length,
		rr -> vertical_margin + vertical_position,
		(left_margin + right_margin + 1.0) * rr -> length, height);
	cairo_set_source_rgb (cr, COLOUR (rule_colour));
	cairo_fill (cr);
	cairo_rectangle (cr,
		rr -> horizontal_margin + rr -> longest_left_margin - left_margin * rr -> length,
		rr -> vertical_margin + vertical_position,
		(left_margin + right_margin + 1.0) * rr -> length, height);
	cairo_set_source_rgb (cr, COLOUR (border_colour));
	cairo_stroke (cr);
	Scale * scale = scales;
	while (scale != 0) {
		scale -> draw (cr, point (index_position, rr -> vertical_margin + scale -> vertical_position));
		scale = scale -> next;
	}
	if (next != 0) next -> draw (cr);
}

Rule :: Rule (SlideRuleSide * root, int ind, Rule * next) {
	this -> root = root;
	slide_id = ind;
	this -> next = next;
	scales = 0;
	vertical_position = next == 0 ? 0.0 : next -> vertical_position + next -> height + 1;
	h0 = 24.0; h1 = 12.0; h2 = 6.0; h3 = 3.0;
	height = 0.0;
	left_margin = right_margin = 0.2;
	rule_colour = colour (1.0, 1.0, 1.0);
	border_colour = colour (0.0, 0.0, 0.0);
}

Rule :: ~ Rule (void) {
	if (scales != 0) delete scales;
	if (next != 0) delete next;
}

//////////
// SIDE //
//////////

void SlideRuleSide :: draw (cairo_t * cr) {
	cairo_set_source_rgb (cr, COLOUR (background_colour));
	cairo_paint (cr);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);
	Rule * r = rules;
	while (r != 0) {r -> draw (cr); r = r -> next;}
}

Rule * SlideRuleSide :: createRule (int ind) {
	rules = new Rule (this, ind, rules);
	rules -> left_margin = left_margin;
	rules -> right_margin = right_margin;
	rules -> rule_colour = rule_colour;
	rules -> border_colour = border_colour;
	return rules;
}

SlideRuleSide :: SlideRuleSide (SlideRule * root, SlideRuleSide * next) {
	this -> root = root;
	rules = 0;
	left_margin = right_margin = 0.2;
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
	if (current != 0) current -> draw (cr);
}

double SlideRule :: totalHeight (void) {
	if (current == 0) return 0.0;
	if (current -> rules == 0) return 0.0;
	return current -> rules -> vertical_position + current -> rules -> height;
}

void SlideRule :: recalculate_margins (void) {
	longest_left_margin = longest_right_margin = 0.0;
	if (current == 0) return;
	if (current -> rules == 0) return;
	Rule * rp = current -> rules;
	while (rp != 0) {
		double x = rp -> left_margin * length;
		if (x > longest_left_margin) longest_left_margin = x;
		x = rp -> right_margin * length;
		if (x > longest_right_margin) longest_right_margin = x;
		rp = rp -> next;
	}
	length_with_margins = longest_left_margin + length + longest_right_margin;
}

void SlideRule :: setSide (int side) {
	current = root;
	while (side-- > 0 && current != 0) current = current -> next;
}

SlideRuleSide * SlideRule :: createSide (void) {return root = current = new SlideRuleSide (this, root);}

SlideRule :: SlideRule (void) {
	current = root = 0;
	createSide ();
	length = 120.0;
	vertical_margin = 10.5;
	horizontal_margin = 48.5;
	cursor_position = 1.0;
	for (int ind = 0; ind < 128; ind++) slide_positions [ind] = 0.0;
	recalculate_margins ();
	strcpy (name, "Slide Rule Emulator");
}
SlideRule :: ~ SlideRule (void) {
	current = 0;
	if (root != 0) delete root; root = 0;
}

