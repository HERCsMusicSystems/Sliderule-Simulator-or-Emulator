
config.o: config.h config.cpp setup_file_reader.h
	g++ -Wno-write-strings config.cpp -c -o config.o
setup_file_reader.o: setup_file_reader.h setup_file_reader.cpp
	g++ -Wno-write-strings setup_file_reader.cpp -c -o setup_file_reader.o
sliderule_main.o: sliderule_main.cpp config.h setup_file_reader.h lanczos.h scale_base.h
	g++ -Wno-write-strings sliderule_main.cpp -c -o sliderule_main.o `wx-config --cxxflags`
lanczos.o: lanczos.h lanczos.cpp
	g++ -Wno-write-strings lanczos.cpp -c -o lanczos.o `wx-config --cxxflags`
scale_base.o: scale_base.h scale_base.cpp lanczos.h
	g++ -Wno-write-strings scale_base.cpp -c -o scale_base.o `wx-config --cxxflags`
scale_log_base.o: scale_log_base.h scale_log_base.cpp scale_base.h lanczos.h
	g++ -Wno-write-strings scale_log_base.cpp -c -o scale_log_base.o `wx-config --cxxflags`
sliderule.o: sliderule.h sliderule.cpp lanczos.h scale_base.h
	g++ -Wno-write-strings sliderule.cpp -c -o sliderule.o `wx-config --cxxflags`

scales_decimal_tracking.o: scales_decimal_tracking.cpp lanczos.h scale_base.h setup_file_reader.h sliderule.h
	g++ -Wno-write-strings scales_decimal_tracking.cpp -c -o scales_decimal_tracking.o `wx-config --cxxflags`
scales_trig.o: scales_trig.cpp lanczos.h scale_base.h setup_file_reader.h sliderule.h
	g++ -Wno-write-strings scales_trig.cpp -c -o scales_trig.o `wx-config --cxxflags`

OBJECTS = sliderule_main.o setup_file_reader.o config.o lanczos.o scale_base.o scale_log_base.o sliderule.o \
	scales_decimal_tracking.o scales_trig.o

../sliderule: ${OBJECTS}
	g++ ${OBJECTS} -o ../sliderule `wx-config --libs`
linux: ../sliderule



osx: sliderule_main_dupa.cpp
	g++ sliderule_main_dupa.cpp -o ../sliderule.app/Contents/MacOS/sliderule `wx-config --cxxflags` `wx-config --libs`



