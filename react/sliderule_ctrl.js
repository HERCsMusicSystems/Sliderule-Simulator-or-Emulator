
var cookies = function () {
	var c = document . cookie . split (';');
  if (c == "") return {};
	var ret = {};
	var sub;
	for (var ind in c) {sub = c [ind] . split ('='); ret [sub [0] . trim ()] = sub [1];}
	return ret;
};

var increaseCookieResult = function (cookie, delta) {
  if (delta == undefined) delta = 1;
  var c = cookies ();
  if (c [cookie] == undefined) {document . cookie = cookie + " = " + delta + "; max-age=33554432"; return;}
  document . cookie = cookie + " = " + (Number (c [cookie]) + delta) + "; max-age=33554432";
};

var removeCookie = function (cookie) {document . cookie = cookie + "=;max-age=0";}
var removeAllCookies = function () {
  var c = cookies ();
  for (var ind in c) removeCookie (ind);
};

var isolate = function (scales) {
  if (scales == undefined) {
    for (var ss in sliderules . sliderules) {
      for (var r in sliderules . sliderules [ss] . rules) {
        for (var s in sliderules . sliderules [ss] . rules [r] . scales) {
          delete sliderules . sliderules [ss] . rules [r] . scales [s] . dimm;
        }
      }
    }
    sliderules . requireRedraw = true;
    return;
  }
  for (var ss in sliderules . sliderules) {
    for (var r in sliderules . sliderules [ss] . rules) {
      for (var s in sliderules . sliderules [ss] . rules [r] . scales) {
        var scale = sliderules . sliderules [ss] . rules [r] . scales [s];
        if (scales . indexOf (scale . left) < 0) scale . dimm = true;
      }
    }
  }
  sliderules . requireRedraw = true;
};
var intensify_colour = function (colour, intensity, colours) {
  if (colour == 'black') colour = '#000000';
  if (colour == 'red') colour = '#ff0000';
  if (colour . charAt (0) == '#') colour = colour . substring (0, 7);
  intensity = intensity . toString (16);
  if (intensity . length < 2) intensity = "0" + intensity;
  colour += intensity;
  switch (colour) {
  case "#000000ff": return 'black';
  case "#ff0000ff": return 'red';
  default: return colour; break;
  }
  return colour;
};
var dimmm = function (from, to, by, colours) {
  if (colours == undefined) colours = ['#000000', '#ff0000'];
  dimm (from, colours);
  by = Math . abs (by);
  if (from == to) return;
  if (from > to) from -= by;
  else if (from < to) from += by;
  if (Math . abs (to - from) < by) from = to;
  setTimeout (function () {dimmm (from, to, by, colours);}, 100);
};
var dimm = function (intensity, colours) {
  for (var esc in sliderules . sliderules) {
    //if (! sliderules . sliderules [esc] . inactive) {
      for (var ind in sliderules . sliderules [esc] . rules) {
        for (var sub in sliderules . sliderules [esc] . rules [ind] . scales) {
          var scale = sliderules . sliderules [esc] . rules [ind] . scales [sub];
          if (scale . dimm) {
            scale . colour = intensify_colour (scale . colour, intensity, colours);
            scale . colour = intensify_colour (scale . colour, intensity, colours);
            scale . alt = '#ff0000' + intensity;
            if (scale . marking_colour != undefined) scale . marking_colour = intensify_colour (scale . marking_colour, intensity, colours);
          }
        }
      }
    //}
  }
  sliderules . requireRedraw = true;
};

var hasScale = function (sliderule, name) {
  for (var rule in sliderule . rules) {
    for (var scale in sliderule . rules [rule] . scales) {
      if (sliderule . rules [rule] . scales [scale] . left == name) return true;
    }
  }
  return false;
};

var hasScales = function (sliderule, names) {
  for (var ind in names) {if (! hasScale (sliderule, names [ind])) return false;}
  return true;
};

var ensureSide = function (names) {
  var ind;
  for (ind in sliderules . sliderules) sliderules . sliderules [ind] . inactive = true;
  for (ind in sliderules . sliderules) {
    if (hasScales (sliderules . sliderules [ind], names)) {
      sliderules . sliderules [ind] . inactive = false;
      sliderules . requireRedraw = true;
      return;
    }
  }
};

var cursorTo = function (name, value) {
  for (var sr in sliderules . sliderules) {
    var sliderule = sliderules . sliderules [sr];
    if (! sliderule . inactive) {
      for (var r in sliderule . rules) {
        var rule = sliderule . rules [r];
        for (var s in rule . scales) {
          var scale = rule . scales [s];
          if (scale . left == name) {
            var target = scale . location (value) + rule . target;
            for (var tss in sliderules . sliderules) sliderules . sliderules [tss] . cursor_target = target;
            return;
          }
        }
      }
    }
  }
};

var slideTo = function (name, value) {
  for (var sr in sliderules . sliderules) {
    var sliderule = sliderules . sliderules [sr];
    if (! sliderule . inactive) {
      for (var r in sliderule . rules) {
        var rule = sliderule . rules [r];
        if (rule . stator != 0) {
          for (var s in rule . scales) {
            var scale = rule . scales [s];
            if (scale . left == name) {
              var target = sliderule . cursor_target - scale . location (value);
              for (var tss in sliderules . sliderules) {
                for (var tr in sliderules . sliderules [tss] . rules) {
                  if (sliderules . sliderules [tss] . rules [tr] . stator != 0) sliderules . sliderules [tss] . rules [tr] . target = target;
                }
              }
              return;
            }
          }
        }
      }
    }
  }
};

var readValue = function (name) {
  for (var sr in sliderules . sliderules) {
    var sliderule = sliderules . sliderules [sr];
    if (! sliderule . inactive) {
      for (var r in sliderule . rules) {
        var rule = sliderule . rules [r];
        for (var s in rule . scales) {
          var scale = rule . scales [s];
          if (scale . left == name) {
            return scale . value (sliderule . cursor_target - rule . target);
          }
        }
      }
    }
  }
  return null;
};

var checkValue = function (name, value, tolerance) {
  if (tolerance == undefined) tolerance = 2;
  return Number (Math . abs (value - readValue (name))) . toFixed (tolerance) == 0;
}

var sequencer = function (steps, index) {
  if (! steps) return;
  if (index == undefined) {setTimeout (function () {sequencer (steps, 0);}, steps [0] . delay); return;}
  if (index >= steps . length) return;
  steps [index] . action ();
  index += 1;
  if (index >= steps . length) return;
  setTimeout (function () {sequencer (steps, index);}, steps [index] . delay);
};

var slideruleLessons = [];
var readSlideruleLessons = function (id, lessons) {
	if (id == undefined) id = 'lessons';
	if (lessons == undefined) lessons = slideruleLessons;
	for (var ind in lessons) {
		for (var sub in lessons [ind]) {
			var option = document . createElement ('option');
			option . text = sub;
			document . getElementById ('lessons') . add (option);
		}
	}
};
var lessonMessage = function (info) {
	if (info == "") {document . getElementById ('info') . innerHTML = ""; return;}
	document . getElementById ('info') . innerHTML += info + "<br />";
};
var playLesson = function (lessons, info) {
	if (lessons == undefined) lessons = 'lessons';
	if (info == undefined) info = 'info';
	document . getElementById (info) . innerHTML = "";
	var lesson_id = document . getElementById (lessons) . value;
	for (var ind in slideruleLessons) {
		var lesson = slideruleLessons [ind] [lesson_id];
		if (lesson != null) {sequencer (lesson (lessonMessage)); return;}
	}
	alert ("Lesson not found.");
};


