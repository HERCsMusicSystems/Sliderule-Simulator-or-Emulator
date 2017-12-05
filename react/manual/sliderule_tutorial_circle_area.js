
var circleAreaLessons = {};

circleAreaLessons ['Area from radius'] = function (message) {
  var r = 0;
  while (r <= 1 || r > 5.6) r = crnu (1 + Math . floor (Math . random () * 111) / 10);
  var area = crnu (Math . PI * r * r, 5);
  return [
    {action: function () {ensureSide (['A', 'B', 'D']); isolate (['A', 'B', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: r = " + r + ", find the area of the circle.");}, delay: 500},
    {action: function () {message ("Move the cursor to " + r + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', r);}, delay: 1500},
    {action: function () {message ("Align index (1) on the B scale with the cursor.");}, delay: 3000},
    {action: function () {slideTo ('B', 1);}, delay: 1500},
    {action: function () {message ("Move the cursor to \u03c0 on the B scale.");}, delay: 1500},
    {action: function () {cursorTo ('B', Math . PI);}, delay: 1500},
    {action: function () {message ("Read the area = " + area + " on the A scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); slideTo ('B', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('A', area) && checkValue ('B', Math . PI)) {
        message ("Mission accomplished!");
        increaseCookieResult ('Area from radius');
        return true;
      }
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

slideruleLessons . push (circleAreaLessons);
