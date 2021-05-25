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

var changeMarkings = function (type, value) {
	for (var ind in sliderules . sliderules) {
		switch (type) {
		case 'static': case 'stator': case 'rule': sliderules . sliderules [ind] . static_markings = value; break;
		case 'cursor': case 'hairline': sliderules . sliderules [ind] . cursor_markings = value; break;
		case 'cursors': case 'hairlines': case 'extras': sliderules . sliderules [ind] . extra_cursor_markings = value; break;
		default: break;
		}
	}
	sliderules . requireRedraw = true;
};

var changeAndActivateMarkings = function (type, value) {
	for (var ind in sliderules . sliderules) {
		switch (type) {
		case 'static': case 'stator': case 'rule': sliderules . sliderules [ind] . static_markings = value; break;
		case 'cursor': case 'hairline': sliderules . sliderules [ind] . cursor_markings = value; break;
		case 'cursors': case 'hairlines': case 'extras':
      sliderules . sliderules [ind] . extra_cursor_markings = value;
      sliderules . sliderules [ind] . hairlines_inactive = ! value;
      break;
		default: break;
		}
	}
	sliderules . requireRedraw = true;
};

var changeSide = function (side) {
	switch (side) {
	case 'front': sliderules . sliderules [0] . inactive = false; sliderules . sliderules [1] . inactive = true; break;
	case 'back': sliderules . sliderules [0] . inactive = true; sliderules . sliderules [1] . inactive = false; break;
	case 'both': sliderules . sliderules [0] . inactive = false; sliderules . sliderules [1] . inactive = false; break;
	default: break;
	}
	switch (side) {
	case 'both': if (sliderules . sliderules [1] . position . y === 0) sliderules . sliderules [1] . position . y = 60; break;
	default: if (sliderules . sliderules [1] . position . y === 60) sliderules . sliderules [1] . position . y = 0; break;
	}
	sliderules . requireRedraw = true;
};

var changeSize = function (length) {for (var ind in sliderules . sliderules) sliderules . sliderules [ind] . length = length; sliderules . requireRedraw = true;};

var changeVersion = function (version) {
	switch (version) {
	case 'us':
    if (window . front_us_braces != undefined) sliderules . sliderules [0] . cursorBraces = front_us_braces;
    if (window . front_us_cursors != undefined) sliderules . sliderules [0] . cursors = front_us_cursors;
    if (window . back_us_braces != undefined) sliderules . sliderules [1] . cursorBraces = back_us_braces;
    if (window . back_us_cursors != undefined) sliderules . sliderules [1] . cursors = back_us_cursors;
    break;
	case 'german':
    if (window . front_german_braces != undefined) sliderules . sliderules [0] . cursorBraces = front_german_braces;
    if (window . front_german_cursors != undefined) sliderules . sliderules [0] . cursors = front_german_cursors;
    if (window . back_german_braces != undefined) sliderules . sliderules [1] . cursorBraces = back_german_braces;
    if (window . back_german_cursors != undefined) sliderules . sliderules [1] . cursors = back_german_cursors;
    break;
	default: break;
	}
	sliderules . requireRedraw = true;
};

var changeMovementSpeed = function (speed) {
    for (var ind in sliderules . sliderules) {
        var sliderule = sliderules . sliderules [ind];
        sliderule . cursor_motion = speed;
        for (var sub in sliderule . rules) {
            sliderule . rules [sub] . rule_motion = speed;
        }
    }
};

var copyright = `Emulator / Simulator Copyright \u00a9 2017 - ${new Date () . getFullYear ()} Dr Robert Wolf @ http://www.sliderules.org Email: robert.wolf@hercsmusicsystems.com.au`;
var copyright_colours = ['blue', 'gray', 'yellow', 'lavender', 'green', 'olive', 'silver', 'tan', 'wheat', 'khaki'];
var copyright_colour = copyright_colours [Math . floor (Math . random () * copyright_colours . length)];

var slideruleMission = function () {if (sliderules . mission) if (sliderules . mission ()) delete sliderules . mission;};
var slideruleObjective = function () {if (sliderules . objective) if (sliderules . objective ()) {delete sliderules . objective; slideruleMission ();}};

sliderules . checkRequired = false;
sliderules . dragging = false;
sliderules . mousePosition = {x: 0, y: 0};

var rootDiv = document . getElementById ('root');
var rootRootDiv = document . createElement ('div');
rootRootDiv . id = 'rooter';
rootRootDiv . setAttribute ("style", "align: center;");
rootRootDiv . oncontextmenu = function (event) {event . preventDefault ();};
rootRootDiv . onmousedown = function (event) {
	sliderules . resetMovers ();
	var position = subvbc ({x: event . clientX, y: event . clientY}, slideruleCanvas . getBoundingClientRect ());
	if (event . button === 2) {
		if (event . ctrlKey) sliderules . deactivateHairlines (false);
		if (event . shiftKey) sliderules . overrideSlides (true);
		var ret = sliderules . synchroniseTarget (addv (position, {x: 0.5, y: 0}));
		if (event . ctrlKey) sliderules . deactivateHairlines (true);
		if (event . shiftKey) sliderules . overrideSlides (false);
	} else sliderules . dragging = true;
	sliderules . mousePosition = position;
};
rootRootDiv . onmousemove = function (event) {
	if (! sliderules . dragging) return;
	var position = subvbc ({x: event . clientX, y: event . clientY}, slideruleCanvas . getBoundingClientRect ());
	var delta = subv (position, sliderules . mousePosition);
	sliderules . synchroniseMove (delta, position, sliderules . mousePosition);
	sliderules . mousePosition = position;
};
rootRootDiv . onmouseup = function (event) {sliderules . dragging = false;};
rootRootDiv . onmouseleave = function (event) {sliderules . dragging = false;};
rootRootDiv . onmouseout = function (event) {sliderules . dragging = false;};
rootRootDiv . onwheel = function (event) {
	event . preventDefault ();
	var delta = sliderules . scale;
	if (event . deltaY < 0) delta *= sliderules . scaling_factor;
	if (event . deltaY > 0) delta /= sliderules . scaling_factor;
	var point = subvbc ({x: event . clientX, y: event . clientY}, slideruleCanvas . getBoundingClientRect ());
	point = scalv (point, 1 / sliderules . scale);
	var offset = scalv (point, delta - sliderules . scale);
	sliderules . position = subv (sliderules . position, scalv (offset, 1 / delta));
	sliderules . scale = delta;
	sliderules . requireRedraw = true;
};

var touchTimer;
var longDuration = 550; //ms
var onLongTouch;
var startPos;

rootRootDiv . ontouchstart = function (event) {
	event . preventDefault ();
	sliderules . resetMovers ();
	sliderules . dragging = true;
	touchTimer = setTimeout(onLongTouch, longDuration);
	var touches = event.changedTouches;
	startPos = subvbc ({x: touches[0] . clientX, y: touches[0] . clientY}, slideruleCanvas . getBoundingClientRect ());
	sliderules . mousePosition = startPos;
};

rootRootDiv . ontouchmove = function (event) {
	if (! sliderules . dragging) return;
	var touches = event.changedTouches;
	var position = subvbc ({x: touches[0] . clientX, y: touches[0] . clientY}, slideruleCanvas . getBoundingClientRect ());
	var delta = subv (position, sliderules . mousePosition);
	sliderules . synchroniseMove (delta, position, sliderules . mousePosition);
	sliderules . mousePosition = position;
};

rootRootDiv . ontouchend = function (event) {
	sliderules . dragging = false;
	clearTimeout(touchTimer);	
};

onLongTouch = function () {
	if(Math.abs(startPos . x - sliderules . mousePosition . x) < 10 && Math.abs(startPos . y - sliderules . mousePosition . y) < 10){
		var ret = sliderules . synchroniseTarget (addv (startPos, {x: 0.5, y: 0}));
	}
};

rootRootDiv . onscroll = function () {return false;}

rootDiv . appendChild (rootRootDiv);
var slideruleCanvas = document . createElement ('canvas');
slideruleCanvas . id = 'sliderule_canvas';
rootRootDiv . appendChild (slideruleCanvas);

var ctx = slideruleCanvas . getContext ('2d');

var previous_width = 0, previous_height = 0;
var drawSliderule = function () {
	var width = window . innerWidth, height = window . innerHeight;
	var bound = slideruleCanvas . getBoundingClientRect ();
	var new_width = width - bound . left * 4, new_height = height - bound . top * 1.5;
	if (sliderules . fixedHeight) new_height = sliderules . fixedHeight;
//	console . log (width, height, new_width, new_height, bound);
	if (previous_width !== new_width || previous_height !== new_height) {
		slideruleCanvas . width = new_width;
		slideruleCanvas . height = new_height;
		previous_width = new_width;
		previous_height = new_height;
		sliderules . requireRedraw = true;
	}
	if (sliderules . noChange ()) {
		if (sliderules . checkRequired) {slideruleObjective (); sliderules . checkRequired = false;}
		return;
	}
	sliderules . checkRequired = true;
	ctx . save ();
	sliderules . draw (ctx, new_width, new_height);
	ctx . restore ();
	ctx . fillStyle = copyright_colour;
	ctx . textAlign = 'right';
	ctx . fillText (copyright, new_width - 4, new_height - 4);
};

setInterval (drawSliderule, 20);

'use strict';

//ReactDOM.render(React.createElement(SlideruleApplication, null), document.getElementById('root'));

