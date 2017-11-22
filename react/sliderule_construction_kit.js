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

var rndlist = function (list) {return list [Math . floor (Math . random () * list . length)];};
var crnu = function (number) {return Number (number . toFixed (14));};

var roundRect = function (ctx, left1, left2, top, right1, right2, bottom, radius) {
  var atan = Math . atan2 (left1 - left2, bottom - top - radius - radius);
  var btan = Math . atan2 (right1 - right2, bottom - top - radius - radius);
  ctx . arc (left1 + radius, top + radius, radius, Math . PI + atan, Math . PI * 1.5);
  ctx . arc (right1 - radius, top + radius, radius, Math . PI * 1.5, 0 + btan);
  ctx . arc (right2 - radius, bottom - radius, radius, btan, Math . PI * 0.5);
  ctx . arc (left2 + radius, bottom - radius, radius, Math . PI * 0.5, Math . PI + atan);
  ctx . closePath ();
};
var roundRectt = function (ctx, left1, left2, top, right1, right2, bottom, radius) {
  var step = Math . PI * 0.5; ctx . moveTo (left1 + radius, top); ctx . lineTo (right1 - radius, top); ctx . arc (right1 - radius, top + radius, radius, -step, 0);
  ctx . lineTo (right1, bottom - radius); ctx . arc (right1 - radius, bottom - radius, radius, 0, step); ctx . lineTo (left1 + radius, bottom);
  ctx . arc (left1 + radius, bottom - radius, radius, step, step + step); ctx . lineTo (left1, top + radius); ctx . arc (left1 + radius, top + radius, radius, step + step, - step);
};

var leftBrace = function (ctx, left, top, right, bottom, radius, braceRadius, angle, verticalShift) {
  var step = Math . PI * 0.5; ctx . moveTo (left + radius, top); ctx . lineTo (right - radius, top); ctx . arc (right - radius, top + radius, radius, -step, 0);
  ctx . lineTo (right, bottom - radius); ctx . arc (right - radius, bottom - radius, radius, 0, step); ctx . lineTo (left + radius, bottom);
  ctx . arc (left + radius, bottom - radius, radius, step, step + step);
  var half = (top + bottom) * 0.5;
  if (verticalShift != undefined) half += verticalShift;
  ctx . lineTo (left, half + braceRadius);
  ctx . arc (left - Math . cos (angle) * braceRadius, half, braceRadius, angle, - angle, true);
  ctx . lineTo (left, half - braceRadius);
  ctx . lineTo (left, top + radius); ctx . arc (left + radius, top + radius, radius, step + step, - step);
};

var rightBrace = function (ctx, left, top, right, bottom, radius, braceRadius, angle) {
  var step = Math . PI * 0.5; ctx . moveTo (left + radius, top); ctx . lineTo (right - radius, top); ctx . arc (right - radius, top + radius, radius, -step, 0);
  var half = (top + bottom) * 0.5;
  ctx . lineTo (right, half - braceRadius);
  ctx . arc (right + Math . cos (angle) * braceRadius, half, braceRadius, Math . PI + angle, Math . PI - angle, true);
  ctx . lineTo (right, bottom - radius);
  ctx . arc (right - radius, bottom - radius, radius, 0, step); ctx . lineTo (left + radius, bottom);
  ctx . arc (left + radius, bottom - radius, radius, step, step + step);
  ctx . lineTo (left, half - braceRadius);
  ctx . lineTo (left, top + radius); ctx . arc (left + radius, top + radius, radius, step + step, - step);
};

var doubleBrace = function (ctx, left, top, right, bottom, radius, braceRadius, angle) {
  var step = Math . PI * 0.5; ctx . moveTo (left + radius, top); ctx . lineTo (right - radius, top); ctx . arc (right - radius, top + radius, radius, -step, 0);
  var half = (top + bottom) * 0.5;
  ctx . lineTo (right, half - braceRadius);
  ctx . arc (right + Math . cos (angle) * braceRadius, half, braceRadius, Math . PI + angle, Math . PI - angle, true);
  ctx . lineTo (right, bottom - radius);
  ctx . arc (right - radius, bottom - radius, radius, 0, step); ctx . lineTo (left + radius, bottom);
  ctx . arc (left + radius, bottom - radius, radius, step, step + step);
  ctx . lineTo (left, half + braceRadius);
  ctx . arc (left - Math . cos (angle) * braceRadius, half, braceRadius, angle, - angle, true);
  ctx . lineTo (left, half - braceRadius);
  ctx . lineTo (left, top + radius); ctx . arc (left + radius, top + radius, radius, step + step, - step);
};

var addv = function (v1, v2) {return {x: v1 . x + v2 . x, y: v1 . y + v2 . y};};
var subv = function (v1, v2) {return {x: v1 . x - v2 . x, y: v1 . y - v2 . y};};
var subvbc = function (v1, v2) {return {x: v1 . x - v2 . left, y: v1 . y - v2 . top};};
var scalv = function (vector, scale) {return {x: vector . x * scale, y: vector . y * scale};};

var tick = function (ctx, x, height) {ctx . beginPath (); ctx . moveTo (x, 0); ctx . lineTo (x, - height); ctx . stroke ();};
var stick = function (ctx, x, base, height) {ctx . beginPath (); ctx . moveTo (x, - base); ctx . lineTo (x, - height); ctx . stroke ();};
var mark = function (ctx, ind, x, height) {tick (ctx, x, height); if (height < 0) ctx . fillText (ind, x, - height - height - 3); else ctx . fillText (ind, x, - height - 2);};
var mmark = function (ctx, ind, x, height) {if (height < 0) ctx . fillText (ind, x, - height - height - 3); else ctx . fillText (ind, x, - height - 2);};
var smark = function (ctx, ind, x, base, height) {
  stick (ctx, x, base, height);
  if (height < 0) ctx . fillText (ind, x, - height - height - 3);
  else ctx . fillText (ind, x, - height - 2);
};

var draw_XRI = function (ctx, fn, length, from, to, limit, height, step, micro_from, micro_to) {
  var esc = true; var location;
  to += 0.000000001;
  for (var ind = from; esc && ind > to; ind -= step) {
    for (var sub = micro_from; esc && sub < micro_to; sub += micro_from) {
      location = fn (ind - sub);
      if (location > limit) esc = false;
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
var draw_XLI = function (ctx, fn, length, to, from, limit, height, step, micro_from, micro_to) {
  var esc = true; var location;
  from += 0.0000001;
  for (var ind = from; esc && ind < to; ind += step) {
    for (var sub = micro_from; esc & sub < micro_to; sub += micro_from) {
      location = fn (ind + sub);
      if (location < limit) esc = false;
      else tick (ctx, length * location, height);
    }
  }
};
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
  to += 0.0000001;
  for (var ind = from; esc && ind <= to; ind += step) {
    location = fn (ind);
    if (location > limit) esc = false;
    else mark (ctx, Number (ind . toFixed (4)), length * location, height);
  }
};
var draw_MRSF = function (ctx, fn, length, from, to, step, fixed, limit, height) {
  var esc = true; var location;
  to += 0.0000001;
  for (var ind = from; esc && ind <= to; ind += step) {
    location = fn (ind);
    if (location > limit) esc = false;
    else mark (ctx, ind . toFixed (fixed), length * location, height);
  }
};
var draw_MRSI = function (ctx, fn, length, from, to, step, limit, height) {
  var esc = true; var location;
  to += 0.0000001;
  for (var ind = from; esc && ind >= to; ind -= step) {
    location = fn (ind);
    if (location > limit) esc = false;
    else mark (ctx, Number (ind . toFixed (4)), length * location, height);
  }
};
var draw_ML = function (ctx, fn, length, from, to, limit, height) {draw_MLS (ctx, fn, length, from, to, 1, limit, height);};
var draw_MLS = function (ctx, fn, length, from, to, step, limit, height) {
  var esc = true; var location;
  limit -= 0.0000001;
  from -= 0.0000001;
  for (var ind = to; esc && ind >= from; ind -= step) {
    location = fn (ind);
    if (location < limit) esc = false;
    else mark (ctx, Number (ind . toFixed (4)), length * location, height);
  }
};
var draw_MLSF = function (ctx, fn, length, from, to, step, fixed, limit, height) {
  var esc = true; var location;
  limit -= 0.0000001;
  from -= 0.0000001;
  for (var ind = to; esc && ind >= from; ind -= step) {
    location = fn (ind);
    if (location < limit) esc = false;
    else mark (ctx, ind . toFixed (fixed), length * location, height);
  }
};
var draw_MLSI = function (ctx, fn, length, to, from, step, limit, height) {
  var esc = true; var location;
  limit -= 0.0000001;
  from -= 0.0000001;
  for (var ind = from; esc && ind <= to; ind += step) {
    location = fn (ind);
    if (location < limit) esc = false;
    else mark (ctx, Number (ind . toFixed (4)), length * location, height);
  }
};

var draw_log_1R = function (ctx, length, height, extension, scale) {
  var location;
  var abs_length = Math . abs (length);
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MR (ctx, Math . log10, length, 2, 9, extension, h5);
  if (abs_length >= 800) draw_MRS (ctx, Math . log10, length, 1.1, 1.9, 0.1, extension, h5);
  if (scale . draw_pi) {location = Math . log10 (Math . PI); if (location < extension) smark (ctx, '\u03c0', length * location, h2, h5);}
  if (scale . draw_e) {location = Math . log10 (Math . E); if (location < extension) smark (ctx, 'e', length * location, h2, h5);}
  if (scale . draw_c) {
    location = Math . log10 (Math . sqrt (4 / Math . PI)); if (location < extension) smark (ctx, 'c', length * location, h2, h5);
    location = (0.5 + Math . log10 (Math . sqrt (4 / Math . PI)));
      if (location < extension) smark (ctx, 'c1', length * (0.5 + Math . log10 (Math . sqrt (4 / Math . PI))), h2, h5);
  }
  if (scale . draw_r) {location = Math . log10 (18 / Math . PI); if (location < extension) smark (ctx, 'R', length * location, h2, h5);}
  if (scale . draw_q) {location = Math . log10 (Math . PI / 1.8); if (location < extension) smark (ctx, 'q', length * location, h2, h5);}
  if (abs_length < 300) { // 2 4
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h4, 1, 0.5, 1);
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h3, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h2, 0.1, 0.05, 0.1);
    draw_XR (ctx, Math . log10, length, 2, 4, extension, h3, 1, 0.5, 1);
    draw_XR (ctx, Math . log10, length, 2, 4, extension, h2, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 4, 10, extension, h2, 1, 0.2, 1);
  } else if (abs_length < 400) { // 3 6
    draw_XR (ctx, Math . log10, length, 1, 3, extension, h4, 1, 0.5, 1);
    draw_XR (ctx, Math . log10, length, 1, 3, extension, h3, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 1, 3, extension, h2, 0.1, 0.05, 0.1);
    draw_XR (ctx, Math . log10, length, 3, 6, extension, h3, 1, 0.5, 1);
    draw_XR (ctx, Math . log10, length, 3, 6, extension, h2, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 6, 10, extension, h2, 1, 0.2, 1);
  } else if (abs_length < 800) { // 2 5
    if (scale . draw_halves) draw_MR (ctx, Math . log10, length, 1.5, 9.5, extension, h5);
    else {
      draw_XR (ctx, Math . log10, length, 1, 5, extension, h4, 1, 0.5, 1);
      draw_XR (ctx, Math . log10, length, 5, 10, extension, h3, 1, 0.5, 1);
    }
    draw_XR (ctx, Math . log10, length, 1, 5, extension, h3, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h2, 0.1, 0.02, 0.1);
    draw_XR (ctx, Math . log10, length, 2, 5, extension, h2, 0.1, 0.05, 0.1);
    draw_XR (ctx, Math . log10, length, 5, 10, extension, h2, 0.5, 0.1, 0.5);
  } else if (abs_length < 1400) { // 2 4
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h3, 0.1, 0.05, 0.1);
    draw_XR (ctx, Math . log10, length, 1, 2, extension, h2, 0.05, 0.01, 0.05);
    if (scale . draw_halves) draw_MR (ctx, Math . log10, length, 2.5, 9.5, extension, h5);
    else draw_XR (ctx, Math . log10, length, 2, 10, extension, h4, 1, 0.5, 1);
    draw_XR (ctx, Math . log10, length, 2, 10, extension, h3, 0.5, 0.1, 0.5);
    draw_XR (ctx, Math . log10, length, 2, 4, extension, h2, 0.1, 0.02, 0.1);
    draw_XR (ctx, Math . log10, length, 4, 10, extension, h2, 0.1, 0.05, 0.1);
  } else {
  	draw_MR (ctx, Math . log10, length, 2.5, 4.5, extension, h5);
  	draw_XR (ctx, Math . log10, length, 1, 2, extension, h4, 0.1, 0.05, 0.1);
  	draw_XR (ctx, Math . log10, length, 1, 2, extension, h3, 0.05, 0.01, 0.05);
  	draw_XR (ctx, Math . log10, length, 1, 2, extension, h2, 0.01, 0.005, 0.01);
  	draw_XR (ctx, Math . log10, length, 2, 5, extension, h4, 0.5, 0.1, 0.5);
  	draw_XR (ctx, Math . log10, length, 2, 5, extension, h3, 0.1, 0.05, 0.1);
  	draw_XR (ctx, Math . log10, length, 2, 5, extension, h2, 0.05, 0.01, 0.05);
  	draw_XR (ctx, Math . log10, length, 5, 10, extension, h4, 1, 0.5, 1);
  	draw_XR (ctx, Math . log10, length, 5, 10, extension, h3, 0.5, 0.1, 0.5);
  	draw_XR (ctx, Math . log10, length, 5, 10, extension, h2, 0.1, 0.02, 0.1);
  }
  if (scale . draw_st_corrections) {
    var esc = true;
    var shift = 2 - Math . log10 (Math . PI / 1.8);
    for (var ind = 4; esc && ind <= 9; ind++) {
      var angle = ind * Math . PI / 180;
      location = shift + Math . log10 (Math . sin (angle));
      var sub_location = shift + Math . log10 (Math . tan (angle));
      if (location > extension) esc = false;
      else {stick (ctx, length * location, h2, h5); stick (ctx, length * sub_location, h2, h5);}
    }
  }
};
var draw_log_1L = function (ctx, length, height, extension, scale) {
  var location;
  var abs_length = Math . abs (length);
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_ML (ctx, Math . log10, length, 2, 9, extension, height * 0.5);
  if (abs_length >= 800) draw_MLS (ctx, Math . log10, length, 1.1, 1.9, 0.1, extension, h5);
  if (scale . draw_pi) {location = Math . log10 (Math . PI); if (location > extension) smark (ctx, '\u03c0', length * location, height * 0.2, height * 0.5);}
  if (scale . draw_e) {location = Math . log10 (Math . E); if (location > extension) smark (ctx, 'e', length * location, height * 0.2, height * 0.5);}
  if (scale . draw_c) {
    location = Math . log10 (Math . sqrt (4 / Math . PI)); if (location > extension) smark (ctx, 'c', length * location, height * 0.2, height * 0.5);
    location = (0.5 + Math . log10 (Math . sqrt (4 / Math . PI)));
      if (location > extension) smark (ctx, 'c1', length * (0.5 + Math . log10 (Math . sqrt (4 / Math . PI))), height * 0.2, height * 0.5);
  }
  if (scale . draw_r) {location = Math . log10 (18 / Math . PI); if (location > extension) smark (ctx, 'R', length * location, h2, h5);}
  if (scale . draw_q) {location = Math . log10 (Math . PI / 1.8); if (location > extension) smark (ctx, 'q', length * location, h2, h5);}
  if (abs_length < 300) {
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h4, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h3, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h2, 0.1, 0.05, 0.1);
    draw_XL (ctx, Math . log10, length, 2, 4, extension, h3, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 2, 4, extension, h2, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 4, 10, extension, h2, 1, 0.2, 1);
  } else if (abs_length < 400) {
    draw_XL (ctx, Math . log10, length, 1, 3, extension, h4, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 1, 3, extension, h3, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 1, 3, extension, h2, 0.1, 0.05, 0.1);
    draw_XL (ctx, Math . log10, length, 3, 6, extension, h3, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 3, 6, extension, h2, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 6, 10, extension, h2, 1, 0.2, 1);
  } else if (abs_length < 800) {
    draw_XL (ctx, Math . log10, length, 1, 5, extension, h4, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 1, 5, extension, h3, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h2, 0.1, 0.02, 0.1);
    draw_XL (ctx, Math . log10, length, 2, 5, extension, h2, 0.1, 0.05, 0.1);
    draw_XL (ctx, Math . log10, length, 5, 10, extension, h3, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 5, 10, extension, h2, 0.5, 0.1, 0.5);
  } else if (abs_length < 1400) {
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h3, 0.1, 0.05, 0.1);
    draw_XL (ctx, Math . log10, length, 1, 2, extension, h2, 0.05, 0.01, 0.05);
    draw_XL (ctx, Math . log10, length, 2, 10, extension, h4, 1, 0.5, 1);
    draw_XL (ctx, Math . log10, length, 2, 10, extension, h3, 0.5, 0.1, 0.5);
    draw_XL (ctx, Math . log10, length, 2, 4, extension, h2, 0.1, 0.02, 0.1);
    draw_XL (ctx, Math . log10, length, 4, 10, extension, h2, 0.1, 0.05, 0.1);
  } else {
  	draw_ML (ctx, Math . log10, length, 2.5, 4.5, extension, h5);
  	draw_XL (ctx, Math . log10, length, 1, 2, extension, h4, 0.1, 0.05, 0.1);
  	draw_XL (ctx, Math . log10, length, 1, 2, extension, h3, 0.05, 0.01, 0.05);
  	draw_XL (ctx, Math . log10, length, 1, 2, extension, h2, 0.01, 0.005, 0.01);
  	draw_XL (ctx, Math . log10, length, 2, 5, extension, h4, 0.5, 0.1, 0.5);
  	draw_XL (ctx, Math . log10, length, 2, 5, extension, h3, 0.1, 0.05, 0.1);
  	draw_XL (ctx, Math . log10, length, 2, 5, extension, h2, 0.05, 0.01, 0.05);
  	draw_XL (ctx, Math . log10, length, 5, 10, extension, h4, 1, 0.5, 1);
  	draw_XL (ctx, Math . log10, length, 5, 10, extension, h3, 0.5, 0.1, 0.5);
  	draw_XL (ctx, Math . log10, length, 5, 10, extension, h2, 0.1, 0.02, 0.1);
  }
};
var draw_log = function (ctx, length, height, scale, left_extension, right_extension) {
  mark (ctx, scale . indices [0], 0, height * 0.5);
  mark (ctx, scale . indices [1], length, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . fillStyle = scale . marking_alt ? scale . marking_alt : scale . alt; ctx . strokeStyle = scale . alt;
  ctx . translate (length, 0);
  draw_log_1R (ctx, length, height, right_extension, scale);
  ctx . translate (- length - length, 0);
  draw_log_1L (ctx, length, height, 1 - left_extension, scale);
};
var draw_log_log = function (ctx, length, height, scale, left_extension, right_extension) {
  length *= 0.5;
  mark (ctx, scale . indices [0], 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . translate (length, 0);
  mark (ctx, scale . indices [1], 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . translate (length, 0);
  mark (ctx, scale . indices [2], 0, height * 0.5);
  ctx . fillStyle = scale . marking_alt ? scale . marking_alt : scale . alt; ctx . strokeStyle = scale . alt;
  draw_log_1R (ctx, length, height, 2 * right_extension, scale);
  ctx . translate (-3 * length, 0);
  draw_log_1L (ctx, length, height, 1 - 2 * left_extension, scale);
};
var draw_log_log_log = function (ctx, length, height, scale, left_extension, right_extension) {
  length /= 3;
  mark (ctx, scale . indices [0], 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . translate (length, 0);
  mark (ctx, scale . indices [1], 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . translate (length, 0);
  mark (ctx, scale . indices [2], 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1, scale);
  ctx . translate (length, 0);
  mark (ctx, scale . indices [3], 0, height * 0.5);
  ctx . fillStyle = scale . marking_alt ? scale . marking_alt : scale . alt; ctx . strokeStyle = scale . alt;
  draw_log_1R (ctx, length, height, 3 * right_extension, scale);
  ctx . translate (-4 * length, 0);
  draw_log_1L (ctx, length, height, 1 - 3 * left_extension, scale);
};
var draw_metric = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var limit = 1 + scale . right_extension;
  var high_value = scale . step * 100;
  draw_MRS (ctx, scale . location, length, 0, high_value, scale . step, limit, h5);
  if (length * (scale . location (scale . step) - scale . location (0)) > 55) {
		draw_XR (ctx, scale . location, length, 0, high_value, limit, h4, scale . step, scale . step * 0.5, scale . step);
		draw_XR (ctx, scale . location, length, 0, high_value, limit, h3, scale . step * 0.5, scale . step * 0.1, scale . step * 0.5);
		draw_XR (ctx, scale . location, length, 0, high_value, limit, h2, scale . step * 0.1, scale . step * 0.05, scale . step * 0.1);
  } else {
		draw_XR (ctx, scale . location, length, 0, high_value, limit, h3, scale . step, scale . step * 0.5, scale . step);
		draw_XR (ctx, scale . location, length, 0, high_value, limit, h2, scale . step * 0.5, scale . step * 0.1, scale . step * 0.5);
  }
};
var fn_lin = function (value) {return value * 0.1;};
var draw_lin = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var limit = 1 + scale . right_extension;
  draw_MR (ctx, fn_lin, length, 0, 20, limit, h5);
  draw_05R (ctx, fn_lin, length, 0, 20, limit, h4);
  draw_01R (ctx, fn_lin, length, 0, 20, limit, h3);
  if (length < 1000) draw_005R (ctx, fn_lin, length, 0, 20, limit, h2); else draw_002R (ctx, fn_lin, length, 0, 20, limit, h2);
  ctx . translate (- length, 0);
  limit = 1 - scale . left_extension;
  draw_ML (ctx, fn_lin, length, 0, 9, limit, h5);
  draw_05L (ctx, fn_lin, length, 0, 10, limit, h4);
  draw_01L (ctx, fn_lin, length, 0, 10, limit, h3);
  if (length < 1000) draw_005L (ctx, fn_lin, length, 0, 10, limit, h2); else draw_002L (ctx, fn_lin, length, 0, 10, limit, h2);
};
var fn_lin_1 = function (value) {return value * 0.2;};
var draw_lin_12 = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var limit = 1 + scale . right_extension;
  mark (ctx, ".0 .5", 0, h5);
  mark (ctx, ".1 .6", length * fn_lin_1 (1), h5);
  mark (ctx, ".2 .7", length * fn_lin_1 (2), h5);
  mark (ctx, ".3 .8", length * fn_lin_1 (3), h5);
  mark (ctx, ".4 .9", length * fn_lin_1 (4), h5);
  mark (ctx, ".5 1.", length, h5);
  draw_XR (ctx, fn_lin_1, length, 0, 10, limit, h5, 1, 0.5, 1);
  draw_XR (ctx, fn_lin_1, length, 0, 10, limit, h4, 0.5, 0.1, 0.5);
  draw_XR (ctx, fn_lin_1, length, 0, 10, limit, h3, 0.1, 0.05, 0.1);
  draw_XR (ctx, fn_lin_1, length, 0, 10, limit, h2, 0.05, 0.01, 0.05);
  limit = - scale . left_extension;
  draw_XL (ctx, fn_lin_1, length, -10, 0, limit, h5, 1, 0.5, 1);
  draw_XL (ctx, fn_lin_1, length, -10, 0, limit, h4, 0.5, 0.1, 0.5);
  draw_XL (ctx, fn_lin_1, length, -10, 0, limit, h3, 0.1, 0.05, 0.1);
  draw_XL (ctx, fn_lin_1, length, -10, 0, limit, h2, 0.05, 0.01, 0.05);
};

var fn_sin_dec = function (value) {return Math . log10 (10 * Math . sin (value * Math . PI / 180));};
var draw_sine_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var grad = scale . location (100) > scale . location (90);
  draw_MLS (ctx, scale . location, length, 4, 9, 1, - scale . left_extension, h5);
  draw_MLS (ctx, scale . location, length, 10, 19, 1, - scale . left_extension, h5);
  draw_MRS (ctx, scale . location, length, 20, 35, 5, 1, h5);
  draw_MRS (ctx, scale . location, length, 40, grad ? 80 : 90, 10, 1, h5);
  draw_XR (ctx, scale . location, length, 60, 80, 1, h3, 10, 5, 10);
  draw_XR (ctx, scale . location, length, 60, 80, 1, h2, 5, 1, 5);
  draw_XR (ctx, scale . location, length, 40, 60, 1, h4, 10, 5, 10);
  draw_XR (ctx, scale . location, length, 20, 60, 1, h3, 5, 1, 5);
  draw_XR (ctx, scale . location, length, 40, 60, 1, h2, 1, 0.5, 1);
  draw_XR (ctx, scale . location, length, 20, 40, 1, h2, 1, 0.2, 1);
  draw_XL (ctx, scale . location, length, 3, 20, - scale . left_extension, h3, 1, 0.5, 1);
  draw_XL (ctx, scale . location, length, 3, 20, - scale . left_extension, h2, 0.5, 0.1, 0.5);
  if (grad) {
  	mark (ctx, "100", length, h5);
  	draw_XR (ctx, scale . location, length, 80, 90, 1, h2, 10, 2, 10);
  	tick (ctx, scale . location (90) * length, h3);
  } else tick (ctx, scale . location (85) * length, h3);
};
var draw_sine_deg = function (ctx, length, height, scale) {
  draw_MRS (ctx, fn_sin_dec, length, 20, 35, 5, 1, height * 0.5);
  draw_MRS (ctx, fn_sin_dec, length, 40, 90, 10, 1, height * 0.5);
  draw_MLS (ctx, fn_sin_dec, length, 4, 15, 1, - scale . left_extension, height * 0.5);
  draw_10R (ctx, fn_sin_dec, length, 40, 80, 1, height * 0.2);
  draw_10R (ctx, fn_sin_dec, length, 15, 40, 1, height * 0.3);
  draw_XR (ctx, fn_sin_dec, length, 15, 40, 1, height * 0.2, 1, 1 / 3, 1);
  draw_10R (ctx, fn_sin_dec, length, 10, 15, 1, height * 0.5);
  draw_50R (ctx, fn_sin_dec, length, 40, 90, 1, height * 0.5);
  draw_05L (ctx, fn_sin_dec, length, 3, 15, - scale . left_extension, height * 0.3);
  draw_XL (ctx, fn_sin_dec, length, 3, 15, - scale . left_extension, height * 0.2, 0.5, 1 / 6, 0.5);
};
var draw_sine_cosine_dec = function (ctx, length, height, scale, type) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MLS (ctx, fn_sin_dec, length, 4, 9, 1, - scale . left_extension, h5);
  draw_MLS (ctx, fn_sin_dec, length, 11, 19, 1, - scale . left_extension, h5);
  draw_MRS (ctx, fn_sin_dec, length, 80, 90, 10, 1, h5);
  tick (ctx, length * fn_sin_dec (85), h3);
  draw_MRS (ctx, fn_sin_dec, length, 25, 35, 10, 1, h5);
  for (var degree = 10; degree <= 70; degree += 10) tick (ctx, length * fn_sin_dec (degree), h5);
  ctx . textAlign = type == 'sc' ? 'right' : 'left';
  var shifter = type == 'sc' ? -1 : 1;
  for (var degree = 10; degree <= 70; degree += 10) mmark (ctx, degree, length * fn_sin_dec (degree) + shifter, h5);
  ctx . textAlign = type == 'cs' ? 'right' : 'left';
  shifter = - shifter;
  ctx . fillStyle = scale . alt;
  for (var degree = 20; degree <= 80; degree += 10) mmark (ctx, degree, length * fn_sin_dec (90 - degree) + shifter, h5);
  draw_XR (ctx, fn_sin_dec, length, 60, 80, 1, h3, 10, 5, 10);
  draw_XR (ctx, fn_sin_dec, length, 60, 80, 1, h2, 5, 1, 5);
  draw_XR (ctx, fn_sin_dec, length, 20, 60, 1, h4, 10, 5, 10);
  draw_XR (ctx, fn_sin_dec, length, 20, 60, 1, h3, 5, 1, 5);
  draw_XR (ctx, fn_sin_dec, length, 40, 60, 1, h2, 1, 0.5, 1);
  draw_XR (ctx, fn_sin_dec, length, 20, 40, 1, h2, 1, 0.2, 1);
  draw_XL (ctx, fn_sin_dec, length, 3, 20, - scale . left_extension, h3, 1, 0.5, 1);
  draw_XL (ctx, fn_sin_dec, length, 3, 20, - scale . left_extension, h2, 0.5, 0.1, 0.5);
};
var draw_small_sine_dec = function (ctx, length, height, s) {
  var shift = s . location (1);
  ctx . translate (length * shift, 0);
  mark (ctx, "1", 0, height * 0.5);
  draw_log_1R (ctx, length, height, 1 + s . right_extension - shift, s);
  ctx . translate (- length, 0);
  draw_log_1L (ctx, length, height, 1 - shift - s . left_extension, s);
};
var draw_small_sine_deg = function (ctx, length, height, s) {
  var shift = s . location (1);
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  ctx . translate (length * shift, 0);
  var limit = 1 + s . right_extension - shift;
  draw_MR (ctx, Math . log10, length, 1, 10, limit, h5);
  var ind = 1;
  var location = Math . log10 (ind + 0.5);
  while (location < limit) {mark (ctx, ind + "\u00b3\u2070", length * location, h5); ind += 1; location = Math . log10 (ind + 0.5);}
  draw_XR (ctx, Math . log10, length, 3, 10, limit, h3, 0.5, 1 / 4, 0.5);
  draw_XR (ctx, Math . log10, length, 3, 10, limit, h2, 0.25, 1 / 12, 0.25);
  limit = - s . left_extension - shift;
  draw_XL (ctx, Math . log10, length, 0, 3, limit, h3, 1 / 6, 1 / 12, 1 / 6);
  draw_XL (ctx, Math . log10, length, 0, 3, limit, h2, 1 / 12, 1 / 60, 1 / 12);
  draw_XL (ctx, Math . log10, length, 1, 3, limit, h4, 0.5, 1 / 6, 0.5);
  location = Math . log10 (5 / 6); if (location >= limit) mark (ctx, "50\u00b0", length * location, h5);
  location = Math . log10 (4 / 6); if (location >= limit) mark (ctx, "40\u00b0", length * location, h5);
  location = Math . log10 (0.5); if (location >= limit) mark (ctx, "30\u00b0", length * location, h5);
  location = Math . log10 (2 / 6); if (location >= limit) mark (ctx, "20\u00b0", length * location, h5);
  location = Math . log10 (1 / 6); if (location >= limit) mark (ctx, "10\u00b0", length * location, h5);
};
var fn_tan_dec = function (value) {return 1 + Math . log10 (Math . tan (value * Math . PI / 180));};
var draw_tan_dec = function (ctx, length, height, scale) {
  draw_MLS (ctx, scale . location, length, 1, 6, 0.5, - scale . left_extension, height * 0.5);
  draw_MR (ctx, scale . location, length, 7, 10, 1, height * 0.5);
  draw_MRS (ctx, scale . location, length, 15, 90, 5, 1 + scale . right_extension, height * 0.5);
  draw_XR (ctx, scale . location, length, 10, 90, 1 + scale . right_extension, height * 0.4, 5, 1, 5);
  draw_XR (ctx, scale . location, length, 10, 90, 1 + scale . right_extension, height * 0.2, 1, 0.2, 1);
  draw_XL (ctx, scale . location, length, 6, 10, - scale . left_extension, height * 0.4, 1, 0.5, 1);
  draw_XL (ctx, scale . location, length, 1, 10, - scale . left_extension, height * 0.2, 0.5, 0.1, 0.5);
};
var draw_tan_deg = function (ctx, length, height, scale) {
  draw_MLS (ctx, fn_tan_dec, length, 1, 5.5, 0.5, - scale . left_extension, height * 0.5);
  draw_MR (ctx, fn_tan_dec, length, 6, 10, 1, height * 0.5);
  draw_MRS (ctx, fn_tan_dec, length, 12, 18, 2, 1, height * 0.5);
  draw_XR (ctx, fn_tan_dec, length, 10, 20, 1, height * 0.4, 2, 1, 2);
  draw_XR (ctx, fn_tan_dec, length, 10, 20, 1, height * 0.3, 1, 0.5, 1);
  draw_MRS (ctx, fn_tan_dec, length, 20, 90, 5, 1 + scale . right_extension, height * 0.5);
  draw_XR (ctx, fn_tan_dec, length, 20, 90, 1 + scale . right_extension, height * 0.3, 5, 1, 5);
  draw_XR (ctx, fn_tan_dec, length, 20, 90, 1 + scale . right_extension, height * 0.2, 1, 1 / 3, 1);
  draw_XL (ctx, fn_tan_dec, length, 6, 10, - scale . left_extension, height * 0.3, 1, 0.5, 1);
  draw_XL (ctx, fn_tan_dec, length, 1, 20, - scale . left_extension, height * 0.2, 0.5, 1 / 6, 0.5);
};
var draw_tan_cotan_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MLS (ctx, fn_tan_dec, length, 1, 5.5, 0.5, - scale . left_extension, h5);
  draw_MR (ctx, fn_tan_dec, length, 6, 9, 1, h5);
  if (length < 1000) {
    draw_MRS (ctx, fn_tan_dec, length, 15, 90, 10, 1 + scale . right_extension, h5);
    draw_XR (ctx, fn_tan_dec, length, 10, 90, 1 + scale . right_extension, h3, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 10, 90, 1 + scale . right_extension, h2, 1, 0.2, 1);
    draw_XL (ctx, fn_tan_dec, length, 6, 10, - scale . left_extension, h3, 1, 0.5, 1);
    draw_XL (ctx, fn_tan_dec, length, 1, 10, - scale . left_extension, h2, 0.5, 0.1, 0.5);
  } else {
    draw_MRS (ctx, fn_tan_dec, length, 25, 90, 10, 1 + scale . right_extension, h5);
    draw_MR (ctx, fn_tan_dec, length, 11, 19, 1, h5);
    draw_XR (ctx, fn_tan_dec, length, 20, 30, 1, h4, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 6, 30, 1, h3, 1, 0.5, 1);
    draw_XL (ctx, fn_tan_dec, length, 1, 30, - scale . left_extension, h2, 0.5, 0.1, 0.5);
    draw_XR (ctx, fn_tan_dec, length, 30, 90, 1 + scale . right_extension, h3, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 30, 90, 1 + scale . right_extension, h2, 1, 0.2, 1);
  }
  var esc = true;
  for (var ind = 10; ind <= 90; ind += 10) {
    var location = fn_tan_dec (ind);
    if (location > 1 + scale . right_extension) esc = false;
    else {
      location *= length;
      tick (ctx, length * fn_tan_dec (ind), height * 0.5);
      ctx . textAlign = 'right'; ctx . fillStyle = scale . colour; mmark (ctx, ind, location - 1, h5);
      ctx . textAlign = 'left'; ctx . fillStyle = scale . alt; mmark (ctx, 90 - ind, location + 1, h5);
    }
  }
};
var draw_cotan_tan_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MLS (ctx, fn_tan_dec, length, 1, 5.5, 0.5, - scale . left_extension, h5);
  draw_MR (ctx, fn_tan_dec, length, 6, 9, 1, h5);
  if (length < 1000) {
    draw_MRS (ctx, fn_tan_dec, length, 15, 90, 10, 1 + scale . right_extension, h5);
    draw_XR (ctx, fn_tan_dec, length, 10, 90, 1 + scale . right_extension, h3, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 10, 90, 1 + scale . right_extension, h2, 1, 0.2, 1);
    draw_XL (ctx, fn_tan_dec, length, 6, 10, - scale . left_extension, h3, 1, 0.5, 1);
    draw_XL (ctx, fn_tan_dec, length, 1, 10, - scale . left_extension, h2, 0.5, 0.1, 0.5);
  } else {
    draw_MRS (ctx, fn_tan_dec, length, 25, 90, 10, 1 + scale . right_extension, h5);
    draw_MR (ctx, fn_tan_dec, length, 11, 19, 1, h5);
    draw_XR (ctx, fn_tan_dec, length, 20, 30, 1, h4, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 6, 30, 1, h3, 1, 0.5, 1);
    draw_XL (ctx, fn_tan_dec, length, 1, 30, - scale . left_extension, h2, 0.5, 0.1, 0.5);
    draw_XR (ctx, fn_tan_dec, length, 30, 90, 1 + scale . right_extension, h3, 5, 1, 5);
    draw_XR (ctx, fn_tan_dec, length, 30, 90, 1 + scale . right_extension, h2, 1, 0.2, 1);
  }
  var esc = true;
  for (var ind = 10; ind <= 90; ind += 10) {
    var location = fn_tan_dec (ind);
    if (location > 1 + scale . right_extension) esc = false;
    else {
      location *= length;
      tick (ctx, length * fn_tan_dec (ind), height * 0.5);
      ctx . textAlign = 'left'; ctx . fillStyle = scale . colour; mmark (ctx, ind, location + 1, h5);
      ctx . textAlign = 'right'; ctx . fillStyle = scale . alt; mmark (ctx, 90 - ind, location - 1, h5);
    }
  }
};
var fn_big_tan_dec = function (value) {return Math . log10 (Math . tan (value * Math . PI / 180));};
var draw_big_tan_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MRS (ctx, fn_big_tan_dec, length, 60, 75, 5, 1 + scale . right_extension, h5);
  draw_MR (ctx, fn_big_tan_dec, length, 80, 90, 1 + scale . right_extension, h5);
  draw_MLS (ctx, fn_big_tan_dec, length, 10, 55, 5, - scale . left_extension, h5);
  draw_XL (ctx, fn_big_tan_dec, length, 10, 80, - scale . left_extension, h4, 5, 1, 5);
  draw_01R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h2);
  draw_05R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h3);
  draw_02L (ctx, fn_big_tan_dec, length, 10, 60, - scale . left_extension, h2);
};
var draw_big_tan_deg = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var limit = 1 + scale . right_extension;
  draw_MLS (ctx, fn_big_tan_dec, length, 10, 75, 5, - scale . left_extension, h5);
  draw_MR (ctx, fn_big_tan_dec, length, 80, 90, limit, h5);
  draw_XR (ctx, fn_big_tan_dec, length, 70, 80, limit, h4, 5, 1, 5);
  draw_XR (ctx, fn_big_tan_dec, length, 70, 90, limit, h3, 1, 1 / 3, 1);
  draw_XR (ctx, fn_big_tan_dec, length, 70, 90, limit, h2, 1 / 3, 1 / 6, 1 / 3);
  draw_XL (ctx, fn_big_tan_dec, length, 10, 70, - scale . left_extension, h3, 5, 1, 5);
  draw_XL (ctx, fn_big_tan_dec, length, 10, 70, - scale . left_extension, h2, 1, 1 / 3, 1);
};

var draw_big_tan_cotan_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MRS (ctx, fn_big_tan_dec, length, 55, 75, 10, 1 + scale . right_extension, h5);
  draw_MR (ctx, fn_big_tan_dec, length, 81, 90, 1 + scale . right_extension, h5);
  draw_MLS (ctx, fn_big_tan_dec, length, 10, 45, 5, - scale . left_extension, h5);
  if (length < 1000) {
    draw_XL (ctx, fn_big_tan_dec, length, 10, 80, - scale . left_extension, h4, 5, 1, 5);
    draw_01R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h2);
    draw_05R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h3);
    draw_02L (ctx, fn_big_tan_dec, length, 10, 60, - scale . left_extension, h2);
  } else {
    draw_XR (ctx, fn_big_tan_dec, length, 45, 80, 1 + scale . right_extension, h4, 5, 1, 5);
    draw_XR (ctx, fn_big_tan_dec, length, 45, 90, 1 + scale . right_extension, h3, 1, 0.5, 1);
    draw_XR (ctx, fn_big_tan_dec, length, 45, 90, 1 + scale . right_extension, h2, 0.5, 0.1, 0.5);
    draw_XL (ctx, fn_big_tan_dec, length, 10, 45, - scale . left_extension, h3, 5, 1, 5);
    draw_XL (ctx, fn_big_tan_dec, length, 10, 45, - scale . left_extension, h2, 1, 0.2, 1);
  }
  for (var degree = 50; degree <= 80; degree += 10) {
    var location = length * fn_big_tan_dec (degree);
    tick (ctx, length * fn_big_tan_dec (degree), height * 0.5);
    ctx . textAlign = 'right'; ctx . fillStyle = scale . colour; mmark (ctx, degree, location - 1, height * 0.5);
    ctx . textAlign = 'left'; ctx . fillStyle = scale . alt; mmark (ctx, 90 - degree, location + 1, height * 0.5);
  }
};

var draw_big_cotan_tan_dec = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  draw_MRS (ctx, fn_big_tan_dec, length, 55, 75, 10, 1 + scale . right_extension, h5);
  draw_MR (ctx, fn_big_tan_dec, length, 81, 90, 1 + scale . right_extension, h5);
  draw_MLS (ctx, fn_big_tan_dec, length, 10, 45, 5, - scale . left_extension, h5);
  if (length < 1000) {
    draw_XL (ctx, fn_big_tan_dec, length, 10, 80, - scale . left_extension, h4, 5, 1, 5);
    draw_01R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h2);
    draw_05R (ctx, fn_big_tan_dec, length, 60, 90, 1 + scale . right_extension, h3);
    draw_02L (ctx, fn_big_tan_dec, length, 10, 60, - scale . left_extension, h2);
  } else {
    draw_XR (ctx, fn_big_tan_dec, length, 45, 80, 1 + scale . right_extension, h4, 5, 1, 5);
    draw_XR (ctx, fn_big_tan_dec, length, 45, 90, 1 + scale . right_extension, h3, 1, 0.5, 1);
    draw_XR (ctx, fn_big_tan_dec, length, 45, 90, 1 + scale . right_extension, h2, 0.5, 0.1, 0.5);
    draw_XL (ctx, fn_big_tan_dec, length, 10, 45, - scale . left_extension, h3, 5, 1, 5);
    draw_XL (ctx, fn_big_tan_dec, length, 10, 45, - scale . left_extension, h2, 1, 0.2, 1);
  }
  for (var degree = 50; degree <= 80; degree += 10) {
    var location = length * fn_big_tan_dec (degree);
    tick (ctx, length * fn_big_tan_dec (degree), height * 0.5);
    ctx . textAlign = 'left'; ctx . fillStyle = scale . colour; mmark (ctx, degree, location + 1, height * 0.5);
    ctx . textAlign = 'right'; ctx . fillStyle = scale . alt; mmark (ctx, 90 - degree, location - 1, height * 0.5);
  }
};

var fn_pe = function (value) {return Math . log10 (10 * Math . sqrt (1 - value * value));};
var draw_pe = function (ctx, length, height, scale) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
  var location = fn_pe (0.996); if (location > - scale . left_extension) mmark (ctx, ".996", length * location, h5);
  location = fn_pe (0.997); if (location > - scale . left_extension) mark (ctx, ".997", length * location, h5);
  mark (ctx, ".995", length * fn_pe (0.995), h5);
  mark (ctx, ".99", length * fn_pe (0.99), h5);
  mark (ctx, ".98", length * fn_pe (0.98), h5);
  mark (ctx, ".97", length * fn_pe (0.97), h5);
  mark (ctx, ".96", length * fn_pe (0.96), h5);
  mark (ctx, ".95", length * fn_pe (0.95), h5);
  mark (ctx, ".94", length * fn_pe (0.94), h5);
  mark (ctx, ".93", length * fn_pe (0.93), h5);
  mark (ctx, ".92", length * fn_pe (0.92), h5);
  mark (ctx, ".91", length * fn_pe (0.91), h5);
  mark (ctx, ".9", length * fn_pe (0.9), h5);
  mark (ctx, ".8", length * fn_pe (0.8), h5);
  mark (ctx, ".7", length * fn_pe (0.7), h5);
  mark (ctx, ".6", length * fn_pe (0.6), h5);
  mark (ctx, ".5", length * fn_pe (0.5), h5);
  mark (ctx, ".4", length * fn_pe (0.4), h5);
  mark (ctx, ".3", length * fn_pe (0.3), h5);
  mark (ctx, ".2", length * fn_pe (0.2), h5);
  mark (ctx, "", length * fn_pe (0.1), h5);
  mark (ctx, 0, length, h5);
  draw_XLI (ctx, fn_pe, length, 0.3, 0.1, - scale . left_extension, h2, 0.1, 0.05, 0.1);
  draw_XLI (ctx, fn_pe, length, 0.4, 0.3, - scale . left_extension, h2, 0.1, 0.02, 0.1);
  draw_XLI (ctx, fn_pe, length, 0.6, 0.4, - scale . left_extension, h3, 0.1, 0.05, 0.1);
  draw_XLI (ctx, fn_pe, length, 0.6, 0.4, - scale . left_extension, h2, 0.05, 0.01, 0.05);
  draw_XLI (ctx, fn_pe, length, 0.9, 0.6, - scale . left_extension, h4, 0.1, 0.05, 0.1);
  draw_XLI (ctx, fn_pe, length, 0.9, 0.6, - scale . left_extension, h3, 0.05, 0.01, 0.05);
  draw_XLI (ctx, fn_pe, length, 0.8, 0.6, - scale . left_extension, h2, 0.01, 0.005, 0.01);
  draw_XLI (ctx, fn_pe, length, 0.9, 0.8, - scale . left_extension, h2, 0.01, 0.002, 0.01);
  draw_XLI (ctx, fn_pe, length, 0.95, 0.9, - scale . left_extension, h2, 0.005, 0.001, 0.005);
  draw_XLI (ctx, fn_pe, length, 0.95, 0.9, - scale . left_extension, h4, 0.01, 0.005, 0.01);
  draw_XLI (ctx, fn_pe, length, 0.99, 0.95, - scale . left_extension, h4, 0.01, 0.005, 0.01);
  draw_XLI (ctx, fn_pe, length, 0.99, 0.95, - scale . left_extension, h3, 0.005, 0.001, 0.005);
  draw_XLI (ctx, fn_pe, length, 0.98, 0.95, - scale . left_extension, h2, 0.001, 0.0005, 0.001);
  draw_XLI (ctx, fn_pe, length, 0.99, 0.98, - scale . left_extension, h2, 0.001, 0.0002, 0.001);
  draw_XLI (ctx, fn_pe, length, 1, 0.99, - scale . left_extension, h4, 0.005, 0.001, 0.005);
  draw_XLI (ctx, fn_pe, length, 1, 0.99, - scale . left_extension, h2, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, fn_pe, length, 1, 0.99, - scale . left_extension, h3, 0.001, 0.0005, 0.001);
};

var fn_ph = function (x) {return 1 + Math . log10 (Math . sqrt (x * x - 1));};
var draw_ph = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	draw_MRS (ctx, s . location, length, 1.01, 1.05, 0.01, 1, h5);
	draw_MRS (ctx, s . location, length, 1.1, 1.2, 0.05, 1, h5);
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 1.3, 4, 0.1, limit, h5);
	draw_XR (ctx, s . location, length, 1.6, 4, limit, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 1.6, 4, limit, h2, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.2, 1.6, limit, h4, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 1.2, 1.6, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.2, 1.6, limit, h2, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h2, 0.01, 0.002, 0.01);
	draw_XR (ctx, s . location, length, 1.05, 1.1, limit, h4, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.05, 1.1, limit, h3, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.05, 1.1, limit, h2, 0.005, 0.001, 0.005);
	draw_XR (ctx, s . location, length, 1.01, 1.05, limit, h4, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.01, 1.05, limit, h3, 0.005, 0.001, 0.005);
	draw_XR (ctx, s . location, length, 1.02, 1.05, limit, h2, 0.001, 0.0005, 0.001);
	draw_XR (ctx, s . location, length, 1.01, 1.02, limit, h2, 0.001, 0.0002, 0.001);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 1.001, 1.005, 0.001, limit, h5);
	draw_XL (ctx, s . location, length, 1.005, 1.01, limit, h4, 0.005, 0.001, 0.005);
	draw_XL (ctx, s . location, length, 1.005, 1.01, limit, h3, 0.001, 0.0005, 0.001);
	draw_XL (ctx, s . location, length, 1.005, 1.01, limit, h2, 0.0005, 0.0001, 0.0005);
	draw_XL (ctx, s . location, length, 1, 1.005, limit, h4, 0.001, 0.0005, 0.001);
	draw_XL (ctx, s . location, length, 1, 1.005, limit, h3, 0.0005, 0.0001, 0.0005);
	draw_XL (ctx, s . location, length, 1, 1.005, limit, h2, 0.0001, 0.00005, 0.0001);
};

var draw_ph2 = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 2.5, 3.5, 0.5, limit, h5);
	draw_MR (ctx, s . location, length, 4, 10, limit, h5);
	draw_XR (ctx, s . location, length, 4, 10, limit, h4, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 4, 10, limit, h3, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 4, 10, limit, h2, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 2, 4, limit, h3, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 2, 4, limit, h2, 0.1, 0.02, 0.1);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 1.1, 2, 0.1, limit, h5);
	draw_XL (ctx, s . location, length, 1.1, 2, limit, h3, 0.1, 0.05, 0.1);
	draw_XL (ctx, s . location, length, 1.1, 2, limit, h2, 0.05, 0.01, 0.05);
};

var draw_LL3 = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var esc; var limit = 1 + s . right_extension;
	draw_ML (ctx, s . location, length, 1, 9, - s . left_extension, h5);
	draw_MRS (ctx, s . location, length, 10, 50, 10, 1, h5);
	mark (ctx, "10\u00b2", length * s . location (100), h5);
	mark (ctx, "2", length * s . location (200), h5);
	mark (ctx, "3", length * s . location (300), h5);
	mark (ctx, "4", length * s . location (400), h5);
	mark (ctx, "5", length * s . location (500), h5);
	mark (ctx, "10\u00b3", length * s . location (1000), h5);
	mark (ctx, "2", length * s . location (2000), h5);
	mark (ctx, "3", length * s . location (3000), h5);
	mark (ctx, "4", length * s . location (4000), h5);
	mark (ctx, "5", length * s . location (5000), h5);
	mark (ctx, "10\u2074", length * s . location (10000), h5);
	mark (ctx, "2", length * s . location (20000), h5);
	smark (ctx, "e", 0, h3, h5);
	esc = s . location (30000); if (esc <= limit) mark (ctx, "3", length * esc, h5);
	esc = s . location (40000); if (esc <= limit) mark (ctx, "4", length * esc, h5);
	esc = s . location (50000); if (esc <= limit) mark (ctx, "5", length * esc, h5);
	esc = s . location (100000); if (esc <= limit) mark (ctx, "10\u2075", length * esc, h5);
	esc = s . location (200000); if (esc <= limit) mark (ctx, "2", length * esc, h5);
	esc = s . location (300000); if (esc <= limit) mark (ctx, "3", length * esc, h5);
	esc = s . location (400000); if (esc <= limit) mark (ctx, "4", length * esc, h5);
	esc = s . location (500000); if (esc <= limit) mark (ctx, "5", length * esc, h5);
	esc = s . location (1000000); if (esc <= limit) mark (ctx, "10\u2076", length * esc, h5);
	esc = s . location (2000000); if (esc <= limit) mark (ctx, "2", length * esc, h5);
	esc = s . location (3000000); if (esc <= limit) mark (ctx, "3", length * esc, h5);
	esc = s . location (4000000); if (esc <= limit) mark (ctx, "4", length * esc, h5);
	esc = s . location (5000000); if (esc <= limit) mark (ctx, "5", length * esc, h5);
	esc = s . location (10000000); if (esc <= limit) mark (ctx, "10\u2077", length * esc, h5);
	draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h4, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h3, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h2, 0.1, 0.02, 0.1);
	draw_XR (ctx, s . location, length, 3, 6, limit, h4, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 3, 6, limit, h3, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 3, 6, limit, h2, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 6, 10, limit, h3, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 6, 10, limit, h2, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 10, 30, limit, h4, 10, 5, 10);
	draw_XR (ctx, s . location, length, 10, 30, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 10, 30, limit, h2, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 30, 50, limit, h3, 10, 5, 10);
	draw_XR (ctx, s . location, length, 30, 50, limit, h2, 5, 1, 5);
	draw_XR (ctx, s . location, length, 50, 100, limit, h3, 50, 10, 50);
	draw_XR (ctx, s . location, length, 50, 100, limit, h2, 10, 2, 10);
	draw_XR (ctx, s . location, length, 100, 400, limit, h3, 100, 50, 100);
	draw_XR (ctx, s . location, length, 100, 400, limit, h2, 50, 10, 50);
	draw_XR (ctx, s . location, length, 400, 500, limit, h2, 100, 20, 100);
	draw_XR (ctx, s . location, length, 500, 1000, limit, h3, 500, 100, 500);
	draw_XR (ctx, s . location, length, 500, 1000, limit, h2, 100, 50, 100);
	draw_XR (ctx, s . location, length, 1000, 3000, limit, h3, 1000, 500, 1000);
	draw_XR (ctx, s . location, length, 1000, 3000, limit, h2, 500, 100, 500);
	draw_XR (ctx, s . location, length, 3000, 5000, limit, h2, 1000, 200, 1000);
	draw_XR (ctx, s . location, length, 5000, 10000, limit, h3, 5000, 1000, 5000);
	draw_XR (ctx, s . location, length, 5000, 10000, limit, h2, 1000, 500, 1000);
	draw_XR (ctx, s . location, length, 10000, 20000, limit, h3, 10000, 5000, 10000);
	draw_XR (ctx, s . location, length, 10000, 20000, limit, h2, 5000, 1000, 5000);
	draw_XR (ctx, s . location, length, 20000, 50000, limit, h2, 10000, 2000, 10000);
	draw_XR (ctx, s . location, length, 50000, 100000, limit, h3, 50000, 10000, 50000);
	draw_XR (ctx, s . location, length, 50000, 100000, limit, h2, 10000, 5000, 10000);
	draw_XR (ctx, s . location, length, 100000, 200000, limit, h3, 100000, 50000, 100000);
	draw_XR (ctx, s . location, length, 100000, 200000, limit, h2, 50000, 10000, 50000);
	draw_XR (ctx, s . location, length, 200000, 500000, limit, h2, 100000, 20000, 100000);
	draw_XR (ctx, s . location, length, 500000, 1000000, limit, h3, 500000, 100000, 500000);
	draw_XR (ctx, s . location, length, 500000, 1000000, limit, h2, 100000, 50000, 100000);
	draw_XR (ctx, s . location, length, 1000000, 2000000, limit, h3, 1000000, 500000, 1000000);
	draw_XR (ctx, s . location, length, 1000000, 2000000, limit, h2, 500000, 100000, 500000);
	draw_XR (ctx, s . location, length, 2000000, 5000000, limit, h2, 1000000, 200000, 1000000);
	draw_XR (ctx, s . location, length, 5000000, 10000000, limit, h3, 5000000, 1000000, 5000000);
	draw_XR (ctx, s . location, length, 5000000, 10000000, limit, h2, 1000000, 500000, 1000000);
};

var draw_LL2 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
	draw_MRS (ctx, s . location, length, 2, 9, 0.5, limit, h5);
	draw_XR (ctx, s . location, length, 2, 2.5, limit, h4, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 1.5, 2.5, limit, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 1.8, 2.5, limit, h2, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.4, 1.8, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.4, 1.8, limit, h2, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.2, 1.4, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 1.2, 1.4, limit, h2, 0.01, 0.002, 0.01);
	draw_XR (ctx, s . location, length, 2.5, 9, limit, h3, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 2.5, 9, limit, h2, 0.1, 0.02, 0.1);
	draw_MRSF (ctx, s . location, length, 1.2, 1.45, 0.05, 2, limit, h5);
	draw_MRS (ctx, s . location, length, 1.5, 1.9, 0.1, limit, h5);
	smark (ctx, "e", length, h3, h5);
	limit = - s . left_extension;
	draw_MLSF (ctx, s . location, length, 1.002, 1.18, 0.02, 2, limit, h5);
	draw_XL (ctx, s . location, length, 1, 1.2, limit, h4, 0.02, 0.01, 0.02);
	draw_XL (ctx, s . location, length, 1, 1.2, limit, h3, 0.01, 0.005, 0.01);
	draw_XL (ctx, s . location, length, 1, 1.2, limit, h2, 0.005, 0.001, 0.005);
};

var draw_LL1 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
	draw_MRSF (ctx, s . location, length, 1.1, 1.2, 0.01, 2, limit, h5);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h3, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h2, 0.005, 0.001, 0.005);
	draw_MRSF (ctx, s . location, length, 1.02, 1.05, 0.005, 3, limit, h5);
	draw_MRS (ctx, s . location, length, 1.06, 1.09, 0.01, limit, h5);
	draw_XR (ctx, s . location, length, 1.02, 1.1, limit, h3, 0.005, 0.001, 0.005);
	draw_XR (ctx, s . location, length, 1.05, 1.1, limit, h4, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.05, 1.1, limit, h2, 0.001, 0.0005, 0.001);
	draw_XR (ctx, s . location, length, 1.02, 1.05, limit, h2, 0.001, 0.0002, 0.001);
	limit = - s . left_extension;
	draw_MLSF (ctx, s . location, length, 1.002, 1.018, 0.002, 3, limit, h5);
	draw_XL (ctx, s . location, length, 1, 1.02, limit, h4, 0.002, 0.001, 0.002);
	draw_XL (ctx, s . location, length, 1, 1.02, limit, h3, 0.001, 0.0005, 0.001);
	draw_XL (ctx, s . location, length, 1, 1.02, limit, h2, 0.0005, 0.0001, 0.0005);
};

var draw_LL0 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
	draw_MRSF (ctx, s . location, length, 1.001, 1.002, 0.001, 3, limit, h5);
	draw_XR (ctx, s . location, length, 1.01, 1.02, limit, h3, 0.001, 0.0005, 0.001);
	draw_XR (ctx, s . location, length, 1.01, 1.02, limit, h2, 0.0005, 0.0001, 0.0005);
	draw_MRSF (ctx, s . location, length, 1.0025, 1.005, 0.0005, 4, limit, h5);
	draw_MRS (ctx, s . location, length, 1.006, 1.1, 0.001, limit, h5);
	draw_XR (ctx, s . location, length, 1.002, 1.01, limit, h3, 0.0005, 0.0001, 0.0005);
	draw_XR (ctx, s . location, length, 1.005, 1.01, limit, h4, 0.001, 0.0005, 0.001);
	draw_XR (ctx, s . location, length, 1.005, 1.01, limit, h2, 0.0001, 0.00005, 0.0001);
	draw_XR (ctx, s . location, length, 1.002, 1.005, limit, h2, 0.0001, 0.00002, 0.0001);
	limit = - s . left_extension;
	draw_MLSF (ctx, s . location, length, 1.0012, 1.0018, 0.0002, 4, limit, h5);
  draw_XL (ctx, s . location, length, 1, 1.001, limit, h5, 0.001, 0.0001, 0.001);
	draw_XL (ctx, s . location, length, 1, 1.002, limit, h4, 0.0002, 0.0001, 0.0002);
	draw_XL (ctx, s . location, length, 1, 1.002, limit, h3, 0.0001, 0.00005, 0.0001);
	draw_XL (ctx, s . location, length, 1, 1.002, limit, h2, 0.00005, 0.00001, 0.00005);
};

var draw_log_ll0 = function (ctx, length, height, s) {
  draw_log (ctx, length, height, s, s . left_extension, s . right_extension);
  ctx . translate (length, 0);
  var h5 = height > 0 ? - height * 0.5 - 2 : - height - 3;
  var ind;
  var measures = [];
  measures . push (ctx . measureText (s . indices [0]) . width * 0.6);
  for (ind = 2; ind < 10; ind++) measures . push (ctx . measureText (ind) . width * 0.6);
  measures . push (ctx . measureText (s . indices [1]) . width * 0.6);
  ctx . font = "italic " + Math . floor (Math . abs (height) * 0.3) + "px arial";
  ctx . textAlign = 'right';
  for (ind = 0; ind < 10; ind++) ctx . fillText ('1.00', length * Math . log10 (ind + 1) - measures [ind], h5);
  ctx . textAlign = 'left';
  ctx . fillText ('01', length * Math . log10 (4) + measures [3], h5);
  ctx . fillText ('01', length * Math . log10 (5) + measures [4], h5);
  ctx . fillText ('02', length * Math . log10 (6) + measures [5], h5);
  ctx . fillText ('02', length * Math . log10 (7) + measures [6], h5);
  ctx . fillText ('03', length * Math . log10 (8) + measures [7], h5);
  ctx . fillText ('04', length * Math . log10 (9) + measures [8], h5);
  ctx . fillText ('05', length * Math . log10 (10) + measures [9], h5);
};

var draw_LL03 = function (ctx, length, height, s) {
	var limit = - s . left_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  smark (ctx, "1/e", 0, h3, h5);
	draw_MLSI (ctx, s . location, length, 0.9, 0.1, 0.05, limit, h5);
	draw_XLI (ctx, s . location, length, 0.9, 0.1, limit, h3, 0.05, 0.01, 0.05);
	draw_XLI (ctx, s . location, length, 0.9, 0.1, limit, h2, 0.01, 0.002, 0.01);
	draw_MLSI (ctx, s . location, length, 0.08, 0.02, 0.02, limit, h5);
	draw_XLI (ctx, s . location, length, 0.1, 0.02, limit, h4, 0.02, 0.01, 0.02);
	draw_XLI (ctx, s . location, length, 0.1, 0.01, limit, h3, 0.01, 0.005, 0.01);
	draw_XLI (ctx, s . location, length, 0.1, 0.01, limit, h2, 0.005, 0.001, 0.005);
  mark (ctx, "10\u207b\u00b2", length * s . location (0.01), h5);
  mark (ctx, "5", length * s . location (0.005), h5);
  mark (ctx, "2", length * s . location (0.002), h5);
  draw_XLI (ctx, s . location, length, 0.01, 0.005, limit, h3, 0.005, 0.001, 0.005);
  draw_XLI (ctx, s . location, length, 0.005, 0.002, limit, h3, 0.003, 0.001, 0.003);
  draw_XLI (ctx, s . location, length, 0.01, 0.001, limit, h2, 0.001, 0.0002, 0.001);
  mark (ctx, "10\u207b\u00b3", length * s . location (0.001), h5);
  mark (ctx, "5", length * s . location (0.0005), h5);
  mark (ctx, "2", length * s . location (0.0002), h5);
  draw_XLI (ctx, s . location, length, 0.001, 0.0005, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, s . location, length, 0.0005, 0.0002, limit, h3, 0.0003, 0.0001, 0.0003);
  draw_XLI (ctx, s . location, length, 0.001, 0.0005, limit, h2, 0.0001, 0.00005, 0.0001);
  draw_XLI (ctx, s . location, length, 0.0005, 0.0001, limit, h2, 0.0001, 0.00002, 0.0001);
  mark (ctx, "10\u207b\u2074", length * s . location (0.0001), h5);
  limit = 1 + s . right_extension;
  mark (ctx, "5", length * s . location (0.00005), h5);
  var location = s . location (0.00002); if (location < limit) mark (ctx, "2", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.0001, 0.00005, limit, h3, 0.00005, 0.00001, 0.00005);
  draw_XRI (ctx, s . location, length, 0.00005, 0.00002, limit, h3, 0.00003, 0.00001, 0.00003);
  draw_XRI (ctx, s . location, length, 0.0001, 0.00002, limit, h2, 0.00001, 0.000005, 0.00001);
  draw_XRI (ctx, s . location, length, 0.00002, 0.00001, limit, h2, 0.00001, 0.000002, 0.000009);
  location = s . location (0.00001); if (location < limit) mark (ctx, "10\u207b\u2075", length * location, h5);
  location = s . location (0.000005); if (location < limit) mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.00001, 0.000005, limit, h2, 0.000005, 0.000001, 0.000005);
  draw_XRI (ctx, s . location, length, 0.000005, 0.000002, limit, h3, 0.000003, 0.000001, 0.000003);
  draw_XRI (ctx, s . location, length, 0.000005, 0.000002, limit, h2, 0.000001, 0.0000005, 0.000001);
  draw_XRI (ctx, s . location, length, 0.000002, 0.000001, limit, h2, 0.000001, 0.0000002, 0.000001);
  location = s . location (0.000002); if (location < limit) mark (ctx, "2", length * location, h5);
  location = s . location (0.000001); if (location < limit) mark (ctx, "10\u207b\u2076", length * location, h5);
  location = s . location (0.0000005); if (location < limit) mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.000001, 0.0000005, limit, h2, 0.0000005, 0.0000001, 0.0000005);
  draw_XRI (ctx, s . location, length, 0.0000005, 0.0000002, limit, h3, 0.0000003, 0.0000001, 0.0000003);
  draw_XRI (ctx, s . location, length, 0.0000005, 0.0000002, limit, h2, 0.0000001, 0.00000005, 0.0000001);
  draw_XRI (ctx, s . location, length, 0.0000002, 0.0000001, limit, h2, 0.0000001, 0.00000002, 0.0000001);
  location = s . location (0.0000002); if (location < limit) mark (ctx, "2", length * location, h5);
  location = s . location (0.0000001); if (location < limit) mark (ctx, "10\u207b\u2077", length * location, h5);
};

var draw_LL02 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  smark (ctx, "1/e", length, h3, h5);
  draw_MRSI (ctx, s . location, length, 0.9, 0.1, 0.05, limit, h5);
  draw_XRI (ctx, s . location, length, 0.8, 0.1, limit, h3, 0.05, 0.01, 0.05);
  draw_XRI (ctx, s . location, length, 0.8, 0.1, limit, h2, 0.01, 0.002, 0.01);
  limit = - s . left_extension;
  draw_XLI (ctx, s . location, length, 0.9, 0.8, limit, h4, 0.05, 0.01, 0.05);
  draw_XLI (ctx, s . location, length, 0.9, 0.8, limit, h3, 0.01, 0.005, 0.01);
  draw_XLI (ctx, s . location, length, 0.9, 0.8, limit, h2, 0.005, 0.001, 0.005);
  draw_MLSI (ctx, s . location, length, 0.99, 0.91, 0.01, limit, h5);
  draw_XLI (ctx, s . location, length, 0.99, 0.9, limit, h4, 0.01, 0.005, 0.01);
  draw_XLI (ctx, s . location, length, 0.99, 0.9, limit, h3, 0.005, 0.001, 0.005);
  draw_XLI (ctx, s . location, length, 0.99, 0.9, limit, h2, 0.001, 0.0005, 0.001);
};

var draw_LL01 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  draw_MRSI (ctx, s . location, length, 0.96, 0.8, 0.01, limit, h5);
  draw_XRI (ctx, s . location, length, 0.9, 0.8, limit, h3, 0.01, 0.005, 0.01);
  draw_XRI (ctx, s . location, length, 0.9, 0.8, limit, h2, 0.005, 0.001, 0.005);
  draw_XRI (ctx, s . location, length, 0.95, 0.9, limit, h4, 0.01, 0.005, 0.01);
  draw_XRI (ctx, s . location, length, 0.95, 0.9, limit, h3, 0.005, 0.001, 0.005);
  draw_XRI (ctx, s . location, length, 0.95, 0.9, limit, h2, 0.001, 0.0005, 0.001);
  draw_XRI (ctx, s . location, length, 0.97, 0.95, limit, h4, 0.01, 0.005, 0.01);
  draw_XRI (ctx, s . location, length, 0.98, 0.95, limit, h3, 0.005, 0.001, 0.005);
  draw_XRI (ctx, s . location, length, 0.98, 0.95, limit, h2, 0.001, 0.0002, 0.001);
  limit = - s . left_extension;
  draw_MLSI (ctx, s . location, length, 0.99, 0.97, 0.005, limit, h5);
  draw_XLI (ctx, s . location, length, 0.99, 0.98, limit, h4, 0.005, 0.001, 0.005);
  draw_XLI (ctx, s . location, length, 0.99, 0.98, limit, h3, 0.001, 0.0005, 0.001);
  draw_XLI (ctx, s . location, length, 0.99, 0.98, limit, h2, 0.0005, 0.0001, 0.0005);
  draw_MLSI (ctx, s . location, length, 0.999, 0.991, 0.001, limit, h5);
  draw_XLI (ctx, s . location, length, 0.999, 0.99, limit, h4, 0.001, 0.0005, 0.001);
  draw_XLI (ctx, s . location, length, 0.999, 0.99, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, s . location, length, 0.999, 0.99, limit, h2, 0.0001, 0.00005, 0.0001);
};

var draw_LL00 = function (ctx, length, height, s) {
	var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  draw_MRSI (ctx, s . location, length, 0.999, 0.891, 0.001, limit, h5);
  mark (ctx, 0.9985, length * s . location (0.9985), h5);
  draw_MLSI (ctx, s . location, length, 0.9999, 0.9991, 0.0001, - s . left_extension, h5);
  draw_XRI (ctx, s . location, length, 0.99, 0.98, limit, h3, 0.001, 0.0005, 0.001);
  draw_XRI (ctx, s . location, length, 0.99, 0.98, limit, h2, 0.0005, 0.0001, 0.0005);
  draw_XRI (ctx, s . location, length, 0.998, 0.99, limit, h4, 0.001, 0.0005, 0.001);
  draw_XRI (ctx, s . location, length, 0.998, 0.99, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XRI (ctx, s . location, length, 0.996, 0.99, limit, h2, 0.0001, 0.00005, 0.0001);
  draw_XRI (ctx, s . location, length, 0.998, 0.996, limit, h2, 0.0001, 0.00002, 0.0001);
  draw_XLI (ctx, s . location, length, 0.999, 0.998, - s . left_extension, h4, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, s . location, length, 1, 0.998, - s . left_extension, h3, 0.0001, 0.00005, 0.0001);
  draw_XLI (ctx, s . location, length, 1, 0.998, - s . left_extension, h2, 0.00005, 0.00001, 0.00005);
};

var draw_sinh = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 10, 20, 2, limit, h5);
	draw_MRS (ctx, s . location, length, 25, 50, 5, limit, h5);
	draw_MRS (ctx, s . location, length, 60, 200, 10, limit, h5);
	draw_XR (ctx, s . location, length, 10, 20, limit, h4, 2, 1, 2);
	draw_XR (ctx, s . location, length, 10, 20, limit, h3, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 10, 20, limit, h2, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 20, 50, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 20, 50, limit, h2, 1, 0.2, 1);
	draw_XR (ctx, s . location, length, 50, 200, limit, h4, 10, 5, 10);
	draw_XR (ctx, s . location, length, 50, 200, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 50, 200, limit, h2, 1, 0.5, 1);
	limit = - s . left_extension;
	draw_ML (ctx, s . location, length, 0, 9, limit, h5);
	draw_XL (ctx, s . location, length, 0, 10, limit, h4, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 0, 10, limit, h3, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 0, 10, limit, h2, 0.1, 0.05, 0.1);
};

var draw_sinh2 = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 110, 300, 10, limit, h5);
	draw_XR (ctx, s . location, length, 100, 300, limit, h4, 10, 5, 10);
	draw_XR (ctx, s . location, length, 100, 300, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 100, 300, limit, h2, 1, 0.5, 1);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 60, 100, 10, limit, h5);
	draw_MLS (ctx, s . location, length, 25, 50, 5, limit, h5);
	draw_XL (ctx, s . location, length, 50, 100, limit, h4, 10, 5, 10);
	draw_XL (ctx, s . location, length, 50, 100, limit, h3, 5, 1, 5);
	draw_XL (ctx, s . location, length, 50, 100, limit, h2, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 20, 50, limit, h3, 5, 1, 5);
	draw_XL (ctx, s . location, length, 20, 50, limit, h2, 1, 0.2, 1);
};

var draw_sinh1_rad = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 0.5, 5, 0.1, limit, h5);
	draw_XR (ctx, s . location, length, 0.4, 5, limit, h4, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 0.4, 5, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 0.4, 5, limit, h2, 0.01, 0.005, 0.01);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 0, 0.4, 0.05, limit, h5);
	draw_XL (ctx, s . location, length, 0.2, 0.4, limit, h3, 0.05, 0.01, 0.05);
	draw_XL (ctx, s . location, length, 0.2, 0.4, limit, h2, 0.01, 0.002, 0.01);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h4, 0.05, 0.01, 0.05);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h3, 0.01, 0.005, 0.01);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h2, 0.005, 0.001, 0.005);
};

var draw_sinh2_rad = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 2, 5, 0.1, limit, h5);
	draw_XR (ctx, s . location, length, 2, 5, limit, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 2, 5, limit, h2, 0.05, 0.01, 0.05);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 0, 1.9, 0.1, limit, h5);
	draw_XL (ctx, s . location, length, 0, 2, limit, h3, 0.1, 0.05, 0.1);
	draw_XL (ctx, s . location, length, 0, 2, limit, h2, 0.05, 0.01, 0.05);
};

var draw_cosh = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 20, 300, 10, limit, h5);
	draw_XR (ctx, s . location, length, 50, 300, limit, h4, 10, 5, 10);
	draw_XR (ctx, s . location, length, 50, 300, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 50, 300, limit, h2, 1, 0.5, 1);
	mark (ctx, 0, 0, h5);
	tick (ctx, s . location (10) * length, h5);
	tick (ctx, s . location (5) * length, h2);
	draw_XR (ctx, s . location, length, 20, 50, limit, h3, 10, 5, 10);
	draw_XR (ctx, s . location, length, 20, 50, limit, h2, 5, 1, 5);
	draw_XR (ctx, s . location, length, 10, 20, limit, h2, 10, 2, 10);
};

var draw_cosh_rad = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	mark (ctx, 0, 0, h5);
	tick (ctx, s . location (0.1) * length, h4);
	tick (ctx, s . location (0.15) * length, h2);
	tick (ctx, s . location (0.2) * length, h4);
	tick (ctx, s . location (0.25) * length, h2);
	mark (ctx, 0.3, s . location (0.3) * length, h5);
	tick (ctx, s . location (0.4) * length, h4);
	mark (ctx, 0.5, s . location (0.5) * length, h5);
	draw_MRS (ctx, s . location, length, 1, 5, 0.1, limit, h5);
	draw_XR (ctx, s . location, length, 0.3, 0.5, limit, h2, 0.1, 0.02, 0.1);
	draw_XR (ctx, s . location, length, 0.5, 1, limit, h4, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 0.5, 5, limit, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 0.5, 5, limit, h2, 0.05, 0.01, 0.05);
};

var draw_tanh = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 10, 20, 2, limit, h5);
	draw_MRS (ctx, s . location, length, 25, 30, 5, limit, h5);
	draw_MRS (ctx, s . location, length, 40, 80, 10, limit, h5);
	mark (ctx, 100, s . location (100) * length, h5);
	mark (ctx, 200, s . location (200) * length, h5);
	draw_XR (ctx, s . location, length, 10, 20, limit, h4, 2, 1, 2);
	draw_XR (ctx, s . location, length, 10, 20, limit, h3, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 10, 20, limit, h2, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 20, 30, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 20, 30, limit, h2, 1, 0.2, 1);
	draw_XR (ctx, s . location, length, 30, 60, limit, h4, 10, 5, 10);
	draw_XR (ctx, s . location, length, 30, 60, limit, h3, 5, 1, 5);
	draw_XR (ctx, s . location, length, 30, 60, limit, h2, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 60, 80, limit, h3, 10, 5, 10);
	draw_XR (ctx, s . location, length, 60, 80, limit, h2, 5, 1, 5);
	draw_XR (ctx, s . location, length, 80, 100, limit, h3, 20, 10, 20);
	draw_XR (ctx, s . location, length, 80, 100, limit, h2, 10, 2, 10);
	tick (ctx, s . location (150) * length, h3);
	draw_XR (ctx, s . location, length, 100, 150, limit, h2, 50, 10, 50);
	tick (ctx, s . location (300) * length, h3);
	limit = - s . left_extension;
	draw_ML (ctx, s . location, length, 0, 9, limit, h5);
	draw_XL (ctx, s . location, length, 0, 10, limit, h4, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 0, 10, limit, h3, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 0, 10, limit, h2, 0.1, 0.05, 0.1);
};

var draw_tanh_rad = function (ctx, length, height, s) {
	var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = 1 + s . right_extension;
	draw_MRS (ctx, s . location, length, 0.5, 1, 0.1, limit, h5);
	mark (ctx, 1.5, s . location (1.5) * length, h5);
	mark (ctx, 2, s . location (2) * length, h5);
	tick (ctx, s . location (2.5) * length, h2);
	mark (ctx, 3, s . location (3) * length, h5);
	draw_XR (ctx, s . location, length, 1, 2, limit, h3, 0.5, 0.1, 0.5);
	draw_XR (ctx, s . location, length, 1.5, 2, limit, h2, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 1, 1.5, limit, h2, 0.1, 0.02, 0.1);
	draw_XR (ctx, s . location, length, 0.7, 1, limit, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 0.7, 1, limit, h2, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 0.4, 0.7, limit, h4, 0.1, 0.05, 0.1);
	draw_XR (ctx, s . location, length, 0.4, 0.7, limit, h3, 0.05, 0.01, 0.05);
	draw_XR (ctx, s . location, length, 0.4, 0.7, limit, h2, 0.01, 0.005, 0.01);
	limit = - s . left_extension;
	draw_MLS (ctx, s . location, length, 0, 0.4, 0.05, limit, h5);
	draw_XL (ctx, s . location, length, 0.2, 0.4, limit, h3, 0.05, 0.01, 0.05);
	draw_XL (ctx, s . location, length, 0.2, 0.4, limit, h2, 0.01, 0.002, 0.01);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h4, 0.05, 0.01, 0.05);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h3, 0.01, 0.005, 0.01);
	draw_XL (ctx, s . location, length, 0, 0.2, limit, h2, 0.005, 0.001, 0.005);
};

var spacer = function (height, options) {
  this . height = height;
  this . indices = ['1', '10', '100', '1000', '10000', '100000'];
  this . draw_c = true;
  this . draw_pi = true;
  this . draw_e = true;
  this . lines = [];
  this . ruleHeight = function () {return this . height;};
  this . hitTest = function (y) {return false;};
  this . la = 'left'; this . ra = 'left'; this . ca = 'center';
  this . ls = 0.1; this . rs = 0.1; this . cs = 0.5;
  this . colour = 'black'; this . alt = 'red';
  this . left_extension = 0; this . right_extension = 0;
  this . highlight_left = 0; this . highlight_right = 0;
  this . draw = function (ctx, length) {};
  this . display = function (location, precision) {
    var v = this . value (location);
    if (v == null) return null;
    return v . toFixed (precision);
  };
  this . value = function (location) {return null;};
  this . location = function (value) {return NaN;};
  this . sub_draw = function (ctx, length) {
    if (this . highlight) {ctx . fillStyle = this . highlight; ctx . fillRect (length * - this . highlight_left, 0, length * (1 + this . highlight_left + this . highlight_right), this . height);}
    ctx . fillStyle = this . marking_colour ? this . marking_colour : this . colour;
    ctx . strokeStyle = this . colour;
    ctx . font = (this . height * 0.5) + 'px arial';
    ctx . textAlign = 'center';
    ctx . save ();
    this . draw (ctx, length);
    ctx . restore ();
    for (var line in this . lines) {
    	ctx . beginPath ();
    	ctx . moveTo (this . lines [line] . left * length, this . height * this . lines [line] . height);
    	ctx . lineTo (this . lines [line] . right * length, this . height * this . lines [line] . height);
    	ctx . stroke ();
    }
    if (this . font) ctx . font = this . font;
    var textBase = this . height * 0.75;
    if (this . baseline !== undefined) textBase = this . baseline;
    if (this . textBaseline !== undefined) ctx . textBaseline = this . textBaseline;
    if (this . left !== undefined) {ctx . textAlign = this . la; ctx . fillText (this . left, - length * this . ls, textBase);}
    if (this . right !== undefined) {ctx . textAlign = this . ra; ctx . fillText (this . right, length * (1 + this . rs), textBase);}
    if (this . cc !== undefined) ctx . fillStyle = this . cc;
    if (this . cf !== undefined) ctx . font = this . cf;
    if (this . centre !== undefined) {ctx . textAlign = this . ca; ctx . fillText (this . centre, length * this . cs, textBase);}
  };
  this . read = function (position) {return null;};
  this . examine = function (position) {
    if (position . y < 0 || position . y > this . height) return null;
    var p = prompt ('Value for: ' + (this . left ? this . left : '') + ' ' + (this . right ? this . right : '') , '0.0000');
    if (p === null) return null;
    var re = this . read (p);
    if (re != null) return re;
    switch (p) {
      case 'pi': p = Math . PI; break;
      case 'pi0': p = Math . PI / 10; break;
      case 'pi1': p = Math . PI * 10; break;
      case 'e': p = Math . E; break;
      case 'e1': p = Math . E * 10; break;
      case 'c': p = Math . sqrt (4 / Math . PI); break;
      case 'c1': p = Math . sqrt (40 / Math . PI); break;
      case 'q': p = Math . PI / 1.8; break;
      case 'r': p = 18 / Math . PI; break;
      case 'r0': p = 1.8 / Math . PI; break;
      case 'r1': p = 180 / Math . PI; break;
      default:
        var pp = p . split (":");
        var divisor = 1;
        p = 0;
        for (var ind in pp) {p += Number (pp [ind]) / divisor; divisor *= 60;}
        break;
    }
    p = this . location (p);
    if (isNaN (p)) return null;
    return p;
  };
  for (var key in options) this [key] = options [key];
};

var RuleBars = function (shift, direction, top, bottom, bars, step, width, colour) {
  this . draw = function (ctx, rule) {
    var position = shift * rule . length;
    ctx . strokeStyle = colour;
    ctx . lineWidth = width;
    for (var ind = 0; ind < bars; ind++) {
      ctx . beginPath (); ctx . moveTo (position + direction * ind * step, top); ctx . lineTo (position + direction * ind * step, bottom); ctx . stroke ();
    }
  }
};

var track = [];

var Rule = function (options) {
  this . stator = 0;
  this . left_margin = 0.2; this . right_margin = 0.2;
  this . shift = 0; this . target = 0; this . animation_delta = 0.01;
  this . rule_motion = 0.5;
  this . rounding = 8;
  this . rule_colour = 'white';
  this . border_colour = 'black';
  this . scales = [];
  this . markings = [];
  this . backMarkings = [];
  this . move = function (delta, length) {
    delta *= this . rule_motion; delta /= length;
    this . target += delta;
    if (this . target > 1 + this . right_margin) this . target = 1 + this . right_margin;
    if (this . target < -1 - this . left_margin) this . target = -1 - this . left_margin;
    this . shift = this . target;
    return delta;
  };
  this . ruleHeight = function () {var h = 0; for (var ind in this . scales) h += this . scales [ind] . height; return h;};
  this . hitTest = function (y) {return this . stator != 0 && y >= 0 && y <= this . ruleHeight ();};
  this . animate = function () {
    if (this . target < this . shift) {
      this . shift -= this . animation_delta;
      if (this . shift < this . target) this . shift = this . target;
    }
    if (this . target > this . shift) {
      this . shift += this . animation_delta;
      if (this . shift > this . target) this . shift = this . target;
    }
  }
  this . draw = function (ctx, length, sliderule) {
    this . animate ();
    ctx . save ();
    ctx . translate (length * (this . shift - this . left_margin), 0);
    if (this . rule_colour) {
    	ctx . fillStyle = this . rule_colour;
		ctx . lineWidth = 1;
		ctx . beginPath ();
			roundRect (ctx, 0, length * (this . left_margin - this . alt_left_margin), 0, (1 + this . left_margin + this . right_margin) * length, (1 + this . left_margin + this . alt_right_margin) * length, this . ruleHeight (), this . rounding);
			ctx . fill (); ctx . strokeStyle = this . border_colour; ctx . stroke ();
    }
    ctx . translate (this . left_margin * length, 0);
    for (var bm in this . backMarkings) {ctx . save (); this . backMarkings [bm] . draw (ctx, sliderule); ctx . restore ();}
    var scale;
    for (var ind in this . scales) {
      scale = this . scales [ind];
      ctx . save ();
      scale . sub_draw (ctx, length);
      ctx . restore ();
      ctx . translate (0, this . scales [ind] . height);
    }
    ctx . restore ();
    for (var mark in this . markings) {ctx . save (); ctx . translate (this . shift * length, 0); this . markings [mark] . draw (ctx, sliderule); ctx . restore ();}
  };
  this . examine = function (position) {
    if (position . y < 0 || position . y > this . ruleHeight ()) return null;
    var value;
    for (var ind in this . scales) {
      var scale = this . scales [ind];
      value = scale . dimm ? null : this . scales [ind] . examine (position);
      if (value !== null) {
        track . push ({scale: this . scales [ind] . left, location: value, value: this . scales [ind] . value (value)});
        return value;
      }
      position = subv (position, {x: 0, y: this . scales [ind] . height});
    }
    return null;
  };
  this . changed = function () {return this . target != this . shift;};
  for (var key in options) this [key] = options [key];
  if (this . alt_left_margin == undefined) this . alt_left_margin = this . left_margin;
  if (this . alt_right_margin == undefined) this . alt_right_margin = this . right_margin;
};

var Cursor = function (shift, from, to, colour, options) {
  this . shift = shift;
  this . from = from;
  this . to = to;
  this . marking_shift = 0.01; this . marking_y_shift = 0; this . marking_align = 'left';
  this . draw = function (ctx, length, static_offset) {
    length *= this . shift - (this . static ? static_offset : 0);
    ctx . save ();
    ctx . strokeStyle = colour;
    ctx . beginPath (); ctx . moveTo (length, from); ctx . lineTo (length, to); ctx . stroke ();
    ctx . restore ();
  };
  for (var key in options) this [key] = options [key];
};

var HairlineD = Math . log10 (1 / Math . sqrt (Math . PI * 0.25));
var HairlineS = - HairlineD;
var HairlineHPElectrical = - Math . log10 (Math . sqrt (0.746));
var HairlineHPMechanical = - Math . log10 (Math . sqrt (0.74569987158227022));
var HairlineHPMetric = - Math . log10 (Math . sqrt (0.73549875));
var HairlineHP = - Math . log10 (Math . sqrt (0.736));
var HairlineHPUS = HairlineHPElectrical;
var HairlineHPEurope = HairlineHP;
var HairlineHPJapan = HairlineHP;
var HairlinePS = HairlineHP;
var Hairline360 = Math . log10 (3.60 / Math . PI);
var Hairline360S = Math . log10 (3.60 / Math . sqrt (10));
var Hairline360R = Hairline360 - Hairline360S - Hairline360S;

var CursorS = function (from, to, colour, options) {return new Cursor (- Math . log10 (1 / Math . sqrt (Math . PI * 0.25)), from, to, colour, options);};
var CursorD = function (from, to, colour, options) {return new Cursor (Math . log10 (1 / Math . sqrt (Math . PI * 0.25)), from, to, colour, options);};
var CursorHPElectrical = function (from, to, colour, options) {return new Cursor (- Math . log10 (Math . sqrt (0.746)), from, to, colour, options);};
var CursorHPMechanical = function (from, to, colour, options) {return new Cursor (- Math . log10 (Math . sqrt (0.74569987158227022)), from, to, colour, options);};
var CursorHPMetric = function (from, to, colour, options) {return new Cursor (- Math . log10 (Math . sqrt (0.73549875)), from, to, colour, options);};
var CursorHP = function (from, to, colour, options) {return new Cursor (- Math . log10 (Math . sqrt (0.736)), from, to, colour, options);};
var CursorHPUS = CursorHPElectrical;
var CursorHPEurope = CursorHP;
var CursorHPJapan = CursorHP;
var CursorPS = CursorHP;
var Cursor360 = function (from, to, colour, options) {return new Cursor (Math . log10 (3.60 / Math . PI), from, to, colour, options);};

var LeftBrace = function (margin, width, radius, background, colour, braceRadius, braceAngle, verticalShift) {
  this . draw = function (ctx, s) {
    ctx . beginPath ();
    leftBrace (ctx, margin, margin, width * s . length, s . height () - margin, 8, braceRadius, braceAngle, verticalShift);
    ctx . fillStyle = background;
    ctx . fill ();
    ctx . strokeStyle = colour;
    ctx . stroke ();
  };
};

var RightBrace = function (margin, width, radius, background, colour, braceRadius, braceAngle, verticalShift) {
  this . draw = function (ctx, s) {
    ctx . beginPath ();
    ctx . translate (s . length * (1 + s . left_margin + s . right_margin), 0);
    ctx . scale (-1, 1);
    leftBrace (ctx, margin, margin, width * s . length, s . height () - margin, 8, braceRadius, braceAngle, verticalShift);
    ctx . fillStyle = background;
    ctx . fill ();
    ctx . strokeStyle = colour;
    ctx . stroke ();
  };
};

var drawDecilonBrace = function (ctx, radius, one, two, three, four, margin, l1, l2, background, colour, s) {
	var delta = {x: s . length * (four - three), y: l2 - l1};
	var atan = Math . atan2 (delta . y, delta . x);
	var r = 0.5 * Math . sqrt (delta . x * delta . x + delta . y * delta . y);
	atan -= Math . asin (radius / r);
	delta = {x: s . length * (three - two), y: l1 - margin};
	var btan = Math . atan2 (delta . x, delta . y);
	r = 0.5 * Math . sqrt (delta . x * delta . x + delta . y * delta . y);
	btan -= Math . asin (radius / r);
	ctx . beginPath ();
	ctx . arc (- s . length * one, margin, radius, Math . PI * 1.5, Math . PI * 2);
	ctx . arc (- s . length * one, s . height () - margin, radius, 0, Math . PI * 0.5);
	ctx . arc (- s . length * four, s . height () - margin, radius, Math . PI * 0.5, Math . PI);
	ctx . arc (- s . length * four, l2, radius, Math . PI, Math . PI * 1.5 - atan);
	ctx . arc (- s . length * three, l1, radius, Math . PI * 0.5 - atan, btan, true);
	ctx . arc (- s . length * two, margin, radius, Math . PI + btan, Math . PI * 1.5);
	ctx . closePath ();
	ctx . strokeStyle = colour;
	ctx . stroke ();
	ctx . fillStyle = background;
	ctx . fill ();
};

var drawStaedtlerBrace = function (ctx, radius, width, dent, left1, left2, top, bottom, dbottom, background, colour, s) {
    ctx . strokeStyle = colour;
    var delta = {x: s . length * (left2 - dent), y: dbottom - bottom};
    var atan = Math . atan2 (s . length * (left2 - left1), bottom - top);
    var btan = Math . atan2 (delta . y, delta . x);
    var r = 0.5 * Math . sqrt (delta . x * delta . x + delta . y * delta . y);
    btan -= Math . asin (radius / r);
    ctx . beginPath ();
    ctx . arc (- s . length * width, top, radius, Math . PI * 1.5, 0);
    ctx . arc (- s . length * width, s . height () - top, radius, 0, Math . PI * 0.5);
    ctx . arc (- s . length * left1, s . height () - top, radius, Math . PI * 0.5, Math . PI - atan);
    ctx . arc (- s . length * left2, s . height () - bottom, radius, Math . PI - atan, Math . PI * 1.5 - btan);
    ctx . arc (- s . length * dent, s . height () - dbottom, radius, Math . PI * 0.5 - btan, 0, true);
    ctx . arc (- s . length * dent, dbottom, radius, 0, Math . PI * 1.5 + btan, true);
    ctx . arc (- s . length * left2, bottom, radius, Math . PI * 0.5 + btan, Math . PI + atan);
    ctx . arc (- s . length * left1, top, radius, Math . PI + atan, Math . PI * 1.5);
    ctx . closePath ();
    ctx . stroke ();
    ctx . fillStyle = background;
    ctx . fill ();
};

var StaedtlerLeftBrace = function (radius, width, dent, left1, left2, top, bottom, dbottom, background, colour) {
  this . draw = function (ctx, s) {
    ctx . translate (s . length * s . left_margin, 0);
    drawStaedtlerBrace (ctx, radius, width, dent, left1, left2, top, bottom, dbottom, background, colour, s);
  };
};

var StaedtlerRightBrace = function (radius, width, dent, left1, left2, top, bottom, dbottom, background, colour) {
  this . draw = function (ctx, s) {
    ctx . translate (s . length * (1 + s . left_margin), 0);
    ctx . scale (-1, 1);
    drawStaedtlerBrace (ctx, radius, width, dent, left1, left2, top, bottom, dbottom, background, colour, s);
  };
};

var DecilonTopLeftBrace = function (radius, one, two, three, four, margin, l1, l2, background, colour) {
	this . draw = function (ctx, s) {
		ctx . translate (s . length * s . left_margin, 0);
		drawDecilonBrace (ctx, radius, one, two, three, four, margin, l1, l2, background, colour, s);
	};
};

var DecilonTopRightBrace = function (radius, one, two, three, four, margin, l1, l2, background, colour) {
	this . draw = function (ctx, s) {
		ctx . translate (s . length * (1 + s . left_margin), 0);
		ctx . scale (-1, 1);
		drawDecilonBrace (ctx, radius, one, two, three, four, margin, l1, l2, background, colour, s);
	};
};

var DecilonBottomLeftBrace = function (radius, one, two, three, four, margin, l1, l2, background, colour) {
	this . draw = function (ctx, s) {
		ctx . translate (s . length * s . left_margin, s . height ());
		ctx . scale (1, -1);
		drawDecilonBrace (ctx, radius, one, two, three, four, margin, l1, l2, background, colour, s);
	};
};

var DecilonBottomRightBrace = function (radius, one, two, three, four, margin, l1, l2, background, colour) {
	this . draw = function (ctx, s) {
		ctx . translate (s . length * (1 + s . left_margin), s . height ());
		ctx . scale (-1, -1);
		drawDecilonBrace (ctx, radius, one, two, three, four, margin, l1, l2, background, colour, s);
	};
};

var drawLBrace = function (ctx, radius, r2, from, to, mid_point, margin, background, colour, s) {
	var ff = from * s . length, tt = to * s . length, hh = s . height ();
	ctx . beginPath ();
	ctx . arc (tt - radius - margin, radius + margin, radius, Math . PI * 1.5, Math . PI * 2);
	ctx . arc (tt - radius - margin, hh - radius - margin, radius, 0, Math . PI * 0.5);
	ctx . arc (ff + radius + margin, hh - radius - margin, radius, Math . PI * 0.5, Math . PI);
	ctx . arc (ff - r2 + margin, mid_point + r2 - margin, r2, 0, Math . PI * 1.5, true);
	ctx . arc (margin + radius, mid_point - radius - margin, radius, Math . PI * 0.5, Math . PI);
	ctx . arc (margin + radius, radius + margin, radius, Math . PI, Math . PI * 1.5);
	ctx . closePath ();
	ctx . fillStyle = background; ctx . fill ();
	ctx . strokeStyle = colour; ctx . stroke ();
};

var LTopLeftBrace = function (radius, r2, from, to, mid_point, margin, background, colour, s) {
  this . draw = function (ctx, s) {drawLBrace (ctx, radius, r2, from, to, mid_point, margin, background, colour, s);};
};
var LTopRightBrace = function (radius, r2, from, to, mid_point, margin, background, colour, s) {
  this . draw = function (ctx, s) {
  	ctx . translate (s . length * (1 + s . left_margin + s . right_margin), 0); ctx . scale (-1, 1);
  	drawLBrace (ctx, radius, r2, from, to, mid_point, margin, background, colour, s);};
};
var LBottomLeftBrace = function (radius, r2, from, to, mid_point, margin, background, colour, s) {
  this . draw = function (ctx, s) {
  	ctx . translate (0, s . height ()); ctx . scale (1, -1);
  	drawLBrace (ctx, radius, r2, from, to, mid_point, margin, background, colour, s);
  };
};

var LBottomRightBrace = function (radius, r2, from, to, mid_point, margin, background, colour, s) {
  this . draw = function (ctx, s) {
  	ctx . translate (s . length * (1 + s . left_margin + s . right_margin), s . height ()); ctx . scale (-1, -1);
  	drawLBrace (ctx, radius, r2, from, to, mid_point, margin, background, colour, s);};
};

var LeftBraceBar = function (location, top, bottom, radius, background, colour) {
  this . draw = function (ctx, s) {
    var position = location * s . length;
    var h = s . height ();
    ctx . beginPath ();
    ctx . moveTo (position + radius, top + radius);
    ctx . lineTo (position + radius, h - bottom - radius);
    ctx . arc (position, h - bottom - radius, radius, 0, Math . PI);
    ctx . lineTo (position - radius, top + radius);
    ctx . arc (position, top + radius, radius, Math . PI, 0);
    ctx . fillStyle = background;
    ctx . strokeStyle = colour;
    ctx . fill ();
    ctx . stroke ();
  };
};

var RightBraceBar = function (location, top, bottom, radius, background, colour) {
  this . draw = function (ctx, s) {
    var position = location * s . length;
    var h = s . height ();
    ctx . beginPath ();
    ctx . translate (s . length * (1 + s . left_margin + s . right_margin), 0);
    ctx . scale (-1, 1);
    ctx . moveTo (position + radius, top + radius);
    ctx . lineTo (position + radius, h - bottom - radius);
    ctx . arc (position, h - bottom - radius, radius, 0, Math . PI);
    ctx . lineTo (position - radius, top + radius);
    ctx . arc (position, top + radius, radius, Math . PI, 0);
    ctx . fillStyle = background;
    ctx . strokeStyle = colour;
    ctx . fill ();
    ctx . stroke ();
  };
};

var BraceSupport = function (location, top, bottom, width, radius, background, colour) {
  this . draw = function (ctx, s) {
    var position = location * s . length;
    var h = s . height ();
    var w = width * 0.5;
    ctx . beginPath ();
    ctx . arc (position + w - radius, bottom - radius, radius, 0, Math . PI * 0.5);
    ctx . arc (position - w + radius, bottom - radius, radius, Math . PI * 0.5, Math . PI);
    ctx . arc (position - w + radius, top + radius, radius, Math . PI, Math . PI * 1.5);
    ctx . arc (position + w - radius, top + radius, radius, Math . PI * 1.5, 0);
    ctx . closePath ();
    ctx . fillStyle = background;
    ctx . strokeStyle = colour;
    ctx . fill ();
    ctx . stroke ();
  };
};

var Logo = function (logo, location, top, scaling, rotation) {
	this . draw = function (ctx, s) {
		ctx . translate (location * s . length, top);
		if (scaling !== undefined) ctx . scale (scaling, scaling);
		if (rotation !== undefined) ctx . rotate (rotation);
		ctx . translate (logo . width * -0.5, logo . height * -0.5);
		ctx . drawImage (logo, 0, 0);
	};
};

var Engraving = function (text, font, align, colour, location, top, rotation) {
	this . draw = function (ctx, s) {
		ctx . fillStyle = colour;
		ctx . font = font;
    ctx . textAlign = align;
    ctx . translate (location * s . length, top);
    if (rotation !== undefined) ctx . rotate (rotation);
		ctx . fillText (text, 0, 0);
	};
};

var CursorBrace = function (left, right, top, bottom, radius, colour) {
	this . draw = function (ctx, s) {
		var l = - left * s . length, r = right * s . length;
		ctx . fillStyle = colour;
		ctx . beginPath ();
		ctx . moveTo (l, bottom); ctx . arc (l + radius, - top + radius, radius, Math . PI, Math . PI * 1.5);
		ctx . arc (r - radius, - top + radius, radius, Math . PI * 1.5, 0); ctx . lineTo (r, bottom);
		ctx . closePath ();
		ctx . fill ();
		ctx . translate (0, s . height ());
		ctx . beginPath ();
		ctx . moveTo (r, - bottom); ctx . arc (r - radius, top - radius, radius, 0, Math . PI * 0.5);
		ctx . arc (l + radius, top - radius, radius, Math . PI * 0.5, Math . PI); ctx . lineTo (l, bottom);
		ctx . closePath ();
		ctx . fill ();
	}
};

var CursorAngledBrace = function (left, right, top, bottom, angled, colour) {
  this . draw = function (ctx, s) {
    ctx . fillStyle = colour;
    ctx . beginPath ();
    var l = - left * s . length, r = right * s . length;
    ctx . moveTo (l, bottom); ctx . lineTo (l, 0); ctx . lineTo (l + angled, - top);
    ctx . lineTo (r - angled, - top);
    ctx . lineTo (r, 0); ctx . lineTo (r, bottom);
    ctx . fill ();
    ctx . beginPath ();
    ctx . translate (0, s . height ());
    ctx . moveTo (l, - bottom); ctx . lineTo (l, 0); ctx . lineTo (l + angled, top);
    ctx . lineTo (r - angled, top); ctx . lineTo (r, 0);
    ctx . lineTo (r, - bottom);
    ctx . fill ();
  };
};

var Floor = function (left, right, top, bottom, rounding, radius, holes, colour, background) {
	this . draw = function (ctx, s) {
		var le = s . length * (s . left_margin - left); var re = s . length * (s . left_margin + 1 + right);
		var bt = s . height () - bottom;
		var mid = (top + bt) * 0.5;
		ctx . beginPath ();
		ctx . arc (le + rounding, top + rounding, rounding, Math . PI, Math . PI * 1.5);
		ctx . arc (re - rounding, top + rounding, rounding, Math . PI * 1.5, 0);
		ctx . arc (re - rounding, mid - radius - rounding, rounding, 0, Math . PI * 0.5);
		ctx . arc (re - rounding, mid, radius, Math . PI * 1.5, Math . PI * 0.5, true);
		ctx . arc (re - rounding, mid + radius + rounding, rounding, Math . PI * 1.5, 0);
		ctx . arc (re - rounding, bt - rounding, rounding, 0, Math . PI * 0.5);
		ctx . arc (le + rounding, bt - rounding, rounding, Math . PI * 0.5, Math . PI);
		ctx . arc (le + rounding, mid + radius + rounding, rounding, Math . PI, Math . PI * 1.5);
		ctx . arc (le + rounding, mid, radius, Math . PI * 0.5, Math . PI * 1.5, true);
		ctx . arc (le + rounding, mid - radius - rounding, rounding, Math . PI * 0.5, Math . PI);
		ctx . closePath ();
		for (var ind in holes) {
			ctx . moveTo (s . length * (s . left_margin + holes [ind] . left), mid - radius);
			ctx . arc (s . length * (s . left_margin + holes [ind] . left), mid, radius, Math . PI * 1.5, Math . PI * 0.5, true);
			ctx . arc (s . length * (s . left_margin + holes [ind] . right), mid, radius, Math . PI * 0.5, Math . PI * 1.5, true);
			ctx . closePath ();
		}
		ctx . fillStyle = background;
		ctx . fill ();
		ctx . strokeStyle = colour;
		ctx . stroke ();
	};
};

var DecilonWindow = function (radius, l1, l2, l3, t1, t2, t3, background, colour) {
	this . draw = function (ctx, s) {
		var le1 = s . length * l1 - radius, le2 = s . length * l2 - radius, le3 = s . length * l3 - radius;
		var te1 = radius - t1, te2 = radius - t2, te3 = radius - t3;
		var ht = s . height ();
		var tb1 = ht - te1, tb2 = ht - te2, tb3 = ht - te3;
		ctx . beginPath ();
		ctx . arc (le1, te1, radius, Math . PI * 1.5, 0);
		ctx . arc (le1, tb1, radius, 0, Math . PI * 0.5);
		ctx . arc (- le1, tb1, radius, Math . PI * 0.5, Math . PI);
		ctx . arc (- le1, te1, radius, Math . PI, Math . PI * 1.5);
		ctx . closePath ();
		ctx . moveTo (le2 + radius, te3);
		ctx . arc (le2, te3, radius, 0, Math . PI * 1.5, true);
		ctx . arc (le3 + radius + radius, te3 - radius - radius, radius, Math . PI * 0.5, Math . PI);
		ctx . arc (le3, te2, radius, 0, Math . PI * 1.5, true);
		ctx . arc (- le3, te2, radius, Math . PI * 1.5, Math . PI, true);
		ctx . arc (- le3 - radius - radius, te3 - radius - radius, radius, 0, Math . PI * 0.5);
		ctx . arc (- le2, te3, radius, Math . PI * 1.5, Math . PI, true);
		ctx . arc (- le2, tb3, radius, Math . PI, Math . PI * 0.5, true);
		ctx . arc (- le3 - radius - radius, tb3 + radius + radius, radius, Math . PI * 1.5, 0);
		ctx . arc (- le3, tb2, radius, Math . PI, Math . PI * 0.5, true);
		ctx . arc (le3, tb2, radius, Math . PI * 0.5, 0, true);
		ctx . arc (le3 + radius + radius, tb3 + radius + radius, radius, Math . PI, Math . PI * 1.5);
		ctx . arc (le2, tb3, radius, Math . PI * 0.5, 0, true);
		ctx . closePath ();
		ctx . fillStyle = background;
		ctx . fill ();
		ctx . strokeStyle = colour;
		ctx . stroke ();
	};
};

var HemmiWindow = function (radius, l1, l2, t1, t3, background, colour) {
	this . draw = function (ctx, s) {
		var le1 = s . length * l1 - radius, le2 = s . length * l2 - radius;
		var te1 = radius - t1, te3 = radius - t3;
		var ht = s . height ();
		var tb1 = ht - te1, tb3 = ht - te3;
		ctx . beginPath ();
		ctx . arc (le1, te1, radius, Math . PI * 1.5, 0);
		ctx . arc (le1, tb1, radius, 0, Math . PI * 0.5);
		ctx . arc (- le1, tb1, radius, Math . PI * 0.5, Math . PI);
		ctx . arc (- le1, te1, radius, Math . PI, Math . PI * 1.5);
		ctx . closePath ();
		ctx . moveTo (le2 + radius, te3);
		ctx . arc (le2, te3, radius, 0, Math . PI * 1.5, true);
		ctx . arc (- le2, te3, radius, Math . PI * 1.5, Math . PI, true);
		ctx . arc (- le2, tb3, radius, Math . PI, Math . PI * 0.5, true);
		ctx . arc (le2, tb3, radius, Math . PI * 0.5, 0, true);
		ctx . closePath ();
		ctx . fillStyle = background;
		ctx . fill ();
		ctx . strokeStyle = colour;
		ctx . stroke ();
	};
};

var DecilonCursorBrace = function (shift, l1, t1, r1, l2, t2, r2, background, colour) {
	this . draw = function (ctx, s) {
		var le1 = s . length * l1, le2 = s . length * l2;
		var ht = s . height ();
		var atan = Math . atan2 (t2 - t1, le1 - le2);
		var delta = {x: le1 - le2, y: t2 - t1};
		var distance = Math . sqrt (delta . x * delta . x + delta . y * delta . y);
		atan -= Math . asin ((r1 - r2) / distance);
		ctx . fillStyle = background; ctx . strokeStyle = colour;
		ctx . beginPath ();
		ctx . arc (- le1, - t1, r1, Math . PI, Math . PI * 1.5 - atan);
		ctx . arc (- le2, - t2, r2, Math . PI * 1.5 - atan, 0);
		ctx . arc (le2, - t2, r2, Math . PI, Math . PI * 1.5 + atan);
		ctx . arc (le1, - t1, r1, Math . PI * 1.5 + atan, 0);
		ctx . lineTo (le1 + r1, - shift);
		ctx . lineTo (- le1 - r1, - shift);
		ctx . closePath ();
		ctx . fill (); ctx . stroke ();
		ctx . beginPath ();
		ctx . arc (le1, ht + t1, r1, 0, Math . PI * 0.5 - atan);
		ctx . arc (le2, ht + t2, r2, Math . PI * 0.5 - atan, Math . PI);
		ctx . arc (- le2, ht + t2, r2, 0, Math . PI * 0.5 + atan);
		ctx . arc (- le1, ht + t1, r1, Math . PI * 0.5 + atan, Math . PI);
		ctx . lineTo (- le1 - r1, ht + shift);
		ctx . lineTo (le1 + r1, ht + shift);
		ctx . closePath ();
		ctx . fill (); ctx . stroke ();
	};
};

var CursorWindow = function (margin, radius, background, colour) {
  this . draw = function (ctx, s) {
    var right = s . cursor_right_extension * s . length;
    var left = - s . cursor_left_extension * s . length;
    var rd = s . cursor_rounding;
    var h = s . height ();
    ctx . beginPath ();
    ctx . arc (right - rd, - s . cursor_top_margin, rd, Math . PI * 1.5, 0);
    ctx . arc (right - rd, h + s . cursor_bottom_margin, rd, 0, Math . PI * 0.5);
    ctx . arc (left + rd, h + s . cursor_bottom_margin, rd, Math . PI * 0.5, Math . PI);
    ctx . arc (left + rd, - s . cursor_top_margin, rd, Math . PI, Math . PI * 1.5);
    ctx . closePath ();
    ctx . moveTo (right - margin, - s . cursor_top_margin + margin - rd + radius);
    ctx . arc (right - radius - margin, - s . cursor_top_margin + margin + radius - rd, radius, 0, Math . PI * 1.5, true);
    ctx . arc (left + radius + margin, - s. cursor_top_margin + margin + radius - rd, radius, Math . PI * 1.5, Math . PI, true);
    ctx . arc (left + radius + margin, h + s . cursor_bottom_margin - margin - radius + rd, radius, Math . PI, Math . PI * 0.5, true);
    ctx . arc (right - radius - margin, h + s . cursor_bottom_margin - margin - radius + rd, radius, Math . PI * 0.5, 0, true);
    ctx . closePath ();
    ctx . fillStyle = background;
    ctx . fill ();
    ctx . strokeStyle = colour; ctx . stroke ();
  };
};

var Screw = function (shift, top, radius, angle, background, colour) {
	this . draw = function (ctx, s) {
		var location = shift * s . length;
		var s = Math . sin (angle) * radius;
		var c = Math . cos (angle) * radius;
		ctx . fillStyle = background; ctx . strokeStyle = colour;
		ctx . beginPath ();
		ctx . arc (location, top, radius, 0, Math . PI * 2);
		ctx . fill (); ctx . stroke ();
		ctx . beginPath (); ctx . moveTo (location + c, top + s); ctx . lineTo (location - c, top - s); ctx . stroke ();
	};
};

var Bolt = function (shift, top, radius, background, colour) {
	this . draw = function (ctx, s) {
		ctx . fillStyle = background; ctx . strokeStyle = colour;
		ctx . beginPath ();
		ctx . arc (shift * s . length, top, radius, 0, Math . PI * 2);
		ctx . fill (); ctx . stroke ();
	};
};

var Glass = function (options) {
	this . glass = "rgba(0, 0, 0, 0.1)";
	this . frame = "rgba(0, 0, 0, 0.1)";
	this . left = 0.1;
	this . right = 0.1;
	this . top = 0;
	this . bottom = 0;
	this . rounding = 4;
	this . draw = function (ctx, s) {
		ctx . beginPath ();
		var le = - s . length * this . left; var re = s . length * this . right;
		roundRect (ctx, le, le, - this . rounding - this . top, re, re, s . height () + this . rounding + this . bottom, this . rounding);
		if (this . glass) {ctx . fillStyle = this . glass; ctx . fill ();}
		if (this . frame) {ctx . strokeStyle = this . frame; ctx . stroke ();}
	};
	for (var key in options) this [key] = options [key];
};

var Sliderule = function (length, options) {
  this . inactive = false;
  this . length = length;
  this . left_margin = 0.2; this . right_margin = 0.2;
  this . position = {x: 0, y: 0};
  this . rules = [];
  this . animation_delta = 0.01;
  this . hairline_top = 0; this . hairline_bottom = 0;
  this . cursor_position = 0; this . cursor_target = 0; this . cursorHairline = 'red'; this . cursor_motion = 0.5;
  this . cursorGlass = "rgba(0, 0, 0, 0.1)";
  this . cursorFrame = "rgba(0, 0, 0, 0.1)";
  this . cursor_left_extension = 0.1; this . cursor_right_extension = 0.1;
  this . cursor_top_margin = 0; this . cursor_bottom_margin = 0;
  this . cursor_rounding = 4;
  this . glasses = [];
  this . cursors = [];
  this . braces = [];
  this . backBraces = [];
  this . cursorBraces = [];
  this . cursorGlassBraces = [];
  this . precision = 5;
  this . static_markings = false;
  this . cursor_markings = false;
  this . extra_cursor_markings = false;
  this . markings_colour = 'black'; this . markings_background = 'white'; this . markings_size = 12;
  this . static_markings_shift = 0.01; this . static_markings_align = 'left';
  this . cursor_markings_shift = 0.01; this . cursor_markings_align = 'left';
  this . cursor_limit_left = 0.2; this . cursor_limit_right = 0.2;
  this . index_area = 0.01;
  this . height = function () {var h = 0; for (var ind in this . rules) h += this . rules [ind] . ruleHeight (); return h;};
  this . moveCursor = function (delta) {
    delta *= this . cursor_motion; delta /= this . length;
    this . cursor_target += delta;
    if (this . cursor_target > 1 + this . cursor_limit_right) this . cursor_target = 1 + this . cursor_limit_right;
    if (this . cursor_target < - this . cursor_limit_left) this . cursor_target = - this . cursor_limit_left;
    this . cursor_position = this . cursor_target;
    return delta;
  };
  this . mover = null;
  this . moveRule = function (delta, position) {
  	if (this . mover == 'cursor') return {rule: this, delta: this . moveCursor (delta . x)};
  	if (this . mover != null) return {rule: this . mover, delta: this . mover . move (delta . x, this . length)};
    if (position . y < 0 || position . y > this . height ()) return null;
    var offset = position . x / this . length - this . left_margin - this . cursor_position;
    if (offset >= - this . cursor_left_extension && offset <= this . cursor_right_extension) {
    	this . mover = 'cursor';
    	return {rule: this, delta: this . moveCursor (delta . x)};
    }
    var y = position . y;
    for (var ind in this . rules) {
      if (this . rules [ind] . hitTest (y)) {
      	this . mover = this . rules [ind];
      	return {rule: this . rules [ind], delta: this . rules [ind] . move (delta . x, this . length)};
      }
      y -= this . rules [ind] . ruleHeight ();
    }
    return null;
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
  this . drawMarkings = function (ctx, shift, align, rule_shift) {
    var y = 0;
    ctx . textBaseline = 'middle';
    ctx . font = this . markings_size + 'px arial';
    var h; var hh; var description; var measure; var rs;
    for (var ind in this . rules) {
      for (var sub in this . rules [ind] . scales) {
        h = this . rules [ind] . scales [sub] . height;
        hh = h * 0.5;
        if (this . rules [ind] . scales [sub] . dimm) description = null;
        else description = this . rules [ind] . scales [sub] . display (this . cursor_position - this . rules [ind] . shift, this . precision);
        if (description !== null) {
          ctx . fillStyle = this . markings_background;
          measure = ctx . measureText (description);
          ctx . textAlign = align;
          rs = shift + (rule_shift ? this . length * this . rules [ind] . shift : 0);
          if (align === 'left') {
            ctx . fillRect (rs, y + hh - 2 - this . markings_size * 0.5, measure . width + 8, this . markings_size + 2);
            ctx . fillStyle = this . markings_colour;
            ctx . fillText (description, rs + 4, y + hh);
          } else if (align === 'center') {
            ctx . fillRect (rs - measure . width * 0.5 - 4, y + hh - 2 - this . markings_size * 0.5, measure . width + 8, this . markings_size + 2);
            ctx . fillStyle = this . markings_colour;
            ctx . fillText (description, rs, y + hh);
          } else {
            ctx . fillRect (rs - measure . width - 8, y + hh - 2 - this . markings_size * 0.5, measure . width + 8, this . markings_size + 2);
            ctx . fillStyle = this . markings_colour;
            ctx . fillText (description, rs - 4, y + hh);
          }
        }
        y += h;
      }
    }
  };
  this . drawExtraMarkings = function (ctx) {
    var y = 0;
    ctx . textBaseline = 'middle';
    ctx . font = this . markings_size + 'px arial';
    var h; var hh; var description; var measure;
    for (var ind in this . rules) {
      for (var sub in this . rules [ind] . scales) {
        h = this . rules [ind] . scales [sub] . height;
        hh = h * 0.5;
        for (var esc in this . cursors) {
          var offset = this . cursors [esc] . shift - (this . cursors [esc] . static ? this . cursor_position : 0);
          if (y + hh >= this . cursors [esc] . from && y + hh <= this . cursors [esc] . to &&
          	((this . extra_cursor_markings && ! this . cursors [esc] . main) || (this . cursor_markings && this . cursors [esc] . main))) {
            ctx . fillStyle = this . markings_background;
            if (this . rules [ind] . scales [sub] . dimm) description = null;
            else description = this . rules [ind] . scales [sub] . display (this . cursor_position - this . rules [ind] . shift + offset, this . precision);
            if (description !== null) {
              measure = ctx . measureText (description);
              ctx . textAlign = this . cursors [esc] . marking_align;
              if (this . cursors [esc] . marking_align === 'left') {
                ctx . fillRect ((offset + this . cursors [esc] . marking_shift) * this . length, y + hh - 2 - this . markings_size * 0.5 + this . cursors [esc] . marking_y_shift, measure . width + 8, this . markings_size + 2);
                ctx . fillStyle = this . markings_colour;
                ctx . fillText (description, (offset + this . cursors [esc] . marking_shift) * this . length + 4, y + hh + this . cursors [esc] . marking_y_shift);
              } else {
                ctx . fillRect ((offset + this . cursors [esc] . marking_shift) * this . length - measure . width - 8, y + hh - 2 - this . markings_size * 0.5 + this . cursors [esc] . marking_y_shift, measure . width + 8, this . markings_size + 2);
                ctx . fillStyle = this . markings_colour;
                ctx . fillText (description, (offset + this . cursors [esc] . marking_shift) * this . length - 4, y + hh + this . cursors [esc] . marking_y_shift);
              }
            }
          }
        }
        y += h;
      }
    }
  };
  this . examine = function (position) {
    if (position . y < 0 || position . y > this .height ()) return false;
    position = subv (position, {x: this . length * this . left_margin, y: 0});
    var adjusted_x_position = position . x / this . length;
    var is_cursor = adjusted_x_position - this . cursor_position;
    is_cursor = (is_cursor < this . cursor_right_extension && is_cursor > - this . cursor_left_extension);
    var value, target, delta;
    for (var ind in this . rules) {
      value = this . rules [ind] . examine (position);
      if (value !== null) {
        if (is_cursor || this . rules [ind] . stator == 0) {
          target = value + this . rules [ind] . shift;
          if (target < - this . cursor_limit_left) target = - this . cursor_limit_left;
          if (target > 1 + this . cursor_limit_right) target = 1 + this . cursor_limit_right;
          delta = target - this . cursor_target;
          this . cursor_target = target;
          track [track . length - 1] . source = 'cursor';
          return {rule: this, target: target, delta: delta, tick: delta * this . length};
        } else {
          if (Math . abs (adjusted_x_position) <= this . index_area) target = - value;
          else if (Math . abs (adjusted_x_position - 1) <= this . index_area) target = 1 - value;
          else target = this . cursor_target - value;
          if (target < -1 - this . left_margin) target = -1 - this . left_margin;
          if (target > 1 + this . right_margin) target = 1 + this . right_margin;
          delta = target - this . rules [ind] . target;
          this . rules [ind] . target = target;
          track [track . length - 1] . source = 'slide';
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
  this . scale = 1; this . scaling_factor = Math . pow (2, 1 / 12);
  this . background_colour = '#99f';
  this . background_translation = {x: 0, y: 0};
  this . sliderules = [];
  this . name = 'generic';
  this . synchronise = function (rule, delta) {
    if (! rule) return;
    var ind, sub, r;
    if (rule . stator) {
      for (ind in this . sliderules) {
        for (sub in this . sliderules [ind] . rules) {
          r = this . sliderules [ind] . rules [sub];
          if (r != rule && ! r . noSync) {
          	if (r . stator == rule . stator) {r . target = rule . target; r . shift = rule . shift; this . requireRedraw = true;}
          	if (r . stator > rule . stator) {r . target += delta; r . shift += delta; this . requireRedraw = true;}
          }
        }
      }
    } else {
      for (ind in this . sliderules) {
        if (this . sliderules [ind] != rule && ! this . sliderules [ind] . noSync) {
          this . sliderules [ind] . cursor_target = rule . cursor_target;
          this . sliderules [ind] . cursor_position = rule . cursor_position; this . requireRedraw = true;
        }
      }
    }
  };
  this . synchroniseTarget = function (delta) {
    var esc = this . examine (scalv (delta, 1 / this . scale));
    if (esc) this . synchronise (esc . rule, esc . delta);
  };
  this . synchroniseMove = function (delta, position) {
    var esc = this . move (scalv (delta, 1 / this . scale), scalv (position, 1 / this . scale));
    if (esc) this . synchronise (esc . rule, esc . delta);
    else this . position = addv (this . position, scalv (delta, 1 / this . scale));
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
    for (var ind in this . sliderules) {
      this . sliderules [ind] . draw (ctx);
    }
  };
  this . move = function (delta, position) {
    position = subv (position, this . position);
    var ret;
    for (var ind in this . sliderules) {
      if (! this . sliderules [ind] . inactive) {
        position = subv (position, this . sliderules [ind] . position);
        ret = this . sliderules [ind] . moveRule (delta, position);
        if (ret) return ret;
        position = subv (position, {x: 0, y: this . sliderules [ind] . height ()});
        if (position . y < 0) return null;
      }
    }
    return null;
  };
  this . resetMovers = function () {for (var ind in this . sliderules) this . sliderules [ind] . mover = null;};
  this . examine = function (position) {
    position = subv (position, this . position);
    var esc;
    for (var ind in this . sliderules) {
      if (! this . sliderules [ind] . inactive) {
        position = subv (position, this . sliderules [ind] . position);
        esc = this . sliderules [ind] . examine (position);
        if (esc) return esc;
        position = subv (position, {x: 0, y: this . sliderules [ind] . height ()});
      }
    }
  };
  this . noChange = function () {
  	if (this . requireRedraw) return false;
  	for (var ind in this . sliderules) {if (this . sliderules [ind] . changed ()) return false;}
  	return true;
  };
  for (var key in options) this [key] = options [key];
};
