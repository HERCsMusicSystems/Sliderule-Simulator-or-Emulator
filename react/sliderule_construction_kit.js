
var roundRect = function (ctx, left, top, right, bottom, radius) {
  var step = Math . PI * 0.5; ctx . moveTo (left + radius, top); ctx . lineTo (right - radius, top); ctx . arc (right - radius, top + radius, radius, -step, 0);
  ctx . lineTo (right, bottom - radius); ctx . arc (right - radius, bottom - radius, radius, 0, step); ctx . lineTo (left + radius, bottom);
  ctx . arc (left + radius, bottom - radius, radius, step, step + step); ctx . lineTo (left, top + radius); ctx . arc (left + radius, top + radius, radius, step + step, - step);
};

var addv = function (v1, v2) {return {x: v1 . x + v2 . x, y: v1 . y + v2 . y};};
var subv = function (v1, v2) {return {x: v1 . x - v2 . x, y: v1 . y - v2 . y};};
var scalv = function (vector, scale) {return {x: vector . x * scale, y: vector . y * scale};};

var tick = function (ctx, x, height) {ctx . beginPath (); ctx . moveTo (x, 0); ctx . lineTo (x, - height); ctx . stroke ();};
var stick = function (ctx, x, base, height) {ctx . beginPath (); ctx . moveTo (x, - base); ctx . lineTo (x, - height); ctx . stroke ();};
var mark = function (ctx, ind, x, height) {tick (ctx, x, height); if (height < 0) ctx . fillText (ind, x, - height - height - 3); else ctx . fillText (ind, x, - height - 2);};
var smark = function (ctx, ind, x, base, height) {
  stick (ctx, x, base, height);
  if (height < 0) ctx . fillText (ind, x, - height - height - 3);
  else ctx . fillText (ind, x, - height - 2);
};

var draw_XRI = function (ctx, fn, length, from, to, limit, height, step, micro_from, micro_to) {
  var esc = true; var location;
  to -= 0.0000001;
  for (var ind = from; esc && ind < to; ind += step) {
    for (var sub = micro_from; esc && sub < micro_to; sub += micro_from) {
      location = fn (ind + sub);
      if (location < limit) esc = false;
      else tick (ctx, length * location, height);
    }
  }
};
var draw_XR = function (ctx, fn, length, from, to, limit, height, step, micro_from, micro_to) {
  var esc = true; var location;
  to -= 0.0000001;
  for (var ind = from; esc && ind < to; ind += step) {
    for (var sub = micro_from; esc && sub < micro_to; sub += micro_from) {
      location = fn (ind + sub);
      if (location > limit) esc = false;
      else tick (ctx, length * location, height);
    }
  }
};
var draw_50R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 10, 5, 10);};
var draw_10R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 5, 1, 5);};
var draw_05R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 1, 0.5, 1);};
var draw_02R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 1, 0.2, 1);};
var draw_01R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 0.5, 0.1, 0.5);};
var draw_005R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 0.1, 0.05, 0.1);};
var draw_002R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 0.1, 0.02, 0.1);};
var draw_001R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 0.05, 0.01, 0.05);};
var draw_0001R = function (ctx, fn, length, from, to, limit, height) {draw_XR (ctx, fn, length, from, to, limit, height, 0.005, 0.001, 0.005);};
var draw_XL = function (ctx, fn, length, from, to, limit, height, step, micro_from, micro_to) {
  var esc = true; var location;
  from += 0.0000001;
  for (var ind = to; esc && ind > from; ind -= step) {
    for (var sub = micro_from; esc & sub < micro_to; sub += micro_from) {
      location = fn (ind - sub);
      if (location < limit) esc = false;
      else tick (ctx, length * location, height);
    }
  }
};
var draw_05L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 1, 0.5, 1);};
var draw_02L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 1, 0.2, 1);};
var draw_01L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 0.5, 0.1, 0.5);};
var draw_005L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 0.1, 0.05, 0.1);};
var draw_002L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 0.1, 0.02, 0.1);};
var draw_001L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 0.05, 0.01, 0.05);};
var draw_0001L = function (ctx, fn, length, from, to, limit, height) {draw_XL (ctx, fn, length, from, to, limit, height, 0.005, 0.001, 0.005);};
var draw_MR = function (ctx, fn, length, from, to, limit, height) {draw_MRS (ctx, fn, length, from, to, 1, limit, height);};
var draw_MRS = function (ctx, fn, length, from, to, step, limit, height) {
  var esc = true; var location;
  for (var ind = from; esc && ind <= to; ind += step) {
    location = fn (ind);
    if (location > limit) esc = false;
    else mark (ctx, ind, length * location, height);
  }
};
var draw_ML = function (ctx, fn, length, from, to, limit, height) {draw_MLS (ctx, fn, length, from, to, 1, limit, height);};
var draw_MLS = function (ctx, fn, length, from, to, step, limit, height) {
  var esc = true; var location;
  for (var ind = to; esc && ind >= from; ind -= step) {
    location = fn (ind);
    if (location < limit) esc = false;
    else mark (ctx, ind, length * location, height);
  }
};

var draw_log = function (ctx, length, height, scale) {
  mark (ctx, 1, length, height * 0.5);
  smark (ctx, '\u03c0', length * Math . log10 (Math . PI), height * 0.2, height * 0.5);
  smark (ctx, 'e', length * Math . log10 (Math . E), height * 0.2, height * 0.5);
  smark (ctx, 'c', length * Math . log10 (Math . sqrt (4 / Math . PI)), height * 0.2, height * 0.5);
  smark (ctx, 'c1', length * (0.5 + Math . log10 (Math . sqrt (4 / Math . PI))), height * 0.2, height * 0.5);
  draw_MR (ctx, Math . log10, length, 1, 9, 1, height * 0.5);
  draw_002R (ctx, Math . log10, length, 1, 2, 1, height * 0.2);
  draw_01R (ctx, Math . log10, length, 1, 10, 1, height * 0.3);
  draw_05R (ctx, Math . log10, length, 1, 10, 1, height * 0.4);
  ctx . translate (length, 0);
  ctx . strokeStyle = scale . alt; ctx . fillStyle = scale . alt;
  draw_002R (ctx, Math . log10, length, 1, 10, scale . right_extension, height * 0.2);
  draw_01R (ctx, Math . log10, length, 1, 10, scale . right_extension, height * 0.3);
  ctx . translate (- length - length, 0);
  draw_01L (ctx, Math . log10, length, 1, 10, 1 - scale . left_extension, height * 0.2);
  draw_05L (ctx, Math . log10, length, 1, 10, 1 - scale . left_extension, height * 0.3);
  draw_ML (ctx, Math . log10, length, 1, 9, 1 - scale . left_extension, height * 0.5);
};
var draw_one_log = function (ctx, length, height) {
  draw_MR (ctx, Math . log10, length, 1, 9, 1, height * 0.5);
  draw_05R (ctx, Math . log10, length, 1, 8, 1, height * 0.4);
  draw_02R (ctx, Math . log10, length, 8, 10, 1, height * 0.3);
  draw_01R (ctx, Math . log10, length, 1, 8, 1, height * 0.3);
  draw_002R (ctx, Math . log10, length, 1, 2, 1, height * 0.2);
};
var draw_log_log = function (ctx, length, height, scale) {
  mark (ctx, 1, length, height * 0.5); length *= 0.5;
  smark (ctx, '\u03c0', length * Math . log10 (Math . PI), height * 0.2, height * 0.5);
  smark (ctx, 'e', length * Math . log10 (Math . E), height * 0.2, height * 0.5);
  smark (ctx, '\u03c0', length * Math . log10 (Math . PI * 10), height * 0.2, height * 0.5);
  smark (ctx, 'e', length * Math . log10 (Math . E * 10), height * 0.2, height * 0.5);
  draw_one_log (ctx, length, height); ctx . translate (length, 0); draw_one_log (ctx, length, height);
  ctx . strokeStyle = scale . alt; ctx . fillStyle = scale . alt;
  ctx . translate (length, 0);
  draw_05R (ctx, Math . log10, length, 1, 9, scale . right_extension * 2, height * 0.4);
  draw_01R (ctx, Math . log10, length, 1, 8, scale . right_extension * 2, height * 0.3);
  draw_002R (ctx, Math . log10, length, 1, 2, scale . right_extension * 2, height * 0.2);
  ctx . translate (length * -3, 0);
  draw_ML (ctx, Math . log10, length, 1, 9, 1 - scale . left_extension * 2, height * 0.5);
  draw_05L (ctx, Math . log10, length, 1, 8, 1 - scale . left_extension * 2, height * 0.4);
  draw_02L (ctx, Math . log10, length, 8, 10, 1 - scale . left_extension * 2, height * 0.3);
  draw_01L (ctx, Math . log10, length, 1, 8, 1 - scale . left_extension * 2, height * 0.3);
};
var draw_log_log_log = function (ctx, length, height, scale) {
  mark (ctx, 1, length, height * 0.5); length /= 3;
  draw_one_log (ctx, length, height); ctx . translate (length, 0); draw_one_log (ctx, length, height); ctx . translate (length, 0); draw_one_log (ctx, length, height);
  ctx . strokeStyle = scale . alt; ctx . fillStyle = scale . alt;
  ctx . translate (length, 0);
  draw_05R (ctx, Math . log10, length, 1, 9, scale . right_extension * 2, height * 0.4);
  draw_01R (ctx, Math . log10, length, 1, 8, scale . right_extension * 2, height * 0.3);
  draw_002R (ctx, Math . log10, length, 1, 2, scale . right_extension * 2, height * 0.2);
  ctx . translate (length * -3, 0);
  draw_ML (ctx, Math . log10, length, 1, 9, 1 - scale . left_extension * 2, height * 0.5);
  draw_05L (ctx, Math . log10, length, 1, 8, 1 - scale . left_extension * 2, height * 0.4);
  draw_02L (ctx, Math . log10, length, 8, 10, 1 - scale . left_extension * 2, height * 0.3);
  draw_01L (ctx, Math . log10, length, 1, 8, 1 - scale . left_extension * 2, height * 0.3);
};
var fn_lin = function (value) {return value * 0.1;};
var draw_lin = function (ctx, length, height, scale) {
  draw_MR (ctx, fn_lin, length, 0, 10, 1, height * 0.5);
  draw_05R (ctx, fn_lin, length, 0, 10, 1, height * 0.4);
  draw_01R (ctx, fn_lin, length, 0, 10, 1, height * 0.3);
  draw_005R (ctx, fn_lin, length, 0, 10, 1, height * 0.2);
};

var fn_sin_deg = function (value) {return Math . log10 (10 * Math . sin (value * Math . PI / 180));};
var draw_deg = function (ctx, length, height, scale) {
  draw_MRS (ctx, fn_sin_deg, length, 10, 35, 5, 1, height * 0.5);
  draw_MRS (ctx, fn_sin_deg, length, 40, 90, 10, 1, height * 0.5);
  draw_MLS (ctx, fn_sin_deg, length, 4, 9, 1, - scale . left_extension, height * 0.5);
  //draw_MLS (ctx, fn_sin_deg, length, 3.5, 5.5, 1, - scale . left_extension, height * 0.4);
  draw_10R (ctx, fn_sin_deg, length, 15, 80, 1, height * 0.4);
  draw_10R (ctx, fn_sin_deg, length, 10, 15, 1, height * 0.5);
  draw_50R (ctx, fn_sin_deg, length, 40, 90, 1, height * 0.5);
  draw_05L (ctx, fn_sin_deg, length, 3, 15, - scale . left_extension, height * 0.3);
  draw_01L (ctx, fn_sin_deg, length, 3, 15, - scale . left_extension, height * 0.2);
};

var fn_pe = function (value) {return Math . log10 (10 * Math . sqrt (1 - value * value));};
var draw_pe = function (ctx, length, height, scale) {
  mark (ctx, ".995", length * fn_pe (0.995), height * 0.5);
  mark (ctx, ".99", length * fn_pe (0.99), height * 0.5);
  mark (ctx, ".98", length * fn_pe (0.98), height * 0.5);
  mark (ctx, ".97", length * fn_pe (0.97), height * 0.5);
  mark (ctx, ".96", length * fn_pe (0.96), height * 0.5);
  mark (ctx, ".95", length * fn_pe (0.95), height * 0.5);
  mark (ctx, ".94", length * fn_pe (0.94), height * 0.5);
  mark (ctx, ".93", length * fn_pe (0.93), height * 0.5);
  mark (ctx, ".92", length * fn_pe (0.92), height * 0.5);
  mark (ctx, ".91", length * fn_pe (0.91), height * 0.5);
  mark (ctx, ".9", length * fn_pe (0.9), height * 0.5);
  mark (ctx, ".8", length * fn_pe (0.8), height * 0.5);
  mark (ctx, ".7", length * fn_pe (0.7), height * 0.5);
  mark (ctx, ".6", length * fn_pe (0.6), height * 0.5);
  mark (ctx, ".5", length * fn_pe (0.5), height * 0.5);
  mark (ctx, ".4", length * fn_pe (0.4), height * 0.5);
  mark (ctx, ".3", length * fn_pe (0.3), height * 0.5);
  mark (ctx, ".2", length * fn_pe (0.2), height * 0.5);
  mark (ctx, "", length * fn_pe (0.1), height * 0.5);
  mark (ctx, 0, length, height * 0.5);
  draw_XRI (ctx, fn_pe, length, 0.2, 0.3, - scale . left_extension, height * 0.4, 0.1, 0.05, 0.1);
  draw_XRI (ctx, fn_pe, length, 0.3, 0.4, - scale . left_extension, height * 0.3, 0.1, 0.02, 0.1);
  draw_XRI (ctx, fn_pe, length, 0.4, 0.9, - scale . left_extension, height * 0.4, 0.1, 0.05, 0.1);
  draw_XRI (ctx, fn_pe, length, 0.4, 0.9, - scale . left_extension, height * 0.2, 0.05, 0.01, 0.05);
  draw_XRI (ctx, fn_pe, length, 0.9, 0.99, - scale . left_extension, height * 0.2, 0.005, 0.001, 0.005);
  draw_XRI (ctx, fn_pe, length, 0.9, 0.99, - scale . left_extension, height * 0.4, 0.01, 0.005, 0.01);
  draw_XRI (ctx, fn_pe, length, 0.99, 1, - scale . left_extension, height * 0.4, 0.005, 0.001, 0.005);
  draw_XRI (ctx, fn_pe, length, 0.99, 1, - scale . left_extension, height * 0.2, 0.0005, 0.0001, 0.0005);
  draw_XRI (ctx, fn_pe, length, 0.99, 1, - scale . left_extension, height * 0.3, 0.001, 0.0005, 0.001);
};

var spacer = function (height, options) {
  this . height = height;
  this . ruleHeight = function () {return this . height;};
  this . hitTest = function (y) {return false;};
  this . la = 'left'; this . ra = 'right'; this . ca = 'center';
  this . ls = 0.1; this . rs = 0.1; this . cs = 0.5;
  this . colour = 'black'; this . alt = 'red';
  this . left_extension = 0; this . right_extension = 0;
  this . highlight_left = 0; this . highlight_right = 0;
  this . draw = function (ctx, length) {};
  this . value = function (location) {return null;};
  this . location = function (value) {return NaN;};
  this . sub_draw = function (ctx, length) {
    if (this . highlight) {ctx . fillStyle = this . highlight; ctx . fillRect (length * - this . highlight_left, 0, length * (1 + this . highlight_left + this . highlight_right), this . height);}
    ctx . fillStyle = this . marking_colour ? this . marking_colour : this . colour;
    ctx . strokeStyle = this . colour;
    ctx . font = (height * 0.5) + 'px arial';
    ctx . textAlign = 'center';
    ctx . save ();
    this . draw (ctx, length);
    ctx . restore ();
    if (this . font) ctx . font = this . font;
    var textBase = this . height * 0.75;
    if (this . baseline !== undefined) textBase = this . baseline;
    if (this . textBaseline !== undefined) ctx . textBaseline = this . textBaseline;
    if (this . left !== undefined) {ctx . textAlign = this . la; ctx . fillText (this . left, - length * this . ls, textBase);}
    if (this . right !== undefined) {ctx . textAlign = this . ra; ctx . fillText (this . right, length * (1 + this . rs), textBase);}
    if (this . centre !== undefined) {ctx . textAlign = this . ca; ctx . fillText (this . centre, length * this . cs, textBase);}
  };
  this . examine = function (position) {
    if (position . y < 0 || position . y > this . height) return null;
    var p = prompt ('Value for: ' + (this . left ? this . left : '') + ' ' + (this . right ? this . right : '') , '0.0000');
    if (p === null) return null;
    switch (p) {
      case 'pi': p = Math . PI; break;
      case 'pi1': p = Math . PI * 10; break;
      case 'e': p = Math . E; break;
      case 'e1': p = Math . E * 10; break;
      case 'c': p = Math . sqrt (4 / Math . PI); break;
      case 'c1': p = Math . sqrt (40 / Math . PI); break;
      default: p = Number (p); break;
    }
    p = this . location (p);
    if (isNaN (p)) return null;
    return p;
  };
  for (var key in options) this [key] = options [key];
};

var Rule = function (options) {
  this . stator = 0;
  this . left_margin = 0.2; this . right_margin = 0.2;
  this . shift = 0; this . target = 0; this . animation_delta = 0.004;
  this . rule_motion = 0.1;
  this . rounding = 8;
  this . scales = [];
  this . move = function (delta, length) {delta *= this . rule_motion; delta /= length; this . target += delta; return delta;};
  this . ruleHeight = function () {var h = 0; for (var ind in this . scales) h += this . scales [ind] . height; return h;};
  this . hitTest = function (y) {return this . stator != 0 && y >= 0 && y <= this . ruleHeight ();};
  this . draw = function (ctx, length) {
    if (this . target < this . shift) {
      this . shift -= this . animation_delta;
      if (this . shift < this . target) this . shift = this . target;
    }
    if (this . target > this . shift) {
      this . shift += this . animation_delta;
      if (this . shift > this . target) this . shift = this . target;
    }
    ctx . save ();
    ctx . fillStyle = 'white';
    ctx . lineWidth = 1;
    ctx . translate (length * (this . shift - this . left_margin), 0);
    ctx . beginPath ();
    	roundRect (ctx, 0, 0, (1 + this . left_margin + this . right_margin) * length, this . ruleHeight (), this . rounding);
    	ctx . fill (); ctx . stroke ();
    ctx . fillStyle = 'black';
    ctx . translate (this . left_margin * length, 0);
    var scale;
    for (var ind in this . scales) {
      scale = this . scales [ind];
      ctx . save ();
      scale . sub_draw (ctx, length);
      ctx . restore ();
      ctx . translate (0, this . scales [ind] . height);
    }
    ctx . restore ();
  };
  this . examine = function (position) {
    if (position . y < 0 || position . y > this . ruleHeight ()) return null;
    var value;
    for (var ind in this . scales) {
      value = this . scales [ind] . examine (position);
      if (value !== null) return value;
      position = subv (position, {x: 0, y: this . scales [ind] . height});
    }
    return null;
  };
  this . changed = function () {return this . target != this . shift;};
  for (var key in options) this [key] = options [key];
};

var Cursor = function (shift, from, to, colour) {
  this . draw = function (ctx, length) {
    length *= shift;
    ctx . save ();
    ctx . strokeStyle = colour;
    ctx . beginPath (); ctx . moveTo (length, from); ctx . lineTo (length, to); ctx . stroke ();
    ctx . restore ();
  };
};

var Sliderule = function (length, options) {
  this . length = length;
  this . left_margin = 0.2; this . right_margin = 0.2;
  this . position = {x: 0, y: 0};
  this . rules = [];
  this . animation_delta = 0.004;
  this . cursor_position = 0; this . cursor_target = 0; this . cursor_colour = 'red'; this . cursor_motion = 0.1;
  this . cursors = [];
  this . cursor_left_extension = 0.1; this . cursor_right_extension = 0.1;
  this . cursor_markings = true;
  this . cursor_rounding = 4;
  this . precision = 5;
  this . height = function () {var h = 0; for (var ind in this . rules) h += this . rules [ind] . ruleHeight (); return h;};
  this . moveCursor = function (delta) {delta *= this . cursor_motion; delta /= this . length; this . cursor_target += delta; return delta;};
  this . moveRule = function (delta, position) {
    if (position . y > this . height ()) return null;
    var y = position . y;
    for (var ind in this . rules) {
      if (this . rules [ind] . hitTest (y)) {return {rule: this . rules [ind], delta: this . rules [ind] . move (delta . x, this . length)};}
      y -= this . rules [ind] . ruleHeight ();
    }
    return {rule: this, delta: this . moveCursor (delta . x)};
  };
  this . draw = function (ctx) {
    if (this . cursor_target < this . cursor_position) {
      this . cursor_position -= this . animation_delta;
      if (this . cursor_position < this . cursor_target) this . cursor_position = this . cursor_target;
    }
    if (this . cursor_target > this . cursor_position) {
      this . cursor_position += this . animation_delta;
      if (this . cursor_position > this . cursor_target) this . cursor_position = this . cursor_target;
    }
    ctx . save ();
    ctx . translate (this . length * this . left_margin, 0);
    var ind;
    for (ind in this . rules) {
      this . rules [ind] . draw (ctx, this . length);
      ctx . translate (0, this . rules [ind] . ruleHeight ());
    }
    ctx . restore ();
    ctx . translate (this . length * (this . left_margin + this . cursor_position), 0);
    ctx . strokeStyle = this . cursor_colour;
    ctx . beginPath ();
    roundRect (ctx, - this . length * this . cursor_left_extension, - this . cursor_rounding,
      this . length * this . cursor_right_extension, this . height () + this . cursor_rounding, this . cursor_rounding);
    ctx . fillStyle = "rgba(0, 0, 0, 0.1)";
    ctx . fill (); ctx . stroke ();
    ctx . beginPath ();
    ctx . moveTo (0, -4); ctx . lineTo (0, this . height () + 4);
    ctx . stroke ();
    for (ind in this . cursors) {this . cursors [ind] . draw (ctx, this . length);}
    var y = 0;
    ctx . textBaseline = 'middle';
    ctx . font = '12px arial';
    var h; var hh; var description; var measure;
    for (ind in this . rules) {
      for (var sub in this . rules [ind] . scales) {
        h = this . rules [ind] . scales [sub] . height;
        hh = h * 0.5;
        description = this . rules [ind] . scales [sub] . value (this . cursor_position - this . rules [ind] . shift);
        if (description !== null) {
          description = description . toFixed (this . precision);
          ctx . fillStyle = 'white';
          measure = ctx . measureText (description);
          ctx . fillRect (4, y + hh - 8, measure . width + 8, 14);
          ctx . fillStyle = 'black';
          ctx . fillText (description, 8, y + hh);
        }
        y += h;
      }
    }
  };
  this . examine = function (position) {
    if (position . y < 0 || position . y > this .height ()) return false;
    position = subv (position, {x: this . length * this . left_margin, y: 0});
    var is_cursor = position . x / this . length - this . cursor_position;
    is_cursor = (is_cursor < this . cursor_right_extension && is_cursor > - this . cursor_left_extension);
    var value, target, delta;
    for (var ind in this . rules) {
      value = this . rules [ind] . examine (position);
      if (value !== null) {
        if (is_cursor || this . rules [ind] . stator == 0) {
          target = value + this . rules [ind] . shift;
          if (target < - this . left_margin) target = - this . left_margin;
          if (target > 1 + this . right_margin) target = 1 + this . right_margin;
          delta = target - this . cursor_target;
          this . cursor_target = target;
          return {rule: this, target: target, delta: delta, tick: delta * this . length};
        } else {
          target = - value + this . cursor_target;
          if (target < -1 - this . left_margin) target = -1 - this . left_margin;
          if (target > 1 + this . right_margin) target = 1 + this . right_margin;
          delta = target - this . rules [ind] . target;
          this . rules [ind] . target = target;
          return {rule: this . rules [ind], target: target, delta: delta, tick: delta * this . length};
        }
        return true;
      }
      position = subv (position, {x: 0, y: this . rules [ind] . ruleHeight ()});
    }
    return false;
  };
  this . changed = function () {
  	if (this . cursor_position != this . cursor_target) return true;
  	for (var ind in this . rules) {if (this . rules [ind] . changed ()) return true;}
  	return false;
  };
  for (var key in options) this [key] = options [key];
};

var Sliderules = function (options) {
  this . requireRedraw = true;
  this . position = {x: 32.5, y: 32.5};
  this . sliderules = [];
  this . synchronise = function (rule, delta) {
    if (! rule) return;
    var ind, sub, r;
    if (rule . stator) {
      for (ind in this . sliderules) {
        for (sub in this . sliderules [ind] . rules) {
          r = this . sliderules [ind] . rules [sub];
          if ((isNaN (r . stator) ? (r . stator == rule . stator) : (r . stator >= rule . stator)) && r != rule && ! r . noSync) {r . target += delta;}
        }
      }
    } else {
      for (ind in this . sliderules) {
        if (this . sliderules [ind] != rule && ! this . sliderules [ind] . noSync) {this . sliderules [ind] . cursor_target += delta;}
      }
    }
  };
  this . synchroniseTarget = function (delta) {
    var esc = this . examine (delta);
    if (esc) this . synchronise (esc . rule, esc . delta);
  };
  this . synchroniseMove = function (delta, position) {
    var esc = sliderules . move (delta, position);
    if (esc) sliderules . synchronise (esc . rule, esc . delta);
  };
  this . draw = function (ctx) {
  	this . requireRedraw = false;
    ctx . translate (this . position . x, this . position . y);
    for (var ind in this . sliderules) {
      ctx . translate (this . sliderules [ind] . position . x, this . sliderules [ind] . position . y);
      ctx . save ();
      this . sliderules [ind] . draw (ctx);
      ctx . restore ();
      ctx . translate (0, this . sliderules [ind] . height ());
    }
  };
  this . move = function (delta, position) {
    position = subv (position, this . position);
    var ret;
    for (var ind in this . sliderules) {
      position = subv (position, this . sliderules [ind] . position);
      ret = this . sliderules [ind] . moveRule (delta, position);
      if (ret) return ret;
      position = subv (position, {x: 0, y: this . sliderules [ind] . height ()});
      if (position . y < 0) return null;
    }
    return null;
  };
  this . examine = function (position) {
    position = subv (position, this . position);
    var esc;
    for (var ind in this . sliderules) {
      position = subv (position, this . sliderules [ind] . position);
      esc = this . sliderules [ind] . examine (position);
      if (esc) return esc;
      position = subv (position, {x: 0, y: this . sliderules [ind] . height ()});
    }
  };
  for (var key in options) this [key] = options [key];
  this . noChange = function () {
  	if (this . requireRedraw) return false;
  	for (var ind in this . sliderules) {if (this . sliderules [ind] . changed ()) return false;}
  	return true;
  };
};

