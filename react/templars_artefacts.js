
var updateMissions = function (trial, trials) {
	if (trials == undefined) trials = 2;
	document . getElementById ('mission') . innerHTML = cookieMission (trial, trials, sliderules . name) . join ('<br />');
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

var updateSeeker = function () {
	var a = artefacts ();
	var el = document . getElementById ('seeker');
	switch (a . length) {
	case 0: case 1: el . innerHTML = "The poor seeker of ancient artefacts."; break;
	case 2: el . innerHTML = "Ancient artefact's Entered Apprentice."; break;
	case 3: el . innerHTML = "Ancient artefact Fellow."; break;
	case 4: el . innerHTML = "Ancient artefact Master."; break;
	default: el . innerHTML = "Temple Builder."; break;
	}
};

