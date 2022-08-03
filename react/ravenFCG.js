///////////////////////////////////////////////////////////////////////////////////
//                     Copyright (C) 2017 Dr Robert P. Wolf                      //
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

var front_german_cursors = [], front_us_cursors = [], front_german_braces = [], front_us_braces = [];
var back_german_cursors = [], back_us_cursors = [], back_german_braces = [], back_us_braces = [];

var s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22, cursor_top_margin: 13, cursor_bottom_margin: 13, hairline_top: -10, hairline_bottom: -10, cursor_limit_left: 0.062, cursor_limit_right: 0.062, markings_background: 'yellow', cursor_left_extension: 0.09, cursor_right_extension: 0.09, cursor_markings_shift: 0.005}); sliderules . sliderules . push (s);
s . braces . push (new LeftBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new LeftBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new LeftBraceBar (0.052, 20, 20, 4, 'red', 'black'));
s . braces . push (new LeftBraceBar (0.052, 20, 244, 4, 'gray', 'black'));
s . braces . push (new LeftBraceBar (0.052, 244, 20, 4, 'gray', 'black'));
s . braces . push (new RightBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new RightBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new RightBraceBar (0.052, 20, 20, 4, 'red', 'black'));
s . braces . push (new RightBraceBar (0.052, 20, 244, 4, 'gray', 'black'));
s . braces . push (new RightBraceBar (0.052, 244, 20, 4, 'gray', 'black'));
s . braces . push (new Screw (0.026, 42, 12, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (0.016, 222, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (0.032, 268, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.026, 42, 12, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.016, 222, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.032, 268, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . cursorGlassBraces . push (new DecilonCursorBrace (1, 0.09, 12, 8, 0.04, 18, 4, 'tan', 'black'));
var screw_angles = [Math . random () * Math . PI, Math . random () * Math . PI, Math . random () * Math . PI, Math . random () * Math . PI];
var GRHairlineLeft = HairlineD - HairlinePS - HairlinePS;
var GRHairlineRight = GRHairlineLeft - Math . log10 (Math . log (2));
var USHairlineLeft = HairlineD - HairlineHPUS - HairlineHPUS;
var USHairlineRight = USHairlineLeft - Math . log10 (Math . log (2));
front_german_braces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
front_german_braces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
front_german_braces . push (new Engraving ('PS', '12px arial', 'center', 'red', HairlinePS, 68));
front_german_braces . push (new Engraving ('s', '12px arial', 'center', 'red', HairlineS, 68));
front_german_braces . push (new Engraving ('d', '12px arial', 'center', 'red', HairlineD, 166));
front_german_braces . push (new Engraving ('PS', '12px arial', 'center', 'red', HairlineD, 235));
front_german_braces . push (new Engraving ('kW', '12px arial', 'left', 'red', GRHairlineLeft + 0.0025, 235));
front_german_braces . push (new Screw (-0.07, -9, 6, screw_angles [0], 'gold', 'silver'));
front_german_braces . push (new Screw (0.07, -9, 6, screw_angles [1], 'gold', 'silver'));
front_german_braces . push (new Screw (-0.07, 303, 6, screw_angles [2], 'gold', 'silver'));
front_german_braces . push (new Screw (0.07, 303, 6, screw_angles [3], 'gold', 'silver'));
front_us_braces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
front_us_braces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
front_us_braces . push (new Engraving ('HP', '12px arial', 'center', 'red', HairlineHPUS, 68));
front_us_braces . push (new Engraving ('s', '12px arial', 'center', 'red', HairlineS, 68));
front_us_braces . push (new Engraving ('d', '12px arial', 'center', 'red', HairlineD, 166));
front_us_braces . push (new Engraving ('HP', '12px arial', 'center', 'red', HairlineD, 235));
front_us_braces . push (new Engraving ('kW', '12px arial', 'left', 'red', USHairlineLeft + 0.0025, 235));
front_us_braces . push (new Screw (-0.06, -9, 6, screw_angles [0], 'gold', 'silver'));
front_us_braces . push (new Screw (0.06, -9, 6, screw_angles [1], 'gold', 'silver'));
front_us_braces . push (new Screw (-0.06, 303, 6, screw_angles [2], 'gold', 'silver'));
front_us_braces . push (new Screw (0.06, 303, 6, screw_angles [3], 'gold', 'silver'));
front_german_cursors . push (new Cursor (HairlinePS, 74, 122, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (HairlineS, 74, 122, 'red', {marking_shift: 0.005}));
front_german_cursors . push (new Cursor (HairlineD, 172, 220, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineLeft, 26, 74, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineLeft, 76, 120, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineLeft, 122, 170, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineLeft, 172, 218, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineLeft, 220, 284, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_german_cursors . push (new Cursor (GRHairlineRight, 26, 74, 'red', {marking_shift: 0.005}));
front_german_cursors . push (new Cursor (GRHairlineRight, 76, 120, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_german_cursors . push (new Cursor (GRHairlineRight, 122, 170, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_german_cursors . push (new Cursor (GRHairlineRight, 172, 218, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_german_cursors . push (new Cursor (GRHairlineRight, 220, 284, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_us_cursors . push (new Cursor (HairlineHPUS, 74, 122, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (HairlineS, 74, 122, 'red', {marking_shift: 0.005}));
front_us_cursors . push (new Cursor (HairlineD, 172, 220, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineLeft, 26, 74, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineLeft, 76, 120, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineLeft, 122, 170, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineLeft, 172, 218, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineLeft, 220, 284, 'red', {marking_align: 'right', marking_shift: -0.005}));
front_us_cursors . push (new Cursor (USHairlineRight, 26, 74, 'red', {marking_shift: 0.005}));
front_us_cursors . push (new Cursor (USHairlineRight, 76, 120, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_us_cursors . push (new Cursor (USHairlineRight, 122, 170, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_us_cursors . push (new Cursor (USHairlineRight, 172, 218, 'red', {marking_shift: 'right', marking_shift: 0.005}));
front_us_cursors . push (new Cursor (USHairlineRight, 220, 284, 'red', {marking_shift: 'right', marking_shift: 0.005}));
var r = new Rule ({left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . markings . push (new Engraving (sliderules . name, '16px arial', 'left', 'red', 0, 20));
r . markings . push (new Engraving (sliderules . made_in, '12px arial', 'right', 'black', 1, 16));
r . scales . push (new spacer (26));
r . scales . push (new scale_P (24, {left: "P", ls: 0.145, right: "\u221a1-\u{1d465}\u00b2", rs: 0.145, ra: 'right', left_extension: 0.1115}));
r . scales . push (new scale_K (24, {left: "K", ls: 0.145, right: "\u{1d465}\u00b3", rs: 0.145, ra: 'right', left_extension: 0.1006, right_extension: 0.1004, indices: ['1', '10', '100', '1000']}));
r . scales . push (new scale_A (24, {left: "A", ls: 0.145, right: "\u{1d465}\u00b2", rs: 0.145, ra: 'right', left_extension: 0.111, right_extension: 0.089, indices: ['1', '10', '100'], highlight: '#eeeeff'}));
r = new Rule ({stator: 1, left_margin: 0.23, right_margin: 0.23}); s . rules . push (r);
r . markings . push (new RuleBars (-0.224, 1, 4, 94, 9, 6, 4, 'tan'));
r . markings . push (new RuleBars (1.224, -1, 4, 94, 9, 6, 4, 'tan'));
r . scales . push (new scale_B (24, {left: "B", ls: 0.145, right: "\u{1d465}\u00b2", rs: 0.145, ra: 'right', left_extension: 0.111, right_extension: 0.089, indices: ['1', '10', '100'], highlight: '#eeeeff'}));
r . scales . push (new scale_SCdec (24, {left: "S", ls: 0.145, right: "sin \u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.106}));
r . scales . push (new spacer (2));
r . scales . push (new scale_TCTdec_down (24, {left: "T", ls: 0.145, right: "tan \u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.106, right_extension: 0.0762}));
r . scales . push (new scale_C (24, {left: "C", ls: 0.145, right: "\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.097, right_extension: 0.114, draw_st_corrections: true, draw_r: true, draw_q: true, highlight: '#ccffcc'}));
r = new Rule ({left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_D (24, {left: "D", ls: 0.145, right: "\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.097, right_extension: 0.114, draw_r: true, draw_q: true, highlight: '#ccffcc'}));
r . scales . push (new scale_DI (24, {left: "DI", ls: 0.145, right: "10/\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.114, right_extension: 0.097, marking_colour: 'red'}));
r . scales . push (new scale_LL01_down (24, {left: "LL01", ls: 0.145, marking_colour: 'red', right: "e\u207b\u2070\u22c5\u2070\u00b9\u02e3", rs: 0.1, ra: 'left', left_extension: 0.0968, right_extension: 0.087}));
r . scales . push (new scale_LL00_down (24, {left: "LL00", ls: 0.145, marking_colour: 'red', right: "e\u207b\u2070\u22c5\u2070\u2070\u00b9\u02e3", rs: 0.1, ra: 'left', left_extension: 0.0968, right_extension: 0.084}));
r . scales . push (new spacer (2));


var s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22, cursor_top_margin: 13, cursor_bottom_margin: 13, hairline_top: -10, hairline_bottom: -10, cursor_limit_left: 0.062, cursor_limit_right: 0.062, markings_background: 'yellow', cursor_left_extension: 0.09, cursor_right_extension: 0.09, cursor_markings_align: 'right', cursor_markings_shift: -0.005}); sliderules . sliderules . push (s);
s . braces . push (new LeftBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new LeftBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new LeftBraceBar (0.052, 20, 20, 4, 'red', 'black'));
s . braces . push (new LeftBraceBar (0.052, 20, 244, 4, 'gray', 'black'));
s . braces . push (new LeftBraceBar (0.052, 244, 20, 4, 'gray', 'black'));
s . braces . push (new RightBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new RightBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new RightBraceBar (0.052, 20, 20, 4, 'red', 'black'));
s . braces . push (new RightBraceBar (0.052, 20, 244, 4, 'gray', 'black'));
s . braces . push (new RightBraceBar (0.052, 244, 20, 4, 'gray', 'black'));
s . braces . push (new Screw (0.026, 294 - 42, 12, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (0.016, 294 - 222, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (0.032, 294 - 268, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.026, 294 - 42, 12, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.016, 294 - 222, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . braces . push (new Screw (1.44 - 0.032, 294 - 268, 8, Math . random () * Math . PI, '#b5a642', '#605321'));
s . cursorGlassBraces . push (new DecilonCursorBrace (1, 0.09, 12, 8, 0.04, 18, 4, 'tan', 'black'));
back_german_braces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
back_german_braces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
back_german_braces . push (new Engraving ('360', '12px arial', 'center', 'red', Hairline360, 68));
back_german_braces . push (new Screw (-0.07, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_german_braces . push (new Screw (0.07, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_german_braces . push (new Screw (-0.07, 303, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_german_braces . push (new Screw (0.07, 303, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_us_braces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
back_us_braces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
back_us_braces . push (new Engraving ('360', '12px arial', 'center', 'red', Hairline360, 68));
back_us_braces . push (new Screw (-0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_us_braces . push (new Screw (0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_us_braces . push (new Screw (-0.06, 303, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_us_braces . push (new Screw (0.06, 303, 6, Math . random () * Math . PI, 'gold', 'silver'));
back_german_cursors . push (new Cursor (Hairline360, 74, 122, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineLeft, 10, 74, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineLeft, 76, 120, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineLeft, 122, 170, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineLeft, 172, 218, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineLeft, 220, 284, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_german_cursors . push (new Cursor (GRHairlineRight, 10, 74, 'red', {marking_shift: 0.005}));
back_german_cursors . push (new Cursor (GRHairlineRight, 76, 120, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_german_cursors . push (new Cursor (GRHairlineRight, 122, 170, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_german_cursors . push (new Cursor (GRHairlineRight, 172, 218, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_german_cursors . push (new Cursor (GRHairlineRight, 220, 284, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_us_cursors . push (new Cursor (Hairline360, 74, 122, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineLeft, 10, 74, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineLeft, 76, 120, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineLeft, 122, 170, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineLeft, 172, 218, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineLeft, 220, 284, 'red', {marking_align: 'right', marking_shift: -0.005}));
back_us_cursors . push (new Cursor (USHairlineRight, 10, 74, 'red', {marking_shift: 0.005}));
back_us_cursors . push (new Cursor (USHairlineRight, 76, 120, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_us_cursors . push (new Cursor (USHairlineRight, 122, 170, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_us_cursors . push (new Cursor (USHairlineRight, 172, 218, 'red', {marking_shift: 'right', marking_shift: 0.005}));
back_us_cursors . push (new Cursor (USHairlineRight, 220, 284, 'red', {marking_shift: 'right', marking_shift: 0.005}));
var r = new Rule ({left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . markings . push (new Engraving (sliderules . name, '12px arial', 'center', 'red', 0.05, 12));
r . markings . push (new Engraving (sliderules . made_in, '12px arial', 'center', 'black', 0.95, 12));
r . scales . push (new spacer (2));
r . scales . push (new scale_L (24, {left: "L", ls: 0.145, right: "log \u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.1, right_extension: 0.1}));
r . scales . push (new scale_LL03 (24, {left: "LL03", ls: 0.145, marking_colour: 'red', right: "e\u207b\u02e3", left_extension: 0.09773, rs: 0.1, right_extension: 0.0866}));
r . scales . push (new scale_LL02 (24, {left: "LL02", ls: 0.145, marking_colour: 'red', right: "e\u207b\u2070\u22c5\u00b9\u02e3", rs: 0.1, left_extension: 0.10813, right_extension: 0.08062}));
r . scales . push (new scale_CF (24, {left: "DF", ls: 0.145, right: "\u03c0\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.0993, right_extension: 0.105, indices: ['>>1<<'], highlight: '#eeeeff'}));
r = new Rule ({stator: 1, left_margin: 0.23, right_margin: 0.23}); s . rules . push (r);
r . markings . push (new RuleBars (-0.224, 1, 4, 94, 9, 6, 4, 'tan'));
r . markings . push (new RuleBars (1.224, -1, 4, 94, 9, 6, 4, 'tan'));
r . scales . push (new scale_DF (24, {left: "CF", ls: 0.145, right: "\u03c0\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.0993, right_extension: 0.105, indices: ['>>1<<'], draw_r: true, draw_q: true, highlight: '#ccffcc'}));
r . scales . push (new scale_CIF (24, {left: "CIF", ls: 0.145, right: "10/\u03c0\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.1, right_extension: 0.105, marking_colour: 'red', indices: ['>>1<<']}));
r . scales . push (new spacer (2));
r . scales . push (new scale_DI (24, {left: "CI", ls: 0.145, right: "10/\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.114, right_extension: 0.097, marking_colour: 'red'}));
r . scales . push (new scale_C (24, {left: "C", ls: 0.145, right: "\u{1d465}", rs: 0.145, ra: 'right', left_extension: 0.097, right_extension: 0.114, draw_c: false, highlight: '#ccffcc'}));
r = new Rule ({left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_DLL0 (24, {left: "D", ls: 0.145, right: "\u{1d465}", rs: 0.145, ra: 'right', centre: "e\u2070\u22c5\u2070\u2070\u00b9\u02e3", cs: 1.1, ca: 'left', cc: 'red', left_extension: 0.097, right_extension: 0.08, draw_c: false, highlight: '#eeeeff'}));
r . markings . push (new Engraving ("LL0", 'italic 12px arial', 'left', 'red', -0.13, 18));
r . scales . push (new scale_LL1_down (24, {left: "LL1", ls: 0.145, right: "e\u2070\u22c5\u2070\u00b9\u02e3", rs: 0.1, left_extension: 0.09865, right_extension: 0.088}));
r . scales . push (new scale_LL2_down (24, {left: "LL2", ls: 0.145, right: "e\u2070\u22c5\u00b9\u02e3", rs: 0.1, left_extension: 0.114, right_extension: 0.088}));
r . scales . push (new scale_LL3_down (24, {left: "LL3", ls: 0.145, right: "e\u02e3", rs: 0.1, left_extension: 0.1033, right_extension: 0.087}));
r . scales . push (new spacer (2));

