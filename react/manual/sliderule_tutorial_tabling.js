
var tablingLessons = {};

tablingLessons ['Staggering calculations'] = function (message) {
  var a = 0, b = 0, c = 0, d = 0, abc = 0;
  while (a <= 1 || a >= 10 || b <= 1 || b >= 10 || c <= 1 || c >= 10 || d <= 1 || d >= 10 || a <= c || abc > 10 || abc < d) {
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

tablingLessons ['Compounded multiplication'] = function (message) {
  var a = 0, b = 0, c = 0, abc = 0;
  while (a <= 1 || b <= 1 || c <= 1 || a >= 10 || b >= 10 || c >= 10 || abc < 10 || abc > 100) {
    a = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    b = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    c = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    abc = crnu (a * b * c);
  }
  return [
  {action: function () {ensureSide (['C', 'CI', 'D']); isolate (['C', 'CI', 'D']); dimmm (255, 40, 8); changeMarkings ('hairline', true);}, delay: 100},
  {action: function () {message ("The task: multiply " + a + " * " + b + " * " + c);}, delay: 500},
  {action: function () {message ("Move the cursor to " + a + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ('D', a);}, delay: 1000},
  {action: function () {message ("Align " + b + " on the CI scale with the cursor.");}, delay: 4000},
  {action: function () {slideTo ('CI', b);}, delay: 1000},
  {action: function () {message ("Now move cursor to " + c + " on the C scale.");}, delay: 4000},
  {action: function () {cursorTo ('C', c);}, delay: 1000},
  {action: function () {message ("Read the value of " + crnu (abc / 10) + " on the D scale.");}, delay: 4000},
  {action: function () {message ("Adjust the decimal point to get the result " + a + " * " + b + " * " + c + " = " + crnu (abc));}, delay: 2000},
  {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if ((checkValue ('C', c) || checkValue ('C', b) || checkValue ('C', a)) && checkValue ('D', abc / 10)) {
      message ("Mission accomplished! " + a + " * " + b + " * " + c + " = " + abc);
      increaseCookieResult ('Compounded multiplication');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

/*
roman denarius = 16 roman assarion
roman denarius = 64 roman quadrans
roman denarius = 128 roman lepton
roman denarius = 0.0001125730994152 hebrew talent
roman denarius = 0.0067543859649123 hebrew mina
roman denarius = 0.33771929824561 hebrew shekel
roman denarius = 6.7543859649123 hebrew gerah
roman denarius = 0.00018872549019608 greek talent
roman denarius = 0.011323529411765 greek mina
roman denarius = 0.28308823529412 greek tetradrachma
roman denarius = 0.56617647058824 greek didrachma
roman denarius = 1.13235294117648 greek drachma
*/

tablingLessons ['Denarius to drachma conversion'] = function (message) {
  var conversion_rate = crnu (1.1323529411765, 5);
  var denarius = 2 + Math . floor (Math . random () * 7);
  var drachma = crnu (denarius * conversion_rate, 5);
  return [
  {action: function () {ensureSide (['C', 'D', 'CF', 'DF']); isolate (['C', 'D', 'CF', 'DF']); dimmm (255, 40, 8); changeMarkings ('hairline', true);}, delay: 100},
  {action: function () {message ("The task: if one roman denarius = " + conversion_rate + " greek drachmae, then how much drachmas you can buy for " + denarius + " denarii?");}, delay: 500},
  {action: function () {message ("Move the cursor to " + conversion_rate + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ('D', conversion_rate);}, delay: 1000},
  {action: function () {message ("Align 1 on the C scale with the cursor.");}, delay: 3000},
  {action: function () {slideTo ('C', 1);}, delay: 1000},
  {action: function () {message ("Move the cursor to " + denarius + " on the C scale.");}, delay: 3000},
  {action: function () {cursorTo ('C', denarius);}, delay: 1000},
  {action: function () {message ("Read the result on the D scale: " + denarius + " roman denarii buys " + drachma + " greek drachmae.");}, delay: 3000},
  {action: function () {message ("Now observe that this setting allows you to convert immediately denarii to drachmae and drachmae to denari.");}, delay: 3000},
  {action: function () {message ("Also, scales CF and DF are automatically aligned for the same conversion rate.");}, delay: 1000},
  {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ('C', denarius) && checkValue ('D', drachma)) {
      message ("Mission accomplished! " + denarius + " roman denarii buys " + drachma + " greek drachmae.");
      increaseCookieResult ('Denarius to drachma conversion');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (tablingLessons);
