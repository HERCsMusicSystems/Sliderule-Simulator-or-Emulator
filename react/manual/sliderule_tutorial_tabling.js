
var tablingLessons = {};

tablingLessons ['Staggering calculations'] = function (message) {
  var a = 0, b = 0, c = 0, d = 0, abc = 0;
  while (a < 1 || a > 10 || b < 0 || b > 10 || c < 1 || c > 10 || d < 1 || d > 10 || a <= c || abc > 10 || abc < d) {
    a = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    b = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    c = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    d = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    abc = a * b / c;
  }
  var result = abc / d;
  return [
  {action: function () {ensureSide (['C', 'D']); isolate (['C', 'D']); dimmm (255, 40, 8); changeMarkings ('hairline', true);}, delay: 100},
  {action: function () {message ("The task: calculate (" + a + " * " + b + ") / (" + c + " * " + d + ")");}, delay: 500},
  {action: function () {message ("Division first: " + a + " / " + c);}, delay: 2000},
  {action: function () {message ("Move cursor to " + a + " on the D scale.");}, delay: 2000},
  {action: function () {cursorTo ('D', a);}, delay: 1000},
  {action: function () {message ("Align " + c + " on the C scale with the cursor.");}, delay: 4000},
  {action: function () {slideTo ('C', c);}, delay: 1000},
  {action: function () {message ("The sub-result of " + crnu (a / c, 5) + " can be read on the D scale under C1.");}, delay: 4000},
  {action: function () {message ("It can now be easily multiplied by " + b + " by moving the cursor to " + b + " on the C scale.");}, delay: 2000},
  {action: function () {cursorTo ('C', b);}, delay: 1000},
  {action: function () {message ("You can now read the result of " + a + " * " + b + " / " + c + " = " + crnu (abc, 5) + " on the D scale.");}, delay: 4000},
  {action: function () {message ("It can now be easily divided by " + d + ".");}, delay: 2000},
  {action: function () {message ("Align " + d + " on the C scale with the cursor.");}, delay: 4000},
  {action: function () {slideTo ('C', d);}, delay: 1000},
  {action: function () {message ("Move the cursor to 1 on the C scale.");}, delay: 4000},
  {action: function () {cursorTo ('C', 1);}, delay: 1000},
  {action: function () {message ("Read the result " + crnu (result, 5) + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('C', 1) && checkValue ('D', result)) {
        message ("Mission accomplished! (" + a + " * " + b + ") / (" + c + " * " + d + ") = " + crnu (result, 5) + ".");
        increaseCookieResult ('Staggering calculations');
        return true;
      }
      return false;
    };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (tablingLessons);
