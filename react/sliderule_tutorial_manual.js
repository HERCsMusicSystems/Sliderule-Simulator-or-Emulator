
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
      if (checkValue ("A", a)) {message ("Mission accomplished!"); increaseCookieResult ('manual_cursor'); return true;}
      return false;
    }
    }, delay: 6000},
    {action: function () {message ("Now try these instruction again.");}, dealy: 4000}
  ];
};

slideruleLessons . push (userManual);
