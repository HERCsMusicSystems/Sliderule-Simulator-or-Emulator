
var userTrial = {};

userTrial ['Moving cursor test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  isolate ([]);
  message ("The task: move cursor to " + a + " on the A scale.");
  sliderules . objective = function () {
    if (checkValue ("A", a)) {
    	message ("Mission accomplished! The cursor is now at " + a + " on the A scale.");
    	increaseCookieResult ('Moving cursor test');
    	isolate ();
    	return true;
    }
    return false;
  };
  return null;
};

userTrial ['Moving slide test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  cursorTo ("A", 10);
  isolate ([]);
  message ("The task: move slide to put " + a + " on the B scale under cursor's hairline. Keep the cursor at 10 on the A scale at the same time.");
  sliderules . objective = function () {
    if (checkValue ("B", a) && checkValue ("A", 10)) {
    	message ("Mission accomplished! The cursor is now at " + a + " on the B scale and at 10 on the A scale.");
    	increaseCookieResult ('Moving slide test');
    	isolate ();
    	return true;
    }
    return false;
  };
  return null;
};

userTrial ['Right-click cursor test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  track = [];
  message ("The task: right-click on the cursor on the A scale to put " + a + " under cursor's hairline.");
  sliderules . objective = function () {
    if (track . length < 1) return false;
    var t = track [track . length - 1];
    if (t . value == a && t . source == 'cursor' && t . scale == 'A' && checkValue ('A', a)) {
    	message ("Mission accomplished! The cursor is now at " + a + " on the A scale.");
    	increaseCookieResult ('Right-click cursor test');
    	return true;
    }
    return false;
  };
  return null;
};

userTrial ['Right-click slide test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  cursorTo ("A", 10);
  track = [];
  message ("The task: right-click on the slide on the B scale to put " + a + " under cursor's hairline. Keep the cursor at 10 on the A scale at the same time.");
  sliderules . objective = function () {
    if (track . length < 1) return false;
    var t = track [track . length - 1];
    if (t . value == a && t . source == 'slide' && t . scale == 'B' && checkValue ('A', 10) && checkValue ('B', a)) {
    	message ("Mission accomplished! The cursor is now at " + a + " on the B scale and at 10 on the A scale.");
    	increaseCookieResult ('Right-click slide test');
    	return true;
    }
    return false;
  };
  return null;
};

slideruleLessons . push (userTrial);

