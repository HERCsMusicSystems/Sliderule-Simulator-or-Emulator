
import studio
import prcgtk

program sliderule #machine := "sliderule"
		[
			sliderule
			stator slide name
			view
			sr
		]

#machine sliderule := "sliderule"

end := [[auto_atoms]
			[sliderule sr]
			[sr name "sonda"]
			[sr slide]
			[sr slide]
			[sr view]
		[gtk_command]] .
