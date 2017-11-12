
var cookies = function () {
	var c = document . cookie . split (';');
	var ret = {};
	var sub;
	for (var ind in c) {sub = c [ind] . split ('='); ret [sub [0] . trim ()] = sub [1];}
	return ret;
};

var dimm_scales = function (sliderule, rules) {
	for (var ind in rules) {
		for (var sub in rules [ind] . scales) {
			sliderules . sliderules [sliderule] . rules [rules [ind] . rule] . scales [rules [ind] . scales [sub]] . dimm = true;
		}
	}
	sliderules . requireRedraw = true;
};

var undimm_scales = function () {
	for (var s in sliderules . sliderules)
		for (var r in sliderules . sliderules [s] . rules)
			for (var c in sliderules . sliderules [s] . rules [r] . scales)
				delete sliderules . sliderules [s] . rules [r] . scales [c] . dimm;
	sliderules . requireRedraw = true;
};

