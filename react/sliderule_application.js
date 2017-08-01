
var changeMarkings = function (type, value) {
	for (var ind in sliderules . sliderules) {
		switch (type) {
		case 'static': case 'stator': case 'rule': sliderules . sliderules [ind] . static_markings = value; break;
		case 'cursor': case 'hairline': sliderules . sliderules [ind] . cursor_markings = value; break;
		case 'cursors': case 'hairlines': case 'extras': sliderules . sliderules [ind] . extra_cursor_markings = value; break;
		default: break;
		}
	}
	sliderules . requireRedraw = true;
};

var SlideruleApplication = React . createClass ({
  dragging: false,
  mousePosition: {x: 0, y: 0},
  onContext: function (event) {event . preventDefault ();},
  onMouseDown: function (event) {
    var position = subv ({x: event . clientX, y: event . clientY}, this . refs . sliderule . getBoundingClientRect ());
    if (event . button === 2) return sliderules . synchroniseTarget (addv (position, {x: 0.5, y: 0}));
    this . dragging = true;
    this . mousePosition = position;
  },
  onMouseMove: function (event) {
    if (! this . dragging) return;
    var position = subv ({x: event . clientX, y: event . clientY}, this . refs . sliderule . getBoundingClientRect ());
    var delta = subv (position, this . mousePosition);
    sliderules . synchroniseMove (delta, position);
    this . mousePosition = position;
  },
  onMouseUp: function (event) {this . dragging = false; this . setState ({});},
  SM: function (event) {changeMarkings ('stator', event . target . checked);},
  CM: function (event) {changeMarkings ('hairline', event . target . checked);},
  EM: function (event) {changeMarkings ('hairlines', event . target . checked);},
  draw: function () {
  	if (sliderules . noChange ()) return;
    var width = window . innerWidth; var height = window . innerHeight;
    var bound = this . refs . sliderule . getBoundingClientRect ();
    this . setState ({width: width - bound . x * 4, height: height - bound . y * 4});
    //this . setState ({width: width, height: height});
    var ctx = this . refs . sliderule . getContext ('2d');
    sliderules . draw (ctx, width, height);
  },
  componentDidMount: function () {setInterval (this . draw, 20);},
  getInitialState: function () {return {width: 200, height: 100, dragging: false};},
  render: function render() {
    return React.createElement(
      'div',
      { style: { align: 'center' } }, [//, position: 'relative' } }, [
      React.createElement('canvas', { key: 1, ref: 'sliderule', width: this.state.width, height: this.state.height, style: { display: 'block' },
        onMouseDown: this.onMouseDown,
        onMouseMove: this.onMouseMove,
        onMouseUp: this.onMouseUp,
        onMouseLeave: this.onMouseUp,
        onContextMenu: this.onContext
      })/*,
      React . createElement (
      	'div',
      	{ key: 2, style: {position: 'absolute', left: '0px', top: '0px'}}, [
		  React . createElement ('input', {key: 3, type: 'checkbox', onChange: this . SM}),
		  	'Markings on stator\u2003\u2003',
		  React . createElement ('input', {key: 4, type: 'checkbox', onChange: this . CM}),
		  	'Markings on hairline\u2003\u2003',
		  React . createElement ('input', {key: 5, type: 'checkbox', onChange: this . EM}),
		  	'Markings on additional hairlines\u2003\u2003',
		  ])*/
      ]
    );
  }
});
