
import studio
import prcgtk

program sliderule #machine := "sliderule"
		[
			sliderule
			stator slide name length location margin
			background_colour rule_colour border_colour
			scale
			view
			sr
		]

#machine sliderule := "sliderule"

end := [[auto_atoms]
			[sliderule sr]
			[sr name "sonda"]
			[sr slide]
			[sr rule_colour 1.0 1.0 0.0]
			[sr margin 0.01 0.7]
			[sr slide]
			[sr border_colour 1.0 1.0 0.0]
			[sr slide]
			[sr scale]
			[sr length 400]
			[sr background_colour 1.0 0.0 0.0]
			[sr location 100.5 100.5]
			[sr view]
		[gtk_command]] .
