
import studio
import prcgtk

program sliderule #machine := "sliderule"
		[
			sliderule
			stator slide name length
			background_colour rule_colour border_colour
			view
			sr
		]

#machine sliderule := "sliderule"

end := [[auto_atoms]
			[sliderule sr]
			[sr name "sonda"]
			[sr slide]
			[sr rule_colour 1.0 1.0 0.0]
			[sr slide]
			[sr border_colour 1.0 1.0 0.0]
			[sr slide]
			[sr view]
			[sr length 400]
			[sr background_colour 1.0 0.0 0.0]
		[gtk_command]] .
