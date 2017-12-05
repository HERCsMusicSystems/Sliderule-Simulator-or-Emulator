
var circleAreaLessons = {};

circleAreaLessons ['Area from radius'] = function (message) {
  var r = 0;
  while (r <= 1 || r > 10) r = 1 + Math . floor (Math . random () * 11);
  var area = crnu (Math . PI * r * r, 5);
  return [
    {action: function () {ensureSide (['A', 'B', 'D']); isolate (['A', 'B', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: r = " + r + ", find the area of the circle.");}, delay: 500}
  ];
};

slideruleLessons . push (circleAreaLessons);
