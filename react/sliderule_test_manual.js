
var userTrial = {};

userTrial ['Moving Cursor test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  message ("The task: move cursor to " + a + " on the A scale.");
  sliderules . objective = function () {
    if (checkValue ("A", a)) {message ("Mission accomplished!"); increaseCookieResult ('trial cursor'); return true;}
    return false;
  };
  return null;
};

userTrial ['Moving Slide test'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  cursorTo ("A", 10);
  message ("The task: move slide to put " + a + " on the B scale under cursor's hairline.");
  sliderules . objective = function () {
    if (checkValue ("B", a)) {message ("Mission accomplished!"); increaseCookieResult ('trial slide'); return true;}
    return false;
  };
  return null;
};

slideruleLessons . push (userTrial);
