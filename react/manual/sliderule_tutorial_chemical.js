
var chemicalLessons = {};

chemicalLessons ['Element mass'] = function (message) {
  var element;
  while (! element || element_mass [element . element] < 10 || element_mass [element . element] > 100) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  var masss = crnu (mass / 10);
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + element . element + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', masss);}, delay: 1500},
    {action: function () {message ("Read " + masss + " on the D scale.");}, delay: 3000},
    {action: function () {message ("Adjust decimal point to obtain the molecular mass of " + element . name + " = " + mass + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', mass)) {message ("Mission accomplished!"); increaseCookieResult ('Light element mass'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

chemicalLessons ['Light element mass'] = function (message) {
  var element;
  while (! element || element_mass [element . element] > 10) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + element . element + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', mass);}, delay: 1500},
    {action: function () {message ("Read the molecular mass of " + element . name + " = " + mass + ".");}, delay: 3000},
    {action: function () {message ("Since " + element . name + " is a light element, as indicated by underscore....");}, delay: 3000},
    {action: function () {message (".... you do not need to adjust the decimal point.");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', mass)) {message ("Mission accomplished!"); increaseCookieResult ('Light element mass'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

slideruleLessons . push (chemicalLessons);
