
var pythagoreanLessons = {};

pythagoreanLessons ['Find b on the P scale'] = function (message) {
  var a = 0;
  while (a >= 10 || a < 2) a = Math . floor (Math . random () * 11);
  aa = crnu (a / 10);
  var b = crnu (Math . sqrt (1 - aa * aa), 5);
  return [
    {action: function () {ensureSide (['D', 'P']); isolate (['D', 'P']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: c = 1, a = " + aa + ", find b on the P scale.");}, delay: 500},
    {action: function () {message ("Move the cursor to " + a + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', a);}, delay: 1500},
    {action: function () {message ("Read b = " + b + " on the P scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('P', b)) {message ("Mission accomplished!"); increaseCookieResult ('Find b on the P scale'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Try these instructions again.");}, delay: 4000}
  ];
};

pythagoreanLessons ['Find b on the D scale'] = function (message) {
  var a = 0;
  while (a >= 10 || a < 2) a = Math . floor (Math . random () * 11);
  aa = crnu (a / 10);
  var b = crnu (Math . sqrt (1 - aa * aa), 5);
  var bb = crnu (Math . sqrt (1 - aa * aa) * 10, 5);
  return [
    {action: function () {ensureSide (['D', 'P']); isolate (['D', 'P']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: c = 1, a = " + aa + ", find b on the D scale.");}, delay: 500},
    {action: function () {message ("Move the cursor to " + aa + " on the P scale.");}, delay: 3000},
    {action: function () {cursorTo ('P', aa);}, delay: 1500},
    {action: function () {message ("Read " + bb + " on the D scale.");}, delay: 3000},
    {action: function () {message ("Adjust the decimal place to obtain b = " + b + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', bb)) {message ("Mission accomplished!"); increaseCookieResult ('Find b on the D scale'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {message ("Try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (pythagoreanLessons);
