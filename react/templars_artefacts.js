
var updateMissions = function () {
	document . getElementById ('mission') . innerHTML = cookieMission (userTrial, 2, sliderules . name) . join ('<br />');
};

var updateArtefacts = function () {
	var a = artefacts ();
	var content = [];
	for (var ind in a) {
		var aa = a [ind];
		var href = "<a href=\"" + aa . replace (" ", "_") . toLowerCase () + ".html\" >" + aa + "</a>";
		content . push (href);
	}
	if (content . length < 1) content . push ('none');
	document . getElementById ('artefacts') . innerHTML = content . join ('<br />');
};

