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

var spacer = function (height, options) {
  this . height = height;
  this . draw = function (ctx, radius) {};
  this . sub_draw = function (ctx, radius) {
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
  this . position = {x: 132.5, y: 132.5};
  this . scale = 1; this . scaling_factor = Math . pow (2, 1 / 12);
  this . background_colour = '#99f';
  this . background_translation = {x: 0, y: 0};
  this . sliderules = [];
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
  for (var key in options) this [key] = options [key];
};