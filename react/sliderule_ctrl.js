
var cookies = function () {
	var c = document . cookie . split (';');
	var ret = {};
	var sub;
	for (var ind in c) {sub = c [ind] . split ('='); ret [sub [0] . trim ()] = sub [1];}
	return ret;
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

var cursorTo = function (name, value) {
  for (var sr in sliderules . sliderules) {
    var sliderule = sliderules . sliderules [sr];
    if (! sliderule . inactive) {
      for (var r in sliderule . rules) {
        var rule = sliderule . rules [r];
        for (var s in rule . scales) {
          var scale = rule . scales [s];
          if (scale . left == name) {
            var location = scale . location (value);
            sliderule . cursor_target = location;
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
              var location = scale . location (value);
              rule . target = - location + sliderule . cursor_target;
              return;
            }
          }
        }
      }
    }
  }
};
