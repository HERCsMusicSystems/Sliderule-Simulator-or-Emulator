
var userManual = {};

userManual ['Moving Cursor'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  return [
    {action: function () {ensureSide (["A"]);}, delay: 0},
    {action: function () {message ("The task: move cursor to " + a + " on the A scale.");}, delay: 100},
    {action: function () {message ("Drag the cursor window until the centre hairline is over " + a + " on the A scale.");}, delay: 2000},
    {action: function () {cursorTo ("A", a);}, delay: 1000},
    {action: function () {cursorTo ("A", 1);
    sliderules . objective = function () {
      if (checkValue ("A", a)) {message ("Mission accomplished!"); increaseCookieResult ('Moving Cursor'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Now try these instruction again.");}, dealy: 4000}
  ];
};

userManual ['Moving Slide'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  return [
    {action: function () {ensureSide (["A", "B"]);}, delay: 0},
    {action: function () {message ("The task: move slide to put " + a + " on the B scale under cursor's hairline.");}, delay: 100},
    {action: function () {message ("Let's say the cursor is currently located at 10.");}, delay: 1000},
    {action: function () {cursorTo ("A", 10);}, delay: 1000},
    {action: function () {message ("Drag the slide until " + a + " on the B scale is under the cursor's hairline.");}, delay: 3000},
    {action: function () {slideTo ("B", a);}, delay: 2000},
    {action: function () {slideTo ("B", 10);
    sliderules . objective = function () {
      if (checkValue ("B", a)) {message ("Mission accomplished!"); increaseCookieResult ('Moving Cursor'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Now try these instructions again.");}, delay: 4000}
  ];
};

userManual ['Moving Cursor by right-click'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  return [
    {action: function () {ensureSide (["A"]);}, delay: 0},
    {action: function () {message ("The task: move cursor to " + a + " on the A scale.");}, delay: 100},
    {action: function () {message ("Right-click on the cursor window over the A scale and enter " + a + ".");}, delay: 2000},
    {action: function () {cursorTo ("A", a);}, delay: 1000},
    {action: function () {cursorTo ("A", 1);
    sliderules . objective = function () {
      if (checkValue ("A", a)) {message ("Mission accomplished!"); increaseCookieResult ('Moving Cursor by right-click'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Now try these instruction again.");}, dealy: 4000}
  ];
};

userManual ['Moving Slide by right-click'] = function (message) {
  var a = 1;
  while (a == 1 || a > 100) a = 1 + Math . floor (Math . random () * 100);
  return [
    {action: function () {ensureSide (["A", "B"]);}, delay: 0},
    {action: function () {message ("The task: move slide to put " + a + " on the B scale under cursor's hairline.");}, delay: 100},
    {action: function () {message ("Let's say the cursor is currently located at 10.");}, delay: 1000},
    {action: function () {cursorTo ("A", 10);}, delay: 1000},
    {action: function () {message ("Right-click on the A scale outside of the cursor window and enter " + a + ".");}, delay: 3000},
    {action: function () {slideTo ("B", a);}, delay: 2000},
    {action: function () {slideTo ("B", 10);
    sliderules . objective = function () {
      if (checkValue ("B", a)) {message ("Mission accomplished!"); increaseCookieResult ('Moving Slide by right-click'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Now try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (userManual);
