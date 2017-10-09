///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2017 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

var addv = function (v1, v2) {return {x: v1 . x + v2 . x, y: v1 . y + v2 . y};};
var subv = function (v1, v2) {return {x: v1 . x - v2 . x, y: v1 . y - v2 . y};};
var subvbc = function (v1, v2) {return {x: v1 . x - v2 . left, y: v1 . y - v2 . top};};
var scalv = function (vector, scale) {return {x: vector . x * scale, y: vector . y * scale};};

var spacer = function (height, options) {
  this . height = height;
  this . colour = 'black'; this . alt = 'red';
  this . draw = function (ctx, radius) {};
  this . sub_draw = function (ctx, radius) {
    ctx . fillStyle = this . marking_colour ? this . marking_colour : this . colour;
    ctx . strokeStyle = this . colour;
    ctx . textAlign = 'center';
    ctx . font = (this . height * 0.5) + 'px arial';
    ctx . save ();
    this . draw (ctx, radius);
    ctx . restore ();
  };
  for (var key in options) this [key] = options [key];
};

var scal = function (height, options) {
  var s = new spacer (height, options);
  s . draw = function (ctx, radius) {
    ctx . fillStyle = 'black';
    ctx . textAlign = 'center';
    ctx . font = '16px arial';
    for (var ind = 0; ind < 24; ind++) {
      ctx . beginPath ();
      ctx . moveTo (0, - radius);
      ctx . lineTo (0, - radius - height, 0);
      ctx . stroke ();
      ctx . fillText (ind, 0, - radius - height);
      ctx . rotate (Math . PI / 12);
    }
  };
  return s;
};

var draw_ticks = function (ctx, init, limit, from, to) {
  ctx . save ();
  var next = 0, log = 0;
  for (var ind = init; ind < limit; ind++) {
    next = Math . log10 (ind);
    ctx . rotate ((next - log) * 2 * Math . PI);
    ctx . moveTo (0, from); ctx . lineTo (0, to);
    log = next;
  }
  ctx . restore ();
};

var scale_C = function (height, options) {
  var s = new spacer (height, options);
  s . draw = function (ctx, radius) {
    var h0 = - radius - this . height;
    var h5 = h0 + this . height * 0.5, h4 = h0 + this . height * 0.4, h3 = h0 + this . height * 0.3, h2 = h0 + this . height * 0.2;
    ctx . beginPath ();
    var log = 0, next = 0, ind = 0;
    ctx . moveTo (0, h0); ctx . lineTo (0, h5);
    ctx . fillText ("1.0", 0, h5);
    ctx . save ();
    for (ind = 2; ind < 10; ind++) {
      next = Math . log10 (ind);
      ctx . rotate ((next - log) * 2 * Math . PI);
      ctx . fillText (ind + ".0", 0, h5);
      ctx . moveTo (0, h0); ctx . lineTo (0, h5);
      log = next;
    }
    ctx . restore ();
    draw_ticks (ctx, 1.5, 10, h0, h4);
    for (ind = 1.1; ind < 1.5; ind += 0.1) draw_ticks (ctx, ind, 10, h0, h3);
    for (ind = 1.6; ind < 2; ind += 0.1) draw_ticks (ctx, ind, 10, h0, h3);
    for (ind = 1.05; ind < 2; ind += 0.05) draw_ticks (ctx, ind, 3, h0, h2);
    ctx . stroke ();
    ctx . strokeStyle = 'red';
    ctx . beginPath ();
    ctx . ellipse (0, 0, radius, radius + this . height * 0.25, 0, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, radius + this . height * 0.25, radius + this . height * 0.5, Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, radius + this . height * 0.5, radius + this . height * 0.75, Math . PI, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, radius + this . height * 0.75, radius + this . height, Math . PI * 1.5, 0, Math . PI * 0.5);
    ctx . closePath ();
    ctx . stroke ();
    ctx . strokeStyle = 'blue';
    ctx . beginPath ();
    ctx . moveTo (radius, 0);
    for (ind = 0; ind < 96; ind ++) {
      ctx . rotate (Math . PI / 48);
      ctx . lineTo (radius + ind * this . height / 96, 0);
    }
    ctx . stroke ();
  };
  return s;
};

var draw_spiral_ticks = function (ctx, init, limit, from, to, spirals, height) {
  ctx . save ();
  var next = 0, log = 0, offset = 0;
  for (var ind = init; ind < limit; ind++) {
    next = spirals * Math . log10 (ind);
    ctx . rotate ((next - log) * 2 * Math . PI);
    offset = height * next;
    ctx . moveTo (0, from - offset); ctx . lineTo (0, to - offset);
    log = next;
  }
  ctx . restore ();
};

var spiral25 = function (height, options) {
  var s = new spacer (height * 26, options);
  s . sub_height = height;
  s . draw = function (ctx, radius) {
    var h0 = - radius - this . height;
    var h10 = h0 + this . height, h5 = h0 + this . height * 0.5, h4 = h0 + this . height * 0.4, h3 = h0 + this . height * 0.3, h2 = h0 + this . height * 0.2;
    var ind;
    ctx . strokeStyle = 'red';
    ctx . beginPath ();
    ctx . arc (0, 0, radius, 0, 6); ctx . arc (0, 0, radius + this . height, 0, 6);
    ctx . stroke ();
    ctx . strokeStyle = 'black';
    ctx . beginPath ();
    for (ind = 1; ind < 26; ind++) {
      ctx . ellipse (0, 0, radius + this . height * ind / 26, radius + this . height * (ind + 0.25) / 26, Math . PI * -0.5, 0, Math . PI * 0.5);
      ctx . ellipse (0, 0, radius + this . height * (ind + 0.25) / 26, radius + this . height * (ind + 0.5) / 26, 0, 0, Math . PI * 0.5);
      ctx . ellipse (0, 0, radius + this . height * (ind + 0.5) / 26, radius + this . height * (ind + 0.75) / 26, Math . PI * 0.5, 0, Math . PI * 0.5);
      ctx . ellipse (0, 0, radius + this . height * (ind + 0.75) / 26, radius + this . height * (ind + 1) / 26, Math . PI, 0, Math . PI * 0.5);
    }
    ctx . stroke ();
    ctx . beginPath ();
    draw_spiral_ticks (ctx, 1, 11, - radius - height, - radius, 25, this . sub_height);
    ctx . stroke ();
    ctx . strokeStyle = 'blue'; ctx . fillStyle = 'yellow';
    var log2, offset, dist;
    ctx . beginPath ();
    log2 = Math . log10 (3) * 25;
    offset = 2 * log2 * Math . PI;
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, 0, Math . PI * 0.5);
    ctx . closePath ();
    log2 = Math . log10 (2) * 25;
    offset = 2 * log2 * Math . PI;
    ctx . moveTo (- radius - height * 12, 0);
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, Math . PI * 0.5, 0, true);
    ctx . closePath ();
    //ctx . stroke ();
    ctx . fill ();

    ctx . beginPath ();
    log2 = Math . log10 (5) * 25;
    offset = 2 * (log2 - 11) * Math . PI;
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, 0, Math . PI * 0.5);
    ctx . closePath ();
    log2 = Math . log10 (4) * 25;
    offset = 2 * log2 * Math . PI;
    dist = radius + height * 16;
    ctx . moveTo (dist * Math . cos (offset), dist * Math . sin (offset));
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, Math . PI * 0.5, 0, true);
    ctx . closePath ();
    //ctx . stroke ();
    ctx . fill ();

    ctx . beginPath ();
    log2 = Math . log10 (7) * 25;
    offset = 2 * log2 * Math . PI;
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, 0, Math . PI * 0.5);
    ctx . closePath ();
    log2 = Math . log10 (6) * 25;
    offset = 2 * log2 * Math . PI;
    dist = radius + height * 21;
    ctx . moveTo (dist * Math . cos (offset), dist * Math . sin (offset));
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, Math . PI * 0.5, 0, true);
    ctx . closePath ();
    //ctx . stroke ();
    ctx . fill ();

    ctx . beginPath ();
    log2 = Math . log10 (9) * 25;
    offset = 2 * (log2 - 11) * Math . PI;
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, 0, Math . PI * 0.5);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, 0, Math . PI * 0.5);
    ctx . closePath ();
    log2 = Math . log10 (8) * 25;
    offset = 2 * log2 * Math . PI;
    dist = radius + height * 24;
    ctx . moveTo (dist * Math . cos (offset), dist * Math . sin (offset));
    ctx . ellipse (0, 0, log2 * height + radius, (log2 + 0.25) * height + radius, offset - Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.75) * height + radius, (log2 + 1) * height + radius, offset + Math . PI, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.5) * height + radius, (log2 + 0.75) * height + radius, offset + Math . PI * 0.5, Math . PI * 0.5, 0, true);
    ctx . ellipse (0, 0, (log2 + 0.25) * height + radius, (log2 + 0.5) * height + radius, offset, Math . PI * 0.5, 0, true);
    ctx . closePath ();
    //ctx . stroke ();
    ctx . fill ();
  };
  return s;
};

var Disc = function (options) {
  this . stator = 0;
  this . disc_colour = 'white';
  this . border_colour = 'black';
  this . scales = [];
  this . width = function () {var h = 0; for (var ind in this . scales) h += this . scales [ind] . height; return h;};
  this . draw = function (ctx, from) {
    ctx . fillStyle = this . disc_colour; ctx . strokeStyle = this . border_colour;
    ctx . beginPath ();
    ctx . arc (0, 0, from + this . width (), 0, Math . PI * 2);
    ctx . closePath ();
    if (from > 0) {ctx . moveTo (from, 0); ctx . arc (0, 0, from, 0, Math . PI * 2, true); ctx . closePath ();}
    ctx . fill ();
    ctx . stroke ();
    var ind;
    var radius = from;
    for (ind in this . scales) {
      ctx . save ();
      this . scales [ind] . sub_draw (ctx, radius);
      radius += this . scales [ind] . height;
      ctx . restore ();
    }
  };
  for (var key in options) this [key] = options [key];
};

var Sliderule = function (options) {
  this . position = {x: 0, y: 0};
  this . discs = [];
  this . draw = function (ctx) {
    ctx . translate (this . position . x, this . position . y);
    var ind;
    var radius = 0;
    for (ind in this . discs) {ctx . save (); this . discs [ind] . draw (ctx, radius); ctx . restore (); radius += this . discs [ind] . width ();}
  };
  this . mover = null;
  this . draww = function (ctx) {
    if (this . cursor_target < this . cursor_position) {
      this . cursor_position -= this . animation_delta;
      if (this . cursor_position < this . cursor_target) this . cursor_position = this . cursor_target;
    }
    if (this . cursor_target > this . cursor_position) {
      this . cursor_position += this . animation_delta;
      if (this . cursor_position > this . cursor_target) this . cursor_position = this . cursor_target;
    }
    if (this . inactive) {
      for (var ind in this . rules) this . rules [ind] . animate ();
      return;
    }
    ctx . translate (this . position . x, this . position . y);
    ctx . save ();
    var ind;
    // BACK BRACES
    for (ind in this . backBraces) {ctx . save (); this . backBraces [ind] . draw (ctx, this); ctx . restore ();}
    // RULES
    ctx . save ();
    ctx . translate (this . length * this . left_margin, 0);
    for (ind in this . rules) {
      this . rules [ind] . draw (ctx, this . length, this);
      ctx . translate (0, this . rules [ind] . ruleHeight ());
    }
    ctx . restore ();
    // BRACES
    for (ind in this . braces) {ctx . save (); this . braces [ind] . draw (ctx, this); ctx . restore ();}
    ctx . save ();
    // STATIC MARKINGS
    if (this . static_markings) this . drawMarkings (ctx, this . length * this . static_markings_shift, this . static_markings_align, true);
    ctx . restore ();
    ctx . translate (this . length * (this . left_margin + this . cursor_position), 0);
    // CURSOR GLASS BRACES
    for (ind in this . cursorGlassBraces) {ctx . save (); this . cursorGlassBraces [ind] . draw (ctx, this); ctx . restore ();}
    // CURSOR
    ctx . beginPath ();
    var le = - this . length * this . cursor_left_extension; var re = this . length * this . cursor_right_extension;
    roundRect (ctx, le, le, - this . cursor_rounding - this . cursor_top_margin, re, re, this . height () + this . cursor_rounding + this . cursor_bottom_margin, this . cursor_rounding);
    if (this . cursorGlass) {ctx . fillStyle = this . cursorGlass; ctx . fill ();}
    if (this . cursorFrame) {ctx . strokeStyle = this . cursorFrame; ctx . stroke ();}
    if (this . cursorHairline) {
      ctx . beginPath ();
      ctx . moveTo (0, - this . hairline_top); ctx . lineTo (0, this . height () + this . hairline_bottom);
      ctx . strokeStyle = this . cursorHairline;
      ctx . stroke ();
    }
    // GLASSES
    for (ind in this . glasses) this . glasses [ind] . draw (ctx, this);
    // CURSORS
    for (ind in this . cursors) {this . cursors [ind] . draw (ctx, this . length, this . cursor_position);}
    // CURSOR BRACES
    for (ind in this . cursorBraces) {ctx . save (); this . cursorBraces [ind] . draw (ctx, this); ctx . restore ();}
    // CURSOR MARKINGS
    if (this . cursor_markings && this . cursorHairline) this . drawMarkings (ctx, this . length * this . cursor_markings_shift, this . cursor_markings_align, false);
    // CURSOR EXTRA MARKINGS
    if (this . extra_cursor_markings || this . cursor_markings) this . drawExtraMarkings (ctx);
    ctx . restore ();
    ctx . translate (0, this . height ());
  };
  this . changed = function () {return this . target != this . shift;};
  for (var key in options) this [key] = options [key];
};

var Sliderules = function (options) {
  this . requireRedraw = true;
  this . position = {x: 200.5, y: 200.5};
  this . scale = 1; this . scaling_factor = Math . pow (2, 1 / 12);
  this . background_colour = '#99f';
  this . background_translation = {x: 0, y: 0};
  this . sliderules = [];
  this . synchroniseMove = function (delta, position) {
  	position = scalv (position, 1 / this . scale);
  	var v1 = subv (subv (position, delta), this . position);
  	var v2 = subv (position, this . position);
  	var a1 = Math . atan2 (v1 . y, v1 . x);
  	var a2 = Math . atan2 (v2 . y, v2 . x);
  	console . log (a1, a2, a2 - a1);
    this . position = addv (this . position, scalv (delta, 1 / this . scale));
    this . requireRedraw = true;
  };
  this . draw = function (ctx, width, height) {
    this . requireRedraw = false;
    ctx . setTransform (1, 0, 0, 1, 0, 0);
    ctx . fillStyle = this . background_colour;
    ctx . fillRect (0, 0, width, height);
    if (this . background) {
      ctx . save ();
      if (this . background_scaling) ctx . scale (this . background_scaling . x, this . background_scaling . y);
      ctx . drawImage (this . background, this . background_translation . x, this . background_translation . y);
      ctx . restore ();
    }
    ctx . scale (this . scale, this . scale);
    ctx . translate (this . position . x, this . position . y);
    for (var ind in this . sliderules) this . sliderules [ind] . draw (ctx);
  };
  this . noChange = function () {
  	if (this . requireRedraw) return false;
  	for (var ind in this . sliderules) {if (this . sliderules [ind] . changed ()) return false;}
  	return true;
  };
  this . resetMovers = function () {for (var ind in this . sliderules) this . sliderules [ind] . mover = null;};
  for (var key in options) this [key] = options [key];
};