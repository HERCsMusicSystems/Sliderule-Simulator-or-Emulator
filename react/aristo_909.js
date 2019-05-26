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

s = new Sliderule (1200,
  {inactive: true, static_markings: false, markings_colour: 'gold', markings_background: 'black',
  cursor_markings_shift: 0.002, cursor_rounding: 10, cursor_limit_left: 0.052, cursor_limit_right: 0.052,
  cursor_left_extension: 0.076, cursor_right_extension: 0.076, cursor_top_margin: 12, cursor_bottom_margin: 12, cursorHairline: 'black'});
sliderules . sliderules . push (s);
s . backBraces . push (new LeftBrace (2, 0.07, 8, 'white', 'black', 68, 1));
s . backBraces . push (new RightBrace (2, 0.07, 8, 'white', 'black', 68, 1));
//s . cursorGlassBraces . push (new Bolt (HairlineS + 0.003, 285, 8, '#a59632', '#605321'));
//s . cursorGlassBraces . push (new Bolt (HairlineD + 0.007, 285, 8, '#a59632', '#605321'));
s . cursorGlassBraces . push (new CursorBrace (0.090, 0.090, 24, -1, 24, 'tan'));
//s . cursorBraces . push (new Bolt (HairlineS + 0.003, 285, 4, '#b5a642', '#605321'));
//s . cursorBraces . push (new Bolt (HairlineD + 0.007, 285, 4, '#b5a642', '#605321'));
s . cursors . push (new Cursor (Hairline360, 56, 104, 'black', {marking_shift: 0.002}));
s . cursorBraces . push (new Screw (HairlineS - 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
s . cursorBraces . push (new Screw (HairlineD + 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
s . cursorBraces . push (new Screw (0, 260 + 11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
s . cursorBraces . push (new Engraving ("ARISTO", '16px arial', 'center', 'black', 0, -4));
s . cursorBraces . push (new Engraving ("D B G M", '10px arial', 'right', 'black', -0.0125, 260 + 4 + 12));
s . cursorBraces . push (new Engraving ("\u2190", '22px arial', 'left', 'black', -0.074, 260 + 4 + 14));
s . cursorBraces . push (new Engraving ("\u2192", '22px arial', 'right', 'black', 0.074, 260 + 4 + 14));
r = new Rule ();
s . rules . push (r);
r . markings . push (new Engraving (sliderules . name, '14px arial', 'right', 'red', 0.028, 15));
r . scales . push (new spacer (16));
r . scales . push (new scale_J (24, {draw_pi: false, draw_e: false, left: "K", la: 'center', ls: 0.084, right: "X\u00b3", rs: 0.08, indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (16));
r . scales . push (new scale_CF (24, {left: "DF", la: 'center', ls: 0.084, right: "\u03c0X", rs: 0.08, ra: 'left', draw_e: false, draw_c: false, left_extension: 0.022, right_extension: 0.022, indices: ['>>1<<']}));
r = new Rule ({stator: 1, rounding: sliderules . slider_rounding});
s . rules . push (r);
r . markings . push (new Engraving ("ARISTO", '16px arial', 'center', 'black', 1.045, 50, Math . PI * 1.5));
r . markings . push (new Engraving ("Nr. 909", '14px arial', 'center', 'black', 1.059, 50, Math . PI * 1.5));
r . markings . push (new Engraving ("MADE IN GERMANY", '8px arial', 'center', 'black', 1.07, 50, Math . PI * 1.5));
r . scales . push (new scale_DF (24, {left: "CF", la: 'center', ls: 0.084, right: "\u03c0X", rs: 0.08, ra: 'left', draw_e: false, draw_c: false, left_extension: 0.022, right_extension: 0.022,
  highlight: sliderules . highlight, highlight_left: 0.19, highlight_right: 0.19, indices: ['>>1<<']}));
r . scales . push (new spacer (2));
r . scales . push (new scale_DIF (24, {left: "CIF", la: 'center', ls: 0.084, right: "1/(\u03c0X)", rs: 0.08, ra: 'left', draw_e: false, draw_c: false, marking_colour: 'red', left_extension: 0.018, right_extension: 0.026}));
r . scales . push (new scale_CI (24, {left: "CI", la: 'center', ls: 0.084, right: "1/x", rs: 0.08, ra: 'left', draw_e: false, draw_c: false, marking_colour: 'red', indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (2));
r . scales . push (new scale_C (24, {left: "C", la: 'center', ls: 0.084, right: "X", rs: 0.08, ra: 'left', draw_q: true, draw_e: false, draw_c: false, highlight: sliderules . highlight, highlight_left: 0.19, highlight_right: 0.19, indices: [1, 1, 1, 1]}));
r = new Rule ();
s . rules . push (r);
if (sliderules . charvoz) r . markings . push (new Engraving ("CHARVOZ", '14px arial', 'center', 'black', -0.04, 68));
r . scales . push (new scale_D (24, {left: "D", la: 'center', ls: 0.084, right: "X", rs: 0.08, ra: 'left', draw_e: false, draw_c: false, indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (16));
r . scales . push (new scale_L (24, {left: "L", la: 'center', ls: 0.084, right: "lg x", rs: 0.08, ra: 'left'}));
r . scales . push (new spacer (16));

var back_german_cursors = [], back_us_cursors = [], back_german_braces = [], back_us_braces = [];
var s = new Sliderule (1200, {static_markings: false, cursor_markings: false, cursor_markings_shift: 0.002, markings_colour: 'gold', markings_background: 'black',
  cursor_rounding: 10, cursor_limit_left: 0.052, cursor_limit_right: 0.052, cursor_left_extension: 0.076, cursor_right_extension: 0.076,
  cursor_top_margin: 12, cursor_bottom_margin: 12, cursorHairline: 'black'});
s . braces . push (new LeftBrace (2, 0.07, 8, 'white', 'black', 68, 1));
s . braces . push (new LeftBrace (4, 0.063, 8, 'white', 'black', 68, 1));
s . braces . push (new RightBrace (2, 0.07, 8, 'white', 'black', 68, 1));
s . braces . push (new RightBrace (4, 0.063, 8, 'white', 'black', 68, 1));
s . braces . push (new Engraving ("ARISTO", '24px arial', 'center', '#dddddd', 0.04, 130, Math . PI * 0.5));
s . braces . push (new Engraving ("ARISTO", '24px arial', 'center', '#dddddd', 1.36, 130, Math . PI * 1.5));
s . cursorGlassBraces . push (new CursorBrace (0.090, 0.090, 24, -1, 24, 'tan'));
back_german_cursors . push (new Cursor (HairlineS, 56, 104, 'black', {marking_shift: -0.002, marking_align: 'right'}));
back_german_cursors . push (new Cursor (HairlinePS, 56, 104, 'black', {marking_shift: 0.002}));
back_german_cursors . push (new Cursor (HairlineD, 156, 204, 'black', {marking_shift: 0.002}));
back_us_cursors . push (new Cursor (HairlineS, 56, 104, 'black', {marking_shift: -0.002, marking_align: 'right'}));
back_us_cursors . push (new Cursor (HairlineHPUS, 56, 104, 'black', {marking_shift: 0.002}));
back_us_cursors . push (new Cursor (HairlineD, 156, 204, 'black', {marking_shift: 0.002}));
back_german_braces . push (new Engraving ("ARISTO", '16px arial', 'center', 'black', 0, 280));//, '12px arial', 'black', 'right', 'middle'));
back_german_braces . push (new Engraving ("W", '16px arial', 'center', 'black', 0, -4));
back_german_braces . push (new Engraving ("PS", '12px arial', 'center', 'black', HairlinePS, 52));
back_german_braces . push (new Screw (HairlineS - 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_german_braces . push (new Screw (HairlineD + 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_german_braces . push (new Screw (HairlineS - 0.003, 260 + 11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_german_braces . push (new Screw (HairlineD + 0.003, 260 + 11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_us_braces . push (new Engraving ("ARISTO", '16px arial', 'center', 'black', 0, 280));//, '12px arial', 'black', 'right', 'middle'));
back_us_braces . push (new Engraving ("W", '16px arial', 'center', 'black', 0, -4));
back_us_braces . push (new Engraving ("HP", '12px arial', 'center', 'black', HairlineHPUS, 52));
back_us_braces . push (new Screw (HairlineS - 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_us_braces . push (new Screw (HairlineD + 0.003, -11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_us_braces . push (new Screw (HairlineS - 0.003, 260 + 11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
back_us_braces . push (new Screw (HairlineD + 0.003, 260 + 11, 8, Math . random () * Math . PI * 2, 'silver', 'black'));
sliderules . sliderules . push (s);
var r = new Rule ();
s . rules . push (r);
r . scales . push (new spacer (24));
r . scales . push (new scale_P_down (24, {left: "P", ls: 0.084, la: 'center', right: "\u221A(1-x\u00B2)", rs: 0.08, ra: 'left', marking_colour: 'red'}));
r . scales . push (new spacer (8));
r . scales . push (new scale_A (24, {left: "A", ls: 0.084, la: 'center', right: "X\u00B2", rs: 0.08, ra: 'left', left_extension: 0.05, right_extension: 0.06, alt: 'black', draw_e: false, indices: [1, 1, 1, 1]}));
r = new Rule ({rounding: sliderules . slider_rounding, stator: 1});
s . rules . push (r);
if (sliderules . dbgm)  r . markings . push (new Engraving ("D B G M", '12px arial', 'center', 'black', 1.19, 50, Math . PI * 1.5));
r . scales . push (new scale_B (24, {left: "B", ls: 0.084, la: 'center', right: "X\u00B2", rs: 0.08, ra: 'left', left_extension: 0.05, right_extension: 0.06, alt: 'black', draw_e: false, indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (2));
r . scales . push (new scale_Tdec (24, {left: "T", ls: 0.084, la: 'center', right: "\u2222tg", rs: 0.08, ra: 'left', left_extension: 0.024}));
r . scales . push (new spacer (1));
r . scales . push (new scale_STdec (24, {left: "ST", ls: 0.084, la: 'center', right: "\u2222arc", rs: 0.08, ra: 'left', left_extension: 0.02, right_extension: 0.022, draw_st_corrections: false}));
r . scales . push (new spacer (1));
r . scales . push (new scale_Sdec (24, {left: "S", ls: 0.084, la: 'center', right: "\u2222sin", rs: 0.08, ra: 'left', left_extension: 0.024}));
r = new Rule ();
s . rules . push (r);
r . scales . push (new scale_D (24, {left: "D", ls: 0.084, la: 'center', right: "X", rs: 0.08, ra: 'left', left_extension: 0.048, right_extension: 0.056, draw_c: false, draw_e: false, alt: 'black', indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (8));
r . scales . push (new scale_CI (24, {left: "DI", ls: 0.084, la: 'center', right: "1/X", rs: 0.08, ra: 'left', marking_colour: 'red', draw_e: false, draw_c: false, indices: [1, 1, 1, 1]}));
r . scales . push (new spacer (24));

