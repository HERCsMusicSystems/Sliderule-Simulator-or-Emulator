
var cookies = function () {
	var c = document . cookie . split (';');
  if (c == "") return {};
	var ret = {};
	var sub;
	for (var ind in c) {
    sub = c [ind] . split ('=');
    var value;
    try {
      value = JSON . parse (sub [1]);
    } catch (e) {value = sub [1];}
    ret [sub [0] . trim ()] = value;
  }
	return ret;
};

var storage = function () {
  var ret = {};
  for (var ind in window . localStorage) {
    var value = window . localStorage . getItem (ind);
    try {ret [ind] = JSON . parse (value);} catch (e) {ret [ind] = value;}
  }
  return ret;
};

//var cook = function (cookie, value) {document . cookie = cookie + " = " + JSON . stringify (value) + "; max-age=33554432";};
var cook = function (cookie, value) {
	if (value == undefined) {
		value = window . localStorage . getItem (cookie);
		if (value == undefined) return undefined;
		return JSON . parse (value);
	}
	window . localStorage . setItem (cookie, JSON . stringify (value));
};

/*var increaseCookieResult = function (cookie, selector, delta) {
  if (selector == undefined) selector = sliderules . name;
  if (selector == undefined) return;
  if (delta == undefined) delta = 1;
  var c = cookies ();
  if (c [cookie] == undefined) {
    var js = {};
    js [selector] = delta;
    document . cookie = cookie + " = " + JSON . stringify (js) + "; max-age=33554432";
    return;
  }
  var cjs = c [cookie];
  if (cjs [selector] == undefined) {
    cjs [selector] = delta;
    document . cookie = cookie + " = " + JSON . stringify (cjs) + "; max-age=33554432";
    return;
  }
  cjs [selector] += delta;
  document . cookie = cookie + " = " + JSON . stringify (cjs) + "; max-age=33554432";
};*/

var increaseCookieResult = function (cookie, selector, delta) {
  if (selector == undefined) selector = sliderules . name;
  if (selector == undefined) return;
  if (delta == undefined) delta = 1;
  var c = window . localStorage . getItem (cookie);
  if (c == undefined) {var js = {}; js [selector] = delta; window . localStorage . setItem (cookie, JSON . stringify (js)); return;}
  var cjs = JSON . parse (c);
  if (cjs [selector] == undefined) {cjs [selector] = delta; window . localStorage . setItem (cookie, JSON . stringify (cjs)); return;}
  cjs [selector] += delta;
  window . localStorage . setItem (cookie, JSON . stringify (cjs));
};

/*var cookieScore = function (cookie, selector) {
  var c = cookies ();
  if (c == undefined) return 0;
  var cs = c [cookie];
  if (cs == undefined) return 0;
  var score = 0;
  if (selector == undefined) {
    for (var ind in cs) score += cs [ind];
    return score;
  }
  cs = cs [selector];
  return cs == undefined ? 0 : cs;
};*/

var cookieScore = function (cookie, selector) {
  var c = window . localStorage . getItem (cookie);
  if (c == undefined) return 0;
  var cs = JSON . parse (c);
  if (cs == undefined) return 0;
  if (selector == undefined) {
    var score = 0;
    for (var ind in cs) score += cs [ind];
    return score;
  }
  cs = cs [selector];
  return cs == undefined ? 0 : cs;
};

var checkCookieScores = function (trials, value, artefact) {
  for (var sub in trials) {
    for (var ind in trials [sub]) if (cookieScore (ind, artefact) < value) return false;
  }
  return true;
};

var cookieMission = function (trials, value, artefact) {
  var ret = [];
  for (var sub in trials) {
    for (var ind in trials [sub]) {
      var score = value - cookieScore (ind, artefact);
      if (score > 0) ret . push (ind + " requires " + score + (score > 1 ? " more passes." : " more pass."));
    }
  }
  if (ret . length == 0) ret . push ("All trials passed.");
  return ret;
};

//var removeCookie = function (cookie) {document . cookie = cookie + "=;max-age=0";}
var removeCookie = function (cookie) {window . localStorage . removeItem (cookie);};
/*var removeAllCookies = function () {
  var c = cookies ();
  for (var ind in c) removeCookie (ind);
};*/
var removeAllCookies = function () {window . localStorage . clear ();};

var cookiesToStorage = function () {
  var c = cookies ();
  for (var ind in c) window . localStorage . setItem (ind, JSON . stringify (c [ind]));
};

var artefacts = function (cookie) {
  if (cookie == undefined) cookie = 'artefacts';
  var artefacts = window . localStorage . getItem (cookie);
  if (artefacts == undefined) return [];
  artefacts = JSON . parse (artefacts);
  return artefacts;
};

var addArtefact = function (artefact, cookie) {
  if (cookie == undefined) cookie = 'artefacts';
  var artefacts = window . localStorage . getItem (cookie);
  if (artefacts == undefined) artefacts = [];
  else artefacts = JSON . parse (artefacts);
  if (artefacts . indexOf (artefact) >= 0) return;
  artefacts . push (artefact);
  cook (cookie, artefacts);
};

var removeArtefact = function (artefact, cookie) {
  if (cookie == undefined) cookie = 'artefacts';
  if (artefact == undefined) {cook (cookie, []); return;}
  var artefacts = window . localStorage . getItem (cookie);
  if (artefacts == undefined) {cook (cookie, []); return;}
  artefacts = JSON . parse (artefacts);
  var ind = artefacts . indexOf (artefact);
  if (ind < 0) return;
  artefacts . splice (ind, 1);
  cook (cookie, artefacts);
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
            scale . alt = intensify_colour (scale . alt, intensity, colours);
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

var readLocation = function (name, value) {
  for (var sr in sliderules . sliderules) {
    var sliderule = sliderules . sliderules [sr];
    if (! sliderules . inactive) {
      for (var r in sliderule . rules) {
        var rule = sliderule . rules [r];
        for (var s in rule . scales) {
          var scale = rule . scales [s];
          if (scale . left == name) {
            if (value == undefined) return sliderule . cursor_target - rule .target;
            return scale . location (value);
          }
        }
      }
    }
  }
};

var checkValue = function (name, value, tolerance) {
  if (tolerance == undefined) tolerance = 0.0005;
  return Number (Math . abs (readLocation (name, value) - readLocation (name))) < tolerance;
}

var sequencerTimeout = null;
var sequencer = function (steps, index) {
  if (! steps) return;
  if (index == undefined) {sequencerTimeout = setTimeout (function () {sequencer (steps, 0);}, steps [0] . delay); return;}
  if (index >= steps . length) return;
  steps [index] . action ();
  index += 1;
  if (index >= steps . length) return;
  sequencerTimeout = setTimeout (function () {sequencer (steps, index);}, steps [index] . delay);
};

var slideruleLessons = [];
var readSlideruleLessons = function (id, lessons) {
	if (id == undefined) id = 'lessons';
	if (lessons == undefined) lessons = slideruleLessons;
	for (var ind in lessons) {
		for (var sub in lessons [ind]) {
			var option = document . createElement ('option');
			option . text = sub;
			document . getElementById (id) . add (option);
		}
	}
};
var lessonMessage = function (info) {
	if (info == "") {document . getElementById ('info') . innerHTML = ""; return;}
	document . getElementById ('info') . innerHTML += info + "<br />";
};
var playLesson = function (lessons, info) {
	clearTimeout (sequencerTimeout);
	if (lessons == undefined) lessons = 'lessons';
	if (info == undefined) info = 'info';
	document . getElementById (info) . innerHTML = "";
	var lesson_id = document . getElementById (lessons) . value;
	for (var ind in slideruleLessons) {
		var lesson = slideruleLessons [ind] [lesson_id];
		if (lesson != null) {sequencer (lesson (lessonMessage)); return;}
	}
	alert ("Scenario [" + lesson_id + "] not found.");
};


