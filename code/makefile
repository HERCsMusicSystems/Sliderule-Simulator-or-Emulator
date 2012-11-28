
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
scale_trig_base.o: scale_trig_base.h scale_trig_base.cpp scale_base.h lanczos.h
	g++ -Wno-write-strings scale_trig_base.cpp -c -o scale_trig_base.o `wx-config --cxxflags`
scale_statistics_base.o: scale_statistics_base.h scale_statistics_base.cpp scale_base.h lanczos.h
	g++ -Wno-write-strings scale_statistics_base.cpp -c -o scale_statistics_base.o `wx-config --cxxflags`
sliderule.o: sliderule.h sliderule.cpp lanczos.h scale_base.h
	g++ -Wno-write-strings sliderule.cpp -c -o sliderule.o `wx-config --cxxflags`

SCALE_DEPENDENCY =  lanczos.h scale_base.h scale_log_base.h scale_trig_base.h scale_statistics_base.h setup_file_reader.h sliderule.h
scales_log_lin.o: scales_log_lin.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_log_lin.cpp -c -o scales_log_lin.o `wx-config --cxxflags`
scales_decimal_tracking.o: scales_decimal_tracking.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_decimal_tracking.cpp -c -o scales_decimal_tracking.o `wx-config --cxxflags`
scales_trig.o: scales_trig.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_trig.cpp -c -o scales_trig.o `wx-config --cxxflags`
scales_trig_dec.o: scales_trig_dec.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_trig_dec.cpp -c -o scales_trig_dec.o `wx-config --cxxflags`
scales_trig_other.o: scales_trig_other.cpp ${SCALE_DEPENDENCEY}
	g++ -Wno-write-strings scales_trig_other.cpp -c -o scales_trig_other.o `wx-config --cxxflags`
scales_log_log.o: scales_log_log.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_log_log.cpp -c -o scales_log_log.o `wx-config --cxxflags`
scales_pythagorean.o: scales_pythagorean.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_pythagorean.cpp -c -o scales_pythagorean.o `wx-config --cxxflags`
scales_hyperbolic.o: scales_hyperbolic.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_hyperbolic.cpp -c -o scales_hyperbolic.o `wx-config --cxxflags`
scales_statistical.o: scales_statistical.cpp ${SCALE_DEPENDENCY}
	g++ -Wno-write-strings scales_statistical.cpp -c -o scales_statistical.o `wx-config --cxxflags`

OBJECTS = sliderule_main.o setup_file_reader.o config.o lanczos.o scale_base.o scale_log_base.o scale_trig_base.o scale_statistics_base.o sliderule.o \
	scales_log_lin.o scales_decimal_tracking.o scales_trig.o scales_trig_dec.o scales_trig_other.o \
	scales_log_log.o scales_pythagorean.o scales_hyperbolic.o scales_statistical.o
#	scales_decimal_tracking.o scales_trig.o

../sliderule: ${OBJECTS}
	g++ ${OBJECTS} -o ../sliderule `wx-config --libs`
linux: ../sliderule


../sliderule.app/Contents/MacOS/sliderule: ${OBJECTS}
	g++ ${OBJECTS} -o ../sliderule.app/Contents/MacOS/sliderule `wx-config --libs`

osx: ../sliderule.app/Contents/MacOS/sliderule
