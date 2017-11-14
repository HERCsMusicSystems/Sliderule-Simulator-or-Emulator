
var multiplicationLessons = {};

multiplicationLessons ['Simple Multiplication (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9]);
  return [
    {action: function () {ensureSide (["A", "B"]);}, delay: 100},
    {action: function () {message ("The task : multiply " + a + " by " + b + " using A/B scales.");}, delay: 1000},
    {action: function () {isolate (["A", "B"]); changeMarkings ('hairline', true);}, delay: 2000},
    {action: function () {message ("Move cursor to " + a + " on A scale.");}, delay: 2000},
    {action: function () {cursorTo ("A", a);}, delay: 2000},
    {action: function () {message ("Now align 1 on B scale with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("B", 1);}, delay: 2000},
    {action: function () {message (".... then move the cursor to " + b + " on B scale.");}, delay: 4000},
    {action: function () {cursorTo ("B", b);}, delay: 1000},
    {action: function () {message ("Then read the result " + (a * b) + " on the A scale.");}, delay: 4000},
    {action: function () {cursorTo ("A", 1); slideTo ("B", 1);}, delay: 6000},
    {action: function () {isolate (); changeMarkings ('hairline', false);}, delay: 2000}
  ]
};

multiplicationLessons ['Simple Multiplication (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a * b > 10) {a = Math . floor (10 + Math . random () * 90) / 10; b = Math . floor (10 + Math . random () * 90) / 10;}
  return [
    {action: function () {ensureSide (["C", "D"]);}, delay: 100},
    {action: function () {message ("The task: multiply " + a + " by " + b + " using C/D scales.");}, delay: 1000},
    {action: function () {isolate (["C", "D"]); changeMarkings ('hairline', true);}, delay: 2000},
    {action: function () {message ("Move cursor to " + a + " on D scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", a);}, delay: 2000},
    {action: function () {message ("Now align 1 on C scale with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("C", 1);}, delay: 2000},
    {action: function () {message (".... then move the cursor to " + b + " on C scale.");}, delay: 4000},
    {action: function () {cursorTo ("C", b);}, delay: 1000},
    {action: function () {message ("Then read the result " + Number (a * b) . toFixed (2) + " on the D scale.");}, delay: 4000},
    {action: function () {cursorTo ("D", 1); slideTo ("C", 1);}, delay: 6000},
    {action: function () {isolate (); changeMarkings ('hairline', false);}, delay: 2000}
  ]
};

for (var ind in multiplicationLessons) {
  var option = document . createElement ('option');
  option . text = ind;
  document . getElementById ('lessons') . add (option);
}