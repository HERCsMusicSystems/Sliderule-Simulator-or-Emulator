
sliderule_prc.o: sliderule.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary_sliderule_prc_start=resource_sliderule\
		sliderule.prc sliderule_prc.o
	objdump -t sliderule_prc.o

sliderule: /usr/lib/sliderule.so

graphics2d.o: ../../prolog/code/prolog/graphics2d.h ../../prolog/code/prolog/graphics2d.cpp
	g++ -Wno-write-strings ../../prolog/code/prolog/graphics2d.cpp -c -o graphics2d.o -fPIC

SlideRule.o: ../../prolog/code/prolog/graphics2d.h SlideRule.h SlideRule.cpp
	g++ -Wno-write-strings -I../../prolog/code/prolog SlideRule.cpp -c -o SlideRule.o -fPIC `pkg-config --cflags gtk+-2.0`

SlideRuleMain.o: ../../prolog/code/prolog/graphics2d.h SlideRule.h SlideRuleMain.cpp
	g++ -Wno-write-strings -I../../prolog/code/prolog SlideRuleMain.cpp -c -o SlideRuleMain.o -fPIC `pkg-config --cflags gtk+-2.0`

/usr/lib/sliderule.so: sliderule_prc.o graphics2d.o SlideRule.o SlideRuleMain.o ~/bin/studio
	g++ -Wno-write-strings -I../../prolog/code/prolog \
		sliderule_prc.o graphics2d.o SlideRule.o SlideRuleMain.o \
		-o /usr/lib/sliderule.so -shared -fPIC `pkg-config --libs gtk+-2.0`


