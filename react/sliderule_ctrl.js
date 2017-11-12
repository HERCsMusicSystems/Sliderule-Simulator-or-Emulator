
var cookies = function () {
	var c = document . cookie . split (';');
	var ret = {};
	var sub;
	for (var ind in c) {sub = c [ind] . split ('='); ret [sub [0] . trim ()] = sub [1];}
	return ret;
};

