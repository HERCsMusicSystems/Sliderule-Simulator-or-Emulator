///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2010 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "setup_file_reader.h"
#include "sliderule.h"
#include "scale_statistics_base.h"

double blocations[5000];
double clocations[5000];
double cdlocations[2800];
double flocations[9000];
bool binitlocations = true;
bool cinitlocations = true;
bool cdinitlocations = true;
bool finitlocations = true;
double blastv1, blastv2, blastn, blastsl;
double clastv, clastn, clastsl;
double cdlasta, cdlastn, cdlastsl;
double flastv1, flastv2, flastn, flastsl;
class ScaleBetaDistUp : public StatisticsBase {
public:
	double v1, v2;
	double n;
	double locations[5000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) {return log10( InvBetaDist( v1, v2, x ) ) - n; }
	virtual double getValue (double x) {return BetaDist( v1, v2, pow(10, x + n ) ); }
	void init_locations (void) {
		if (binitlocations || (v1 != blastv1) || (v2 != blastv2) || (scale_length != blastsl)) {
			for (int ind = 0; ind <= 900; ind++) {
				blocations [ind] = (double) scale_length * (log10( InvBetaDist( v1, v2, 0.00001 + (double) ind / 10000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				blocations [ind + 900] = (double) scale_length * (log10( InvBetaDist( v1, v2, 0.0001 + (double) ind / 1000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				blocations [ind + 1800] = (double) scale_length * (log10( InvBetaDist( v1, v2, 0.001 + (double) ind / 100000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				blocations [ind + 2700] = (double) scale_length * (log10( InvBetaDist( v1, v2, 0.01 + (double) ind / 10000.0 )) - n);}
			for (int ind = 1; ind < 900; ind++) {
				blocations [ind + 3600] = (double) scale_length * (log10( InvBetaDist( v1, v2, 0.1 + (double) ind / 1000.0 )) - n);}
			blocations [4500] = (double) scale_length * (0.0 - n);
			blastn = n;
			blastv1 = v1;
			blastv2 = v2;
			blastsl = scale_length;
			binitlocations = false;
			for (int ind = 0; ind <= 4500; ind++)
				locations [ind] = blocations [ind]; }
		else {
			if (n != blastn) {
				for (int ind = 0; ind <= 4500; ind++)
					blocations [ind] += ((blastn - n)*scale_length);
				blastn = n; }
			for (int ind = 0; ind <= 4500; ind++)
				locations [ind] = blocations [ind]; }
		left_index = find_left_index (locations, 1, 4500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".9"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (left_index <= 4500) location = draw_markings_for_100 (dc, & locations [4400], _T ("1"), x, location, max(0, left_index - 4400), min(right_index - 4400, 100));
		if ( left_index <= 4500 ) draw_text_marker (dc, _T ("1"), x + locations [4500]);
	}
	ScaleBetaDistUp (double powerof10, double degoffreedom1, double degoffreedom2, int height) : StatisticsBase (height) {v1 = degoffreedom1; v2 = degoffreedom2; n = powerof10; left_index = 0; right_index = 4760;}
};
class ScaleBetaDistDown : public ScaleBetaDistUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleBetaDistDown (double powerof10, double degoffreedom1, double degoffreedom2, int height) : ScaleBetaDistUp (powerof10, degoffreedom1, degoffreedom2, height) {}
};
class ScaleFDistUp : public StatisticsBase {
public:
	double v1, v2, n; 
	double locations[9000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) { return log10( InvFDist( v1, v2, x ) ) - n; }
	virtual double getValue (double x) {return FDist( v1, v2, pow(10, x + n ) ); }
	void init_locations (void) {
		if (finitlocations || (v1 != flastv1) || (v2 != flastv2) || (scale_length != flastsl)) {
			for (int ind = 0; ind <= 900; ind++) {
				flocations [ind] = (double) scale_length * (log10( InvFDist( v1, v2, 0.00001 + (double) ind / 10000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 900] = (double) scale_length * (log10( InvFDist( v1, v2, 0.0001 + (double) ind / 1000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 1800] = (double) scale_length * (log10( InvFDist( v1, v2, 0.001 + (double) ind / 100000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 2700] = (double) scale_length * (log10( InvFDist( v1, v2, 0.01 + (double) ind / 10000.0 )) - n);}
			for (int ind = 1; ind <= 700; ind++) {
				flocations [ind + 3600] = (double) scale_length * (log10( InvFDist( v1, v2, 0.1 + (double) ind / 1000.0 )) - n);}
			for (int ind = 1; ind <= 1900; ind++) {
				flocations [ind + 4300] = (double) scale_length * (log10( InvFDist( v1, v2, 0.8 + (double) ind / 10000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 6200] = (double) scale_length * (log10( InvFDist( v1, v2, 0.99 + (double) ind / 100000.0 ) ) - n );}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 7100] = (double) scale_length * (log10( InvFDist( v1, v2, 0.999 + (double) ind / 1000000.0 ) ) - n );}
			for (int ind = 1; ind <= 900; ind++) {
				flocations [ind + 8000] = (double) scale_length * (log10( InvFDist( v1, v2, 0.9999 + (double) ind / 10000000.0 ) ) - n );}
			flastn = n;
			flastv1 = v1;
			flastv2 = v2;
			flastsl = scale_length;
			finitlocations = false;
			for (int ind = 0; ind <= 8900; ind++)
				locations [ind] = flocations [ind]; }
		else {
			if (n != flastn) {
				for (int ind = 0; ind <= 8900; ind++)
					flocations [ind] += ((flastn - n)*scale_length);
				flastn = n; }
			for (int ind = 0; ind <= 8900; ind++)
				locations [ind] = flocations [ind]; }
		left_index = find_left_index (flocations, 1, 8900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (flocations, 1, 8900, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = flocations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (( left_index <= 3650 ) && ( right_index >= 3650 )) draw_text_marker (dc, _T (".15"), x + locations [3650]);
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (( left_index <= 3750 ) && ( right_index >= 3750 )) draw_text_marker (dc, _T (".25"), x + locations [3750]);
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (( left_index <= 3850 ) && ( right_index >= 3850 )) draw_text_marker (dc, _T (".35"), x + locations [3850]);
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (( left_index <= 3950 ) && ( right_index >= 3950 )) draw_text_marker (dc, _T (".45"), x + locations [3950]);
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (( left_index <= 4050 ) && ( right_index >= 4050 )) draw_text_marker (dc, _T (".55"), x + locations [4050]);
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (( left_index <= 4150 ) && ( right_index >= 4150 )) draw_text_marker (dc, _T (".65"), x + locations [4150]);
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (( left_index <= 4250 ) && ( right_index >= 4250 )) draw_text_marker (dc, _T (".75"), x + locations [4250]);
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".81"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (left_index <= 4500) location = draw_markings_for_100 (dc, & locations [4400], _T (".82"), x, location, max(0, left_index - 4400), min(right_index - 4400, 100));
		if (left_index <= 4600) location = draw_markings_for_100 (dc, & locations [4500], _T (".83"), x, location, max(0, left_index - 4500), min(right_index - 4500, 100));
		if (left_index <= 4700) location = draw_markings_for_100 (dc, & locations [4600], _T (".84"), x, location, max(0, left_index - 4600), min(right_index - 4600, 100));
		if (left_index <= 4800) location = draw_markings_for_100 (dc, & locations [4700], _T (".85"), x, location, max(0, left_index - 4700), min(right_index - 4700, 100));
		if (left_index <= 4900) location = draw_markings_for_100 (dc, & locations [4800], _T (".86"), x, location, max(0, left_index - 4800), min(right_index - 4800, 100));
		if (left_index <= 5000) location = draw_markings_for_100 (dc, & locations [4900], _T (".87"), x, location, max(0, left_index - 4900), min(right_index - 4900, 100));
		if (left_index <= 5100) location = draw_markings_for_100 (dc, & locations [5000], _T (".88"), x, location, max(0, left_index - 5000), min(right_index - 5000, 100));
		if (left_index <= 5200) location = draw_markings_for_100 (dc, & locations [5100], _T (".89"), x, location, max(0, left_index - 5100), min(right_index - 5100, 100));
		if (left_index <= 5300) location = draw_markings_for_100 (dc, & locations [5200], _T (".9"), x, location, max(0, left_index - 5200), min(right_index - 5200, 100));
		if (left_index <= 5400) location = draw_markings_for_100 (dc, & locations [5300], _T (".91"), x, location, max(0, left_index - 5300), min(right_index - 5300, 100));
		if (left_index <= 5500) location = draw_markings_for_100 (dc, & locations [5400], _T (".92"), x, location, max(0, left_index - 5400), min(right_index - 5400, 100));
		if (left_index <= 5600) location = draw_markings_for_100 (dc, & locations [5500], _T (".93"), x, location, max(0, left_index - 5500), min(right_index - 5500, 100));
		if (left_index <= 5700) location = draw_markings_for_100 (dc, & locations [5600], _T (".94"), x, location, max(0, left_index - 5600), min(right_index - 5600, 100));
		if (left_index <= 5800) location = draw_markings_for_100 (dc, & locations [5700], _T (".95"), x, location, max(0, left_index - 5700), min(right_index - 5700, 100));
		if (left_index <= 5900) location = draw_markings_for_100 (dc, & locations [5800], _T (".96"), x, location, max(0, left_index - 5800), min(right_index - 5800, 100));
		if (left_index <= 6000) location = draw_markings_for_100 (dc, & locations [5900], _T (".97"), x, location, max(0, left_index - 5900), min(right_index - 5900, 100));
		if (left_index <= 6100) location = draw_markings_for_100 (dc, & locations [6000], _T (".98"), x, location, max(0, left_index - 6000), min(right_index - 6000, 100));
		if (left_index <= 6200) location = draw_markings_for_100 (dc, & locations [6100], _T (""), x, location, max(0, left_index - 6100), min(right_index - 6100, 100));
		if (( left_index <= 6200 ) && ( right_index >= 6200 )) draw_text_marker (dc, _T (".99"), x + locations [6200]);
		if (left_index <= 6300) location = draw_markings_for_100 (dc, & locations [6200], _T (""), x, location, max(0, left_index - 6200), min(right_index - 6200, 100));
		if (left_index <= 6400) location = draw_markings_for_100 (dc, & locations [6300], _T (""), x, location, max(0, left_index - 6300), min(right_index - 6300, 100));
		if (left_index <= 6500) location = draw_markings_for_100 (dc, & locations [6400], _T (""), x, location, max(0, left_index - 6400), min(right_index - 6400, 100));
		if (left_index <= 6600) location = draw_markings_for_100 (dc, & locations [6500], _T (""), x, location, max(0, left_index - 6500), min(right_index - 6500, 100));
		if (left_index <= 6700) location = draw_markings_for_100 (dc, & locations [6600], _T (""), x, location, max(0, left_index - 6600), min(right_index - 6600, 100));
		if (( left_index <= 6700 ) && ( right_index >= 6700 )) draw_text_marker (dc, _T (".995"), x + locations [6700]);
		if (left_index <= 6800) location = draw_markings_for_100 (dc, & locations [6700], _T (""), x, location, max(0, left_index - 6700), min(right_index - 6700, 100));
		if (left_index <= 6900) location = draw_markings_for_100 (dc, & locations [6800], _T (""), x, location, max(0, left_index - 6800), min(right_index - 6800, 100));
		if (left_index <= 7000) location = draw_markings_for_100 (dc, & locations [6900], _T (""), x, location, max(0, left_index - 6900), min(right_index - 6900, 100));
		if (left_index <= 7100) location = draw_markings_for_100 (dc, & locations [7000], _T (""), x, location, max(0, left_index - 7000), min(right_index - 7000, 100));
		if (( left_index <= 7100 ) && ( right_index >= 7100 )) draw_text_marker (dc, _T (".999"), x + locations [7100]);
		if (left_index <= 7200) location = draw_markings_for_100 (dc, & locations [7100], _T (""), x, location, max(0, left_index - 7100), min(right_index - 7100, 100));
		if (left_index <= 7300) location = draw_markings_for_100 (dc, & locations [7200], _T (""), x, location, max(0, left_index - 7200), min(right_index - 7200, 100));
		if (left_index <= 7400) location = draw_markings_for_100 (dc, & locations [7300], _T (""), x, location, max(0, left_index - 7300), min(right_index - 7300, 100));
		if (left_index <= 7500) location = draw_markings_for_100 (dc, & locations [7400], _T (""), x, location, max(0, left_index - 7400), min(right_index - 7400, 100));
		if (left_index <= 7600) location = draw_markings_for_100 (dc, & locations [7500], _T (""), x, location, max(0, left_index - 7500), min(right_index - 7500, 100));
		if (( left_index <= 7600 ) && ( right_index >= 7600 )) draw_text_marker (dc, _T (".9995"), x + locations [7600]);
		if (left_index <= 7700) location = draw_markings_for_100 (dc, & locations [7600], _T (""), x, location, max(0, left_index - 7600), min(right_index - 7600, 100));
		if (left_index <= 7800) location = draw_markings_for_100 (dc, & locations [7700], _T (""), x, location, max(0, left_index - 7700), min(right_index - 7700, 100));
		if (left_index <= 7900) location = draw_markings_for_100 (dc, & locations [7800], _T (""), x, location, max(0, left_index - 7800), min(right_index - 7800, 100));
		if (left_index <= 8000) location = draw_markings_for_100 (dc, & locations [7900], _T (""), x, location, max(0, left_index - 7900), min(right_index - 7900, 100));
		if (( left_index <= 8000 ) && ( right_index >= 8000 )) draw_text_marker (dc, _T (".9999"), x + locations [8000]);
		if (left_index <= 8100) location = draw_markings_for_100 (dc, & locations [8000], _T (""), x, location, max(0, left_index - 8000), min(right_index - 8000, 100));
		if (left_index <= 8200) location = draw_markings_for_100 (dc, & locations [8100], _T (""), x, location, max(0, left_index - 8100), min(right_index - 8100, 100));
		if (left_index <= 8300) location = draw_markings_for_100 (dc, & locations [8200], _T (""), x, location, max(0, left_index - 8200), min(right_index - 8200, 100));
		if (left_index <= 8400) location = draw_markings_for_100 (dc, & locations [8300], _T (""), x, location, max(0, left_index - 8300), min(right_index - 8300, 100));
		if (left_index <= 8500) location = draw_markings_for_100 (dc, & locations [8400], _T (""), x, location, max(0, left_index - 8400), min(right_index - 8400, 100));
		if (( left_index <= 8500 ) && ( right_index >= 8500 )) draw_text_marker (dc, _T (".99995"), x + locations [8500]);
		if (left_index <= 8600) location = draw_markings_for_100 (dc, & locations [8500], _T (""), x, location, max(0, left_index - 8500), min(right_index - 8500, 100));
		if (left_index <= 8700) location = draw_markings_for_100 (dc, & locations [8600], _T (""), x, location, max(0, left_index - 8600), min(right_index - 8600, 100));
		if (left_index <= 8800) location = draw_markings_for_100 (dc, & locations [8700], _T (""), x, location, max(0, left_index - 8700), min(right_index - 8700, 100));
		if (left_index <= 8900) location = draw_markings_for_100 (dc, & locations [8800], _T (""), x, location, max(0, left_index - 8800), min(right_index - 8800, 100));
		if (( left_index <= 8900 ) && ( right_index >= 8900 )) draw_text_marker (dc, _T (".99999"), x + locations [8900]);
	}
	ScaleFDistUp (double powerof10, double degoffreedom1, double degoffreedom2, int height) : StatisticsBase (height) {v1 = degoffreedom1; v2 = degoffreedom2; n = powerof10; left_index = 0; right_index = 4760;}
};
class ScaleFDistDown : public ScaleFDistUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleFDistDown (double powerof10, double degoffreedom1, double degoffreedom2, int height) : ScaleFDistUp (powerof10, degoffreedom1, degoffreedom2, height) {}
};
class ScaleChiSquareDistUp : public StatisticsBase {
public:
	double v;
	double n;
	double locations[5000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) {return log10( InvChiSquareDist( v, x ) ) - n; }
	virtual double getValue (double x) {return ChiSquareDist( v, pow(10, x + n ) ); }
	void init_locations (void) {
		if (cinitlocations || (v != clastv) || (scale_length != clastsl)) {
			for (int ind = 0; ind <= 900; ind++) {
				clocations [ind] = (double) scale_length * (log10( InvChiSquareDist( v, 0.00001 + (double) ind / 10000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				clocations [ind + 900] = (double) scale_length * (log10( InvChiSquareDist( v, 0.0001 + (double) ind / 1000000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				clocations [ind + 1800] = (double) scale_length * (log10( InvChiSquareDist( v, 0.001 + (double) ind / 100000.0 )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				clocations [ind + 2700] = (double) scale_length * (log10( InvChiSquareDist( v, 0.01 + (double) ind / 10000.0 )) - n);}
			for (int ind = 1; ind <= 800; ind++) {
				clocations [ind + 3600] = (double) scale_length * (log10( InvChiSquareDist( v, 0.1 + (double) ind / 1000.0 )) - n);}
			for (int ind = 1; ind <= 90; ind++) {
				clocations [ind + 4400] = (double) scale_length * (log10( InvChiSquareDist( v, 0.9 + (double) ind / 1000.0 )) - n);}
			for (int ind = 1; ind <= 90; ind++) {
				clocations [ind + 4490] = (double) scale_length * (log10( InvChiSquareDist( v, 0.99 + (double) ind / 10000.0 )) - n);}
			for (int ind = 1; ind <= 90; ind++) {
				clocations [ind + 4580] = (double) scale_length * (log10( InvChiSquareDist( v, 0.999 + (double) ind / 100000.0 )) - n);}
			for (int ind = 1; ind <= 90; ind++) {
				clocations [ind + 4670] = (double) scale_length * (log10( InvChiSquareDist( v, 0.9999 + (double) ind / 1000000.0 )) - n);}
			clastn = n;
			clastv = v;
			clastsl = scale_length;
			cinitlocations = false;
			for (int ind = 0; ind <= 4760; ind++)
				locations [ind] = clocations [ind]; }
		else {
			if (n != clastn) {
				for (int ind = 0; ind <= 4760; ind++)
					clocations [ind] += ((clastn - n)*scale_length);
				clastn = n; }
			for (int ind = 0; ind <= 4760; ind++)
				locations [ind] = clocations [ind]; }
		left_index = find_left_index (locations, 1, 4760, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4760, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".9"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (left_index <= 4490) location = draw_markings_for_100 (dc, & locations [4400], _T (""), x, location, max(0, left_index - 4400), min(right_index - 4400, 90));
		if (( left_index <= 4490 ) && ( right_index >= 4490 )) draw_text_marker (dc, _T (".99"), x + locations [4490]);
		if (( left_index <= 4540 ) && ( right_index >= 4540 )) draw_text_marker (dc, _T (".995"), x + locations [4540]);
		if (left_index <= 4580) location = draw_markings_for_100 (dc, & locations [4490], _T (""), x, location, max(0, left_index - 4490), min(right_index - 4490, 90));
		if (( left_index <= 4580 ) && ( right_index >= 4580 )) draw_text_marker (dc, _T (".999"), x + locations [4580]);
		if (( left_index <= 4630 ) && ( right_index >= 4630 )) draw_text_marker (dc, _T (".9995"), x + locations [4630]);
		if (left_index <= 4670) location = draw_markings_for_100 (dc, & locations [4580], _T (""), x, location, max(0, left_index - 4580), min(right_index - 4580, 90));
		if (( left_index <= 4670 ) && ( right_index >= 4670 )) draw_index_location (dc, x + locations [4670], _T (".9999"));
		if (left_index <= 4760) location = draw_markings_for_100 (dc, & locations [4670], _T (""), x, location, max(0, left_index - 4670), min(right_index - 4670, 90));
		if (( left_index <= 4760 ) && ( right_index >= 4760 )) draw_index_location (dc, x + locations [4760], _T (".99999"));;
	}
	ScaleChiSquareDistUp (double powerof10, double degoffreedom, int height) : StatisticsBase (height) {n = powerof10; v = degoffreedom; left_index = 0; right_index = 4760;}
};
class ScaleChiSquareDistDown : public ScaleChiSquareDistUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleChiSquareDistDown (double powerof10, double degoffreedom, int height) : ScaleChiSquareDistUp (powerof10, degoffreedom, height) {}
};
class ScaleChiSquareDegOfFreedomUp : public StatisticsBase {
public:
	double a;
	double n;
	double locations[2800];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) {return log10( InvChiSquareDist( x, a ) ) - n; }
	virtual double getValue (double x) {return ChiSquareDegOfFreedom( pow(10, x + n), a ); }
	void init_locations (void) {
		if (cdinitlocations || (a != cdlasta) || (scale_length != cdlastsl)) {
			for (int ind = 0; ind <= 900; ind++) {
				cdlocations [ind] = (double) scale_length * (log10( InvChiSquareDist( 0.1 + (double) ind / 1000.0, a )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				cdlocations [ind + 900] = (double) scale_length * (log10( InvChiSquareDist( 1.0 + (double) ind / 100.0, a )) - n);}
			for (int ind = 1; ind <= 900; ind++) {
				cdlocations [ind + 1800] = (double) scale_length * (log10( InvChiSquareDist( 10.0 + (double) ind / 10.0, a )) - n);}
			cdlastn = n;
			cdlasta = a;
			cdlastsl = scale_length;
			cdinitlocations = false;
			for (int ind = 0; ind <= 2700; ind++)
				locations [ind] = cdlocations [ind]; }
		else {
			if (n != cdlastn) {
				for (int ind = 0; ind <= 2700; ind++)
					cdlocations [ind] += ((cdlastn - n)*scale_length);
				cdlastn = n; }
			for (int ind = 0; ind <= 2700; ind++)
				locations [ind] = cdlocations [ind]; }
		left_index = find_left_index (locations, 1, 2700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 2700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T ("0.1"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T ("0.2"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T ("0.3"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T ("0.4"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T ("0.5"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T ("0.6"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T ("0.7"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T ("0.8"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T ("0.9"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T ("1"), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T ("2"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T ("3"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T ("4"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T ("5"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T ("6"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T ("7"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T ("8"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T ("9"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T ("10"), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T ("20"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T ("30"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T ("40"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T ("50"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T ("60"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T ("70"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T ("80"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T ("90"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T ("100"), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
	}
	ScaleChiSquareDegOfFreedomUp (double powerof10, double area, int height) : StatisticsBase (height) {n = powerof10; a = area; left_index = 0; right_index = 2700;}
};
class ScaleChiSquareDegOfFreedomDown : public ScaleChiSquareDegOfFreedomUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleChiSquareDegOfFreedomDown (double powerof10, double area, int height) : ScaleChiSquareDegOfFreedomUp (powerof10, area, height) {}
};
class ScaleNormalDist1TailUp : public StatisticsBase {
public:
	double v;
	double n;
	double locations[1300];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	double halfDistanceFrom1(double x) {return 1.0 - (1.0 - x) / 2.0;}
	double doubleDistanceFrom1(double x) {return 1.0 - (1.0 - x) * 2.0;}
	virtual double getLocation (double x) {return (log10( InvNormalDist( doubleDistanceFrom1(x) ) ) - n); }
	virtual double getValue (double x) {return halfDistanceFrom1(NormalDist( pow(10, x + n ) ) ); }
	void init_locations (void) {
		for (int ind = 1; ind <= 500; ind++) {
			locations [ind] = (double) scale_length * (log10( InvNormalDist( doubleDistanceFrom1(0.5 + (double) ind / 10000.0 ))) - n);}
		for (int ind = 1; ind <= 440; ind++) {
			locations [ind + 500] = (double) scale_length * (log10( InvNormalDist( doubleDistanceFrom1(0.55 + (double) ind / 1000.0 ))) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 940] = (double) scale_length * (log10( InvNormalDist( doubleDistanceFrom1(0.99 + (double) ind / 10000.0 ))) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 1030] = (double) scale_length * (log10( InvNormalDist( doubleDistanceFrom1(0.999 + (double) ind / 100000.0 ))) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 1120] = (double) scale_length * (log10( InvNormalDist( doubleDistanceFrom1(0.9999 + (double) ind / 1000000.0 ))) - n);}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 1210, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 1210, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (( left_index <= 10 ) && ( right_index >= 10 )) draw_text_marker (dc, _T (".501"), x + locations [10]);
		if (( left_index <= 20 ) && ( right_index >= 20 )) draw_text_marker (dc, _T (".502"), x + locations [20]);
		if (( left_index <= 30 ) && ( right_index >= 30 )) draw_text_marker (dc, _T (".503"), x + locations [30]);
		if (( left_index <= 40 ) && ( right_index >= 40 )) draw_text_marker (dc, _T (".504"), x + locations [40]);
		if (( left_index <= 50 ) && ( right_index >= 50 )) draw_text_marker (dc, _T (".505"), x + locations [50]);
		if (( left_index <= 60 ) && ( right_index >= 60 )) draw_text_marker (dc, _T (".506"), x + locations [60]);
		if (( left_index <= 70 ) && ( right_index >= 70 )) draw_text_marker (dc, _T (".507"), x + locations [70]);
		if (( left_index <= 80 ) && ( right_index >= 80 )) draw_text_marker (dc, _T (".508"), x + locations [80]);
		if (( left_index <= 90 ) && ( right_index >= 90 )) draw_text_marker (dc, _T (".509"), x + locations [90]);
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".51"), x, location, max(0, left_index), min(right_index, 100));
		if (( left_index <= 150 ) && ( right_index >= 150 )) draw_text_marker (dc, _T (".515"), x + locations [150]);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".52"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".53"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".54"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".55"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (( left_index <= 550 ) && ( right_index >= 550 )) draw_text_marker (dc, _T (".6"), x + locations [550]);
		if (left_index <= 550) location = draw_markings_for_100 (dc, & locations [500], _T (""), x, location, max(0, left_index - 500), min(right_index - 500, 50));
		if (( left_index <= 600 ) && ( right_index >= 600 )) draw_text_marker (dc, _T (".65"), x + locations [600]);
		if (left_index <= 650) location = draw_markings_for_100 (dc, & locations [550], _T (".7"), x, location, max(0, left_index - 550), min(right_index - 550, 100));
		if (left_index <= 750) location = draw_markings_for_100 (dc, & locations [650], _T (".8"), x, location, max(0, left_index - 650), min(right_index - 650, 100));
		if (left_index <= 850) location = draw_markings_for_100 (dc, & locations [750], _T (".9"), x, location, max(0, left_index - 750), min(right_index - 750, 100));
		if (left_index <= 940) location = draw_markings_for_100 (dc, & locations [850], _T (""), x, location, max(0, left_index - 850), min(right_index - 850, 90));
		if (( left_index <= 940 ) && ( right_index >= 940 )) draw_text_marker (dc, _T (".99"), x + locations [940]);
		if (( left_index <= 990 ) && ( right_index >= 990 )) draw_text_marker (dc, _T (".995"), x + locations [990]);
		if (left_index <= 1030) location = draw_markings_for_100 (dc, & locations [940], _T (""), x, location, max(0, left_index - 940), min(right_index - 940, 90));
		if (( left_index <= 1030 ) && ( right_index >= 1030 )) draw_text_marker (dc, _T (".999"), x + locations [1030]);
		if (left_index <= 1120) location = draw_markings_for_100 (dc, & locations [1030], _T (""), x, location, max(0, left_index - 1030), min(right_index - 1030, 90));
		if (( left_index <= 1120 ) && ( right_index >= 1120 )) draw_index_location (dc, x + locations [1120], _T (".9999"));
		if (left_index <= 1210) location = draw_markings_for_100 (dc, & locations [1120], _T (""), x, location, max(0, left_index - 1120), min(right_index - 1120, 90));
		if (( left_index <= 1210 ) && ( right_index >= 1210 )) draw_index_location (dc, x + locations [1210], _T (".99999"));
	}
	ScaleNormalDist1TailUp (double powerof10, int height) : StatisticsBase (height) {n = powerof10; left_index = 0; right_index = 1210;}
};
class ScaleNormalDist1TailDown : public ScaleNormalDist1TailUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleNormalDist1TailDown (double powerof10, int height) : ScaleNormalDist1TailUp (powerof10, height) {}
};
class ScaleNormalDist2TailUp : public StatisticsBase {
public:
	double v;
	double n;
	double locations[5000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) {return (log10( InvNormalDist( x ) ) - n); }
	virtual double getValue (double x) {return NormalDist( pow(10, x + n ) ); }
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			locations [ind] = (double) scale_length * (log10( InvNormalDist( 0.00001 + (double) ind / 10000000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 900] = (double) scale_length * (log10( InvNormalDist( 0.0001 + (double) ind / 1000000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 1800] = (double) scale_length * (log10( InvNormalDist( 0.001 + (double) ind / 100000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 2700] = (double) scale_length * (log10( InvNormalDist( 0.01 + (double) ind / 10000.0 )) - n);}
		for (int ind = 1; ind <= 800; ind++) {
			locations [ind + 3600] = (double) scale_length * (log10( InvNormalDist( 0.1 + (double) ind / 1000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4400] = (double) scale_length * (log10( InvNormalDist( 0.9 + (double) ind / 1000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4490] = (double) scale_length * (log10( InvNormalDist( 0.99 + (double) ind / 10000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4580] = (double) scale_length * (log10( InvNormalDist( 0.999 + (double) ind / 100000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4670] = (double) scale_length * (log10( InvNormalDist( 0.9999 + (double) ind / 1000000.0 )) - n);}
		left_index = find_left_index (locations, 1, 4760, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4760, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (( left_index <= 2750 ) && ( right_index >= 2750 )) draw_text_marker (dc, _T (".015"), x + locations [2750]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (( left_index <= 3650 ) && ( right_index >= 3650 )) draw_text_marker (dc, _T (".15"), x + locations [3650]);
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".9"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (left_index <= 4490) location = draw_markings_for_100 (dc, & locations [4400], _T (""), x, location, max(0, left_index - 4400), min(right_index - 4400, 90));
		if (( left_index <= 4490 ) && ( right_index >= 4490 )) draw_text_marker (dc, _T (".99"), x + locations [4490]);
		if (( left_index <= 4540 ) && ( right_index >= 4540 )) draw_text_marker (dc, _T (".995"), x + locations [4540]);
		if (left_index <= 4580) location = draw_markings_for_100 (dc, & locations [4490], _T (""), x, location, max(0, left_index - 4490), min(right_index - 4490, 90));
		if (( left_index <= 4580 ) && ( right_index >= 4580 )) draw_text_marker (dc, _T (".999"), x + locations [4580]);
		if (left_index <= 4670) location = draw_markings_for_100 (dc, & locations [4580], _T (""), x, location, max(0, left_index - 4580), min(right_index - 4580, 90));
		if (( left_index <= 4670 ) && ( right_index >= 4670 )) draw_index_location (dc, x + locations [4670], _T (".9999"));
		if (left_index <= 4760) location = draw_markings_for_100 (dc, & locations [4670], _T (""), x, location, max(0, left_index - 4670), min(right_index - 4670, 80));
		if (( left_index <= 4760 ) && ( right_index >= 4760 )) draw_index_location (dc, x + locations [4760],_T (".99999"));
	}
	ScaleNormalDist2TailUp (double powerof10, int height) : StatisticsBase (height) {n = powerof10; left_index = 0; right_index = 4760;}
};
class ScaleNormalDist2TailDown : public ScaleNormalDist2TailUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleNormalDist2TailDown (double powerof10, int height) : ScaleNormalDist2TailUp (powerof10, height) {}
};
class ScaleGammaDistUp : public StatisticsBase {
public:
	double v;
	double n;
	double locala;
	double localb;
	double locations[5000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) {return log10( InvGammaDist( locala, localb, x ) )  - n; }
	virtual double getValue (double x) {return GammaDist( locala, localb, pow(10, x + n ) ); }
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			locations [ind] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.00001 + (double) ind / 10000000.0 ) ) - n );}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 900] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.0001 + (double) ind / 1000000.0 ) ) - n );}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 1800] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.001 + (double) ind / 100000.0 ) ) - n );}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 2700] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.01 + (double) ind / 10000.0 ) ) - n );}
		for (int ind = 1; ind <= 800; ind++) {
			locations [ind + 3600] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.1 + (double) ind / 1000.0 ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4400] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.9 + (double) ind / 1000.0 ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4490] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.99 + (double) ind / 10000.0 ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4580] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.999 + (double) ind / 100000.0 ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4670] = (double) scale_length * (log10( InvGammaDist( locala, localb, 0.9999 + (double) ind / 1000000.0 ) ) - n );}
		left_index = find_left_index (locations, 1, 4760, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4760, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (( left_index <= 3650) && ( right_index >= 3650 )) draw_text_marker (dc, _T (".15"), x + locations [3650]);
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".9"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (( left_index <= 4450 ) && ( right_index >= 4450 )) draw_text_marker (dc, _T (".95"), x + locations [4450]);
		if (left_index <= 4490) location = draw_markings_for_100 (dc, & locations [4400], _T (""), x, location, max(0, left_index - 4400), min(right_index - 4400, 90));
		if (( left_index <= 4490 ) && ( right_index >= 4490 )) draw_text_marker (dc, _T (".99"), x + locations [4490]);
		if (( left_index <= 4540 ) && ( right_index >= 4540 )) draw_text_marker (dc, _T (".995"), x + locations [4540]);
		if (left_index <= 4580) location = draw_markings_for_100 (dc, & locations [4490], _T (""), x, location, max(0, left_index - 4490), min(right_index - 4490, 90));
		if (( left_index <= 4580 ) && ( right_index >= 4580 )) draw_text_marker (dc, _T (".999"), x + locations [4580]);
		if (left_index <= 4670) location = draw_markings_for_100 (dc, & locations [4580], _T (""), x, location, max(0, left_index - 4580), min(right_index - 4580, 90));
		if (( left_index <= 4670 ) && ( right_index >= 4670 )) draw_index_location (dc, x + locations [4670], _T (".9999"));
		if (left_index <= 4760) location = draw_markings_for_100 (dc, & locations [4670], _T (""), x, location, max(0, left_index - 4670), min(right_index - 4670, 80));
		if (( left_index <= 4760 ) && ( right_index >= 4760 )) draw_index_location (dc, x + locations [4760],_T (".99999"));
	}
	ScaleGammaDistUp ( double powerof10, double a, double b, int height) : StatisticsBase (height) { n = powerof10; locala = a; localb = b; left_index = 0; right_index = 4760;}
};
class ScaleGammaDistDown : public ScaleGammaDistUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleGammaDistDown (double powerof10, double a, double b, int height) : ScaleGammaDistUp (powerof10, a, b, height) {}
};
class ScaleStudentsTDist1TailUp : public StatisticsBase {
public:
	double v;
	double n;
	double locations[1000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	double halfDistanceFrom1(double x) {return 1.0 - (1.0 - x) / 2.0;}
	double doubleDistanceFrom1(double x) {return 1.0 - (1.0 - x) * 2.0;}
	virtual double getLocation (double x) { return log10( InvStudentsTDist( v, doubleDistanceFrom1(x) ) ) - n; }
	virtual double getValue (double x) {return halfDistanceFrom1(StudentsTDist( v, pow(10, x + n ) ) ); }
	void init_locations (void) {
		for (int ind = 0; ind <= 400; ind++) {
			locations [ind] = (double) scale_length * (log10( InvStudentsTDist( v, doubleDistanceFrom1(0.5 + (double) ind / 1000.0 ) ) ) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 400] = (double) scale_length * (log10( InvStudentsTDist( v, doubleDistanceFrom1(0.9 + (double) ind / 1000.0 ) ) ) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 490] = (double) scale_length * (log10( InvStudentsTDist( v, doubleDistanceFrom1(0.99 + (double) ind / 10000.0 ) ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 580] = (double) scale_length * (log10( InvStudentsTDist( v, doubleDistanceFrom1(0.999 + (double) ind / 100000.0 ) ) ) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 670] = (double) scale_length * (log10( InvStudentsTDist( v, doubleDistanceFrom1(0.9999 + (double) ind / 1000000.0 ) ) ) - n );}
		left_index = find_left_index (locations, 1, 760, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 760, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (( left_index <= 1 ) && ( right_index >= 1 )) draw_text_marker (dc, _T (".501"), x + locations [1]);
		if (( left_index <= 2 ) && ( right_index >= 2 )) draw_text_marker (dc, _T (".502"), x + locations [2]);
		if (( left_index <= 3 ) && ( right_index >= 3 )) draw_text_marker (dc, _T (".503"), x + locations [3]);
		if (( left_index <= 4 ) && ( right_index >= 4 )) draw_text_marker (dc, _T (".504"), x + locations [4]);
		if (( left_index <= 5 ) && ( right_index >= 5 )) draw_text_marker (dc, _T (".505"), x + locations [5]);
		if (( left_index <= 6 ) && ( right_index >= 6 )) draw_text_marker (dc, _T (".506"), x + locations [6]);
		if (( left_index <= 7 ) && ( right_index >= 7 )) draw_text_marker (dc, _T (".507"), x + locations [7]);
		if (( left_index <= 8 ) && ( right_index >= 8 )) draw_text_marker (dc, _T (".508"), x + locations [8]);
		if (( left_index <= 9 ) && ( right_index >= 9 )) draw_text_marker (dc, _T (".509"), x + locations [9]);
		if (( left_index <= 10 ) && ( right_index >= 10 )) draw_text_marker (dc, _T (".51"), x + locations [10]);
		if (( left_index <= 20 ) && ( right_index >= 20 )) draw_text_marker (dc, _T (".52"), x + locations [20]);
		if (( left_index <= 30 ) && ( right_index >= 30 )) draw_text_marker (dc, _T (".53"), x + locations [30]);
		if (( left_index <= 40 ) && ( right_index >= 40 )) draw_text_marker (dc, _T (".54"), x + locations [40]);
		if (( left_index <= 50 ) && ( right_index >= 50 )) draw_text_marker (dc, _T (".55"), x + locations [50]);
		if (( left_index <= 60 ) && ( right_index >= 60 )) draw_text_marker (dc, _T (".56"), x + locations [60]);
		if (( left_index <= 70 ) && ( right_index >= 70 )) draw_text_marker (dc, _T (".57"), x + locations [70]);
		if (( left_index <= 80 ) && ( right_index >= 80 )) draw_text_marker (dc, _T (".58"), x + locations [80]);
		if (( left_index <= 90 ) && ( right_index >= 90 )) draw_text_marker (dc, _T (".59"), x + locations [90]);
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".6"), x, location, max(0, left_index), min(right_index, 100));
		if (( left_index <= 150 ) && ( right_index >= 150 )) draw_text_marker (dc, _T (".65"), x + locations [150]);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".7"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (( left_index <= 250 ) && ( right_index >= 250 )) draw_text_marker (dc, _T (".75"), x + locations [250]);
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".8"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (( left_index <= 350 ) && ( right_index >= 350 )) draw_text_marker (dc, _T (".85"), x + locations [350]);
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".9"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (( left_index <= 450 ) && ( right_index >= 450 )) draw_text_marker (dc, _T (".95"), x + locations [450]);
		if (left_index <= 490) location = draw_markings_for_100 (dc, & locations [400], _T (""), x, location, max(0, left_index - 400), min(right_index - 400, 90));
		if (( left_index <= 490 ) && ( right_index >= 490 )) draw_text_marker (dc, _T (".99"), x + locations [490]);
		if (( left_index <= 540 ) && ( right_index >= 540 )) draw_text_marker (dc, _T (".995"), x + locations [540]);
		if (left_index <= 580) location = draw_markings_for_100 (dc, & locations [490], _T (""), x, location, max(0, left_index - 490), min(right_index - 490, 90));
		if (( left_index <= 580 ) && ( right_index >= 580 )) draw_text_marker (dc, _T (".999"), x + locations [580]);
		if (( left_index <= 630 ) && ( right_index >= 630 )) draw_text_marker (dc, _T (".9995"), x + locations [630]);
		if (left_index <= 670) location = draw_markings_for_100 (dc, & locations [580], _T (""), x, location, max(0, left_index - 580), min(right_index - 580, 90));
		if (( left_index <= 670 ) && ( right_index >= 670 )) draw_index_location (dc, x + locations [670], _T (".9999"));
		if (left_index <= 760) location = draw_markings_for_100 (dc, & locations [670], _T (""), x, location, max(0, left_index - 670), min(right_index - 670, 90));
		if (( left_index <= 760 ) && ( right_index >= 760 )) draw_index_location (dc, x + locations [760], _T (".99999"));
	}
	ScaleStudentsTDist1TailUp (double powerof10, double degoffreedom, int height) : StatisticsBase (height) {v = degoffreedom; n = powerof10; left_index = 0; right_index = 4760;}
};
class ScaleStudentsTDist1TailDown : public ScaleStudentsTDist1TailUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleStudentsTDist1TailDown (double powerof10, double degoffreedom, int height) : ScaleStudentsTDist1TailUp (powerof10, degoffreedom, height) {}
};
class ScaleStudentsTDist2TailUp : public StatisticsBase {
public:
	double v;
	double n;
	double locations[5000];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	virtual double getLocation (double x) { return log10( InvStudentsTDist( v, x ) ) - n; }
	virtual double getValue (double x) {return StudentsTDist( v, pow(10, x + n ) ); }
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			locations [ind] = (double) scale_length * (log10( InvStudentsTDist( v, 0.00001 + (double) ind / 10000000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 900] = (double) scale_length * (log10( InvStudentsTDist( v, 0.0001 + (double) ind / 1000000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 1800] = (double) scale_length * (log10( InvStudentsTDist( v, 0.001 + (double) ind / 100000.0 )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 2700] = (double) scale_length * (log10( InvStudentsTDist( v, 0.01 + (double) ind / 10000.0 )) - n);}
		for (int ind = 1; ind <= 800; ind++) {
			locations [ind + 3600] = (double) scale_length * (log10( InvStudentsTDist( v, 0.1 + (double) ind / 1000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4400] = (double) scale_length * (log10( InvStudentsTDist( v, 0.9 + (double) ind / 1000.0 )) - n);}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4490] = (double) scale_length * (log10( InvStudentsTDist( v, 0.99 + (double) ind / 10000.0 )) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4580] = (double) scale_length * (log10( InvStudentsTDist( v, 0.999 + (double) ind / 100000.0 )) - n );}
		for (int ind = 1; ind <= 90; ind++) {
			locations [ind + 4670] = (double) scale_length * (log10( InvStudentsTDist( v, 0.9999 + (double) ind / 1000000.0 )) - n );}
		left_index = find_left_index (locations, 1, 4760, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4760, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T (".00001"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".00002"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".00003"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T (".00004"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T (".00005"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T (".00006"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T (".00007"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T (".00008"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T (".00009"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (( left_index <= 900 ) && ( right_index >= 900 )) draw_text_marker (dc, _T (".0001"), x + locations [900]);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T (".0002"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T (".0003"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T (".0004"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T (".0005"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T (".0006"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T (".0007"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T (".0008"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T (".0009"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T (""), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (( left_index <= 1800 ) && ( right_index >= 1800 )) draw_text_marker (dc, _T (".001"), x + locations [1800]);
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".002"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".003"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".004"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".005"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".006"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".007"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".008"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".009"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (""), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
		if (( left_index <= 2700 ) && ( right_index >= 2700 )) draw_text_marker (dc, _T (".01"), x + locations [2700]);
		if (( left_index <= 2750 ) && ( right_index >= 2750 )) draw_text_marker (dc, _T (".015"), x + locations [2750]);
		if (left_index <= 2800) location = draw_markings_for_100 (dc, & locations [2700], _T (".02"), x, location, max(0, left_index - 2700), min(right_index - 2700, 100));
		if (left_index <= 2900) location = draw_markings_for_100 (dc, & locations [2800], _T (".03"), x, location, max(0, left_index - 2800), min(right_index - 2800, 100));
		if (left_index <= 3000) location = draw_markings_for_100 (dc, & locations [2900], _T (".04"), x, location, max(0, left_index - 2900), min(right_index - 2900, 100));
		if (left_index <= 3100) location = draw_markings_for_100 (dc, & locations [3000], _T (".05"), x, location, max(0, left_index - 3000), min(right_index - 3000, 100));
		if (left_index <= 3200) location = draw_markings_for_100 (dc, & locations [3100], _T (".06"), x, location, max(0, left_index - 3100), min(right_index - 3100, 100));
		if (left_index <= 3300) location = draw_markings_for_100 (dc, & locations [3200], _T (".07"), x, location, max(0, left_index - 3200), min(right_index - 3200, 100));
		if (left_index <= 3400) location = draw_markings_for_100 (dc, & locations [3300], _T (".08"), x, location, max(0, left_index - 3300), min(right_index - 3300, 100));
		if (left_index <= 3500) location = draw_markings_for_100 (dc, & locations [3400], _T (".09"), x, location, max(0, left_index - 3400), min(right_index - 3400, 100));
		if (left_index <= 3600) location = draw_markings_for_100 (dc, & locations [3500], _T (".1"), x, location, max(0, left_index - 3500), min(right_index - 3500, 100));
		if (( left_index <= 3650 ) && ( right_index >= 3650 )) draw_text_marker (dc, _T (".15"), x + locations [3650]);
		if (left_index <= 3700) location = draw_markings_for_100 (dc, & locations [3600], _T (".2"), x, location, max(0, left_index - 3600), min(right_index - 3600, 100));
		if (left_index <= 3800) location = draw_markings_for_100 (dc, & locations [3700], _T (".3"), x, location, max(0, left_index - 3700), min(right_index - 3700, 100));
		if (left_index <= 3900) location = draw_markings_for_100 (dc, & locations [3800], _T (".4"), x, location, max(0, left_index - 3800), min(right_index - 3800, 100));
		if (left_index <= 4000) location = draw_markings_for_100 (dc, & locations [3900], _T (".5"), x, location, max(0, left_index - 3900), min(right_index - 3900, 100));
		if (left_index <= 4100) location = draw_markings_for_100 (dc, & locations [4000], _T (".6"), x, location, max(0, left_index - 4000), min(right_index - 4000, 100));
		if (left_index <= 4200) location = draw_markings_for_100 (dc, & locations [4100], _T (".7"), x, location, max(0, left_index - 4100), min(right_index - 4100, 100));
		if (left_index <= 4300) location = draw_markings_for_100 (dc, & locations [4200], _T (".8"), x, location, max(0, left_index - 4200), min(right_index - 4200, 100));
		if (left_index <= 4400) location = draw_markings_for_100 (dc, & locations [4300], _T (".9"), x, location, max(0, left_index - 4300), min(right_index - 4300, 100));
		if (( left_index <= 4450 ) && ( right_index >= 4450 )) draw_text_marker (dc, _T (".95"), x + locations [4450]);
		if (left_index <= 4490) location = draw_markings_for_100 (dc, & locations [4400], _T (""), x, location, max(0, left_index - 4400), min(right_index - 4400, 90));
		if (( left_index <= 4490 ) && ( right_index >= 4490 )) draw_text_marker (dc, _T (".99"), x + locations [4490]);
		if (( left_index <= 4540 ) && ( right_index >= 4540 )) draw_text_marker (dc, _T (".995"), x + locations [4540]);
		if (left_index <= 4580) location = draw_markings_for_100 (dc, & locations [4490], _T (""), x, location, max(0, left_index - 4490), min(right_index - 4490, 90));
		if (( left_index <= 4580 ) && ( right_index >= 4580 )) draw_text_marker (dc, _T (".999"), x + locations [4580]);
		if (left_index <= 4670) location = draw_markings_for_100 (dc, & locations [4580], _T (""), x, location, max(0, left_index - 4580), min(right_index - 4580, 90));
		if (( left_index <= 4670 ) && ( right_index >= 4670 )) draw_index_location (dc, x + locations [4670], _T (".9999"));
		if (left_index <= 4760) location = draw_markings_for_100 (dc, & locations [4670], _T (""), x, location, max(0, left_index - 4670), min(right_index - 4670, 80));
		if (( left_index <= 4760 ) && ( right_index >= 4760 )) draw_index_location (dc, x + locations [4760],_T (".99999"));
	}
	ScaleStudentsTDist2TailUp (double powerof10, double degoffreedom, int height) : StatisticsBase (height) {v = degoffreedom; n = powerof10; left_index = 0; right_index = 4760;}
};
class ScaleStudentsTDist2TailDown : public ScaleStudentsTDist2TailUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleStudentsTDist2TailDown (double powerof10, double degoffreedom, int height) : ScaleStudentsTDist2TailUp (powerof10, degoffreedom, height) {}
};
class ScaleStudentsTDegOfFreedomUp : public StatisticsBase {
public:
	double a;
	double n;
	double locations[2800];
	int left_index, right_index;
	int min(int i1, int i2) {if (i1<i2) return i1; else return i2;}
	int max(int i1, int i2) {if (i1>i2) return i1; else return i2;}
	double doubleDistanceFrom1(double x) {return 1.0 - (1.0 - x) * 2.0;}
	virtual double getLocation (double x) {return log10( InvStudentsTDist( x, doubleDistanceFrom1(a) ) ) - n; }
	virtual double getValue (double x) {return StudentsTDegOfFreedom( pow(10, x + n), doubleDistanceFrom1(a) ); }
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			locations [ind] = (double) scale_length * (log10( InvStudentsTDist( 100.0 - (double) ind / 10.0, doubleDistanceFrom1(a) )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 900] = (double) scale_length * (log10( InvStudentsTDist( 10.0 - (double) ind / 100.0, doubleDistanceFrom1(a) )) - n);}
		for (int ind = 1; ind <= 900; ind++) {
			locations [ind + 1800] = (double) scale_length * (log10( InvStudentsTDist( 1.0 - (double) ind / 1000.0, doubleDistanceFrom1(a) )) - n);}
		//locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 2700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 2700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) draw_index_location (dc, x + location, _T ("100"));
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T ("90"), x, location, max(0, left_index), min(right_index, 100));
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T ("80"), x, location, max(0, left_index - 100), min(right_index - 100, 100));
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations [200], _T ("70"), x, location, max(0, left_index - 200), min(right_index - 200, 100));
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations [300], _T ("60"), x, location, max(0, left_index - 300), min(right_index - 300, 100));
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations [400], _T ("50"), x, location, max(0, left_index - 400), min(right_index - 400, 100));
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations [500], _T ("40"), x, location, max(0, left_index - 500), min(right_index - 500, 100));
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations [600], _T ("30"), x, location, max(0, left_index - 600), min(right_index - 600, 100));
		if (left_index <= 800) location = draw_markings_for_100 (dc, & locations [700], _T ("20"), x, location, max(0, left_index - 700), min(right_index - 700, 100));
		if (left_index <= 900) location = draw_markings_for_100 (dc, & locations [800], _T ("10"), x, location, max(0, left_index - 800), min(right_index - 800, 100));
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & locations [900], _T ("9"), x, location, max(0, left_index - 900), min(right_index - 900, 100));
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & locations [1000], _T ("8"), x, location, max(0, left_index - 1000), min(right_index - 1000, 100));
		if (left_index <= 1200) location = draw_markings_for_100 (dc, & locations [1100], _T ("7"), x, location, max(0, left_index - 1100), min(right_index - 1100, 100));
		if (left_index <= 1300) location = draw_markings_for_100 (dc, & locations [1200], _T ("6"), x, location, max(0, left_index - 1200), min(right_index - 1200, 100));
		if (left_index <= 1400) location = draw_markings_for_100 (dc, & locations [1300], _T ("5"), x, location, max(0, left_index - 1300), min(right_index - 1300, 100));
		if (left_index <= 1500) location = draw_markings_for_100 (dc, & locations [1400], _T ("4"), x, location, max(0, left_index - 1400), min(right_index - 1400, 100));
		if (left_index <= 1600) location = draw_markings_for_100 (dc, & locations [1500], _T ("3"), x, location, max(0, left_index - 1500), min(right_index - 1500, 100));
		if (left_index <= 1700) location = draw_markings_for_100 (dc, & locations [1600], _T ("2"), x, location, max(0, left_index - 1600), min(right_index - 1600, 100));
		if (left_index <= 1800) location = draw_markings_for_100 (dc, & locations [1700], _T ("1"), x, location, max(0, left_index - 1700), min(right_index - 1700, 100));
		if (left_index <= 1900) location = draw_markings_for_100 (dc, & locations [1800], _T (".9"), x, location, max(0, left_index - 1800), min(right_index - 1800, 100));
		if (left_index <= 2000) location = draw_markings_for_100 (dc, & locations [1900], _T (".8"), x, location, max(0, left_index - 1900), min(right_index - 1900, 100));
		if (left_index <= 2100) location = draw_markings_for_100 (dc, & locations [2000], _T (".7"), x, location, max(0, left_index - 2000), min(right_index - 2000, 100));
		if (left_index <= 2200) location = draw_markings_for_100 (dc, & locations [2100], _T (".6"), x, location, max(0, left_index - 2100), min(right_index - 2100, 100));
		if (left_index <= 2300) location = draw_markings_for_100 (dc, & locations [2200], _T (".5"), x, location, max(0, left_index - 2200), min(right_index - 2200, 100));
		if (left_index <= 2400) location = draw_markings_for_100 (dc, & locations [2300], _T (".4"), x, location, max(0, left_index - 2300), min(right_index - 2300, 100));
		if (left_index <= 2500) location = draw_markings_for_100 (dc, & locations [2400], _T (".3"), x, location, max(0, left_index - 2400), min(right_index - 2400, 100));
		if (left_index <= 2600) location = draw_markings_for_100 (dc, & locations [2500], _T (".2"), x, location, max(0, left_index - 2500), min(right_index - 2500, 100));
		if (left_index <= 2700) location = draw_markings_for_100 (dc, & locations [2600], _T (".1"), x, location, max(0, left_index - 2600), min(right_index - 2600, 100));
	}
	ScaleStudentsTDegOfFreedomUp (double powerof10, double area, int height) : StatisticsBase (height) {n = powerof10; a = area; left_index = 0; right_index = 2700;}
};
class ScaleStudentsTDegOfFreedomDown : public ScaleStudentsTDegOfFreedomUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleStudentsTDegOfFreedomDown (double powerof10, double area, int height) : ScaleStudentsTDegOfFreedomUp (powerof10, area, height) {}
};
class ScaleLogGamma0Up : public StatisticsBase {
public:
	double locations1[600];
	double locations2[600];
	int left_index, right_index;
	virtual double getLocation (double x) {return 4.0 + log10( LogGammaF( x ) );}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x - 4.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 500; ind++) {
			locations1 [ind] = (double) scale_length * (4.0 + log10(LogGammaF(2.0005 + (double) ind / 1000000.0)));
		}
		for (int ind = 0; ind <= 500; ind++) {
			locations2 [ind] = (double) scale_length * (4.0 + log10(LogGammaF(2.001 + (double) ind / 100000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.0006"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.0007"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.0008"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.0009"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.001"), x, location);
		draw_text_marker (dc, _T ("2.0015"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("2.002"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("2.003"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("2.004"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("2.005"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T ("2.006"), x, location, 0, right_index - 400);
	}
	ScaleLogGamma0Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 500;}
};

class ScaleLogGamma0Down : public ScaleLogGamma0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma0Down (int height) : ScaleLogGamma0Up (height) {}
};
class ScaleLogGamma1Up : public StatisticsBase {
public:
	double locations1[600];
	double locations2[600];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10( LogGammaF( x ) );}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x - 3.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 500; ind++) {
			locations1 [ind] = (double) scale_length * (3.0 + log10(LogGammaF(2.005 + (double) ind / 100000.0)));
		}
		for (int ind = 0; ind <= 500; ind++) {
			locations2 [ind] = (double) scale_length * (3.0 + log10(LogGammaF(2.01 + (double) ind / 10000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.006"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.007"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.008"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.009"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.01"), x, location);
		draw_text_marker (dc, _T ("2.015"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("2.02"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("2.03"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("2.04"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("2.05"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T ("2.06"), x, location, 0, right_index - 400);
	}
	ScaleLogGamma1Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1000;}
};

class ScaleLogGamma1Down : public ScaleLogGamma1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma1Down (int height) : ScaleLogGamma1Up (height) {}
};
class ScaleLogGamma2Up : public StatisticsBase {
public:
	double locations1[600];
	double locations2[500];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10( LogGammaF( x ) );}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x - 2.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 500; ind++) {
			locations1 [ind] = (double) scale_length * (2.0 + log10(LogGammaF(2.05 + (double) ind / 10000.0)));
		}
		for (int ind = 0; ind <= 400; ind++) {
			locations2 [ind] = (double) scale_length * (2.0 + log10(LogGammaF(2.1 + (double) ind / 1000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.06"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.07"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.08"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.09"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.1"), x, location);
		draw_text_marker (dc, _T ("2.15"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("2.2"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("2.3"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("2.4"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("2.5"), x, location, 0, right_index - 300);
	}
	ScaleLogGamma2Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 900;}
};

class ScaleLogGamma2Down : public ScaleLogGamma2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma2Down (int height) : ScaleLogGamma2Up (height) {}
};
class ScaleLogGamma3Up : public StatisticsBase {
public:
	double locations1[2200];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10( LogGammaF( x ) );}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x - 1.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 2100; ind++) {
			locations1 [ind] = (double) scale_length * (1.0 + log10(LogGammaF(2.4 + (double) ind / 1000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 2100, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations1, 1, 2100, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.5"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("3.0"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("3.1"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("3.2"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T ("3.3"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T ("3.4"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T ("3.5"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1100], _T ("3.6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1200], _T ("3.7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1300], _T ("3.8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1400], _T ("3.9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1500], _T ("4.0"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1600], _T ("4.1"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1700], _T ("4.2"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1800], _T ("4.3"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1900], _T ("4.4"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [2000], _T ("4.5"), x, location, 0, right_index - 2000);
	}
	ScaleLogGamma3Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 2000;}
};

class ScaleLogGamma3Down : public ScaleLogGamma3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma3Down (int height) : ScaleLogGamma3Up (height) {}
};
class ScaleLogGamma4Up : public StatisticsBase {
public:
	double locations1[1300];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( LogGammaF( x ) );}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 1200; ind++) {
			locations1 [ind] = (double) scale_length * (log10(LogGammaF(4.0 + (double) ind / 100.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 1100, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations1, 1, 1100, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		draw_text_marker (dc, _T ("4.5"), x + locations1 [50]);
		location = draw_markings_for_100 (dc, locations1, _T ("5"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("10"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("11"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("12"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T ("13"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T ("14"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T ("15"), x, location, 0, right_index - 1000);
	}
	ScaleLogGamma4Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1000;}
};

class ScaleLogGamma4Down : public ScaleLogGamma4Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma4Down (int height) : ScaleLogGamma4Up (height) {}
};

class ScaleLogGamma5Up : public StatisticsBase {
public:
	double locations1[800];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( LogGammaF( x ) ) - 1.0;}
	virtual double getValue (double x) {return InvLogGammaF( pow( 10, x + 1.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 700; ind++) {
			locations1 [ind] = (double) scale_length * (log10(LogGammaF(10.0 + (double) ind / 10.0)) - 1.0);
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations1, 1, 700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		draw_text_marker (dc, _T ("15"), x + locations1 [50]);
		location = draw_markings_for_100 (dc, locations1, _T ("20"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("30"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("40"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("50"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("60"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("70"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("80"), x, location, 0, right_index - 600);
	}
	ScaleLogGamma5Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 700;}
};

class ScaleLogGamma5Down : public ScaleLogGamma5Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLogGamma5Down (int height) : ScaleLogGamma5Up (height) {}
};

class ScaleLnGamma0Up : public StatisticsBase {
public:
	double locations1[900];
	double locations2[300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10( LnGammaF( x ) );}
	virtual double getValue (double x) {return InvLnGammaF( pow( 10, x - 3.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {
			locations1 [ind] = (double) scale_length * (3.0 + log10(LnGammaF(2.002 + (double) ind / 100000.0)));
		}
		for (int ind = 0; ind <= 200; ind++) {
			locations2 [ind] = (double) scale_length * (3.0 + log10(LnGammaF(2.01 + (double) ind / 10000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.003"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.004"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.005"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.006"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.007"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("2.008"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("2.009"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("2.01"), x, location);




		draw_text_marker (dc, _T ("2.015"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("2.02"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("2.03"), x, location, 0, right_index - 100);
	}
	ScaleLnGamma0Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1000;}
};

class ScaleLnGamma0Down : public ScaleLnGamma0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLnGamma0Down (int height) : ScaleLnGamma0Up (height) {}
};
class ScaleLnGamma1Up : public StatisticsBase {
public:
	double locations1[900];
	double locations2[300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10( LnGammaF( x ) );}
	virtual double getValue (double x) {return InvLnGammaF( pow( 10, x - 2.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {
			locations1 [ind] = (double) scale_length * (2.0 + log10(LnGammaF(2.02 + (double) ind / 10000.0)));
		}
		for (int ind = 0; ind <= 200; ind++) {
			locations2 [ind] = (double) scale_length * (2.0 + log10(LnGammaF(2.1 + (double) ind / 1000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.03"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.04"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.05"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.06"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.07"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("2.08"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("2.09"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("2.1"), x, location);
		draw_text_marker (dc, _T ("2.15"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("2.2"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("2.3"), x, location, 0, right_index - 100);
	}
	ScaleLnGamma1Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1000;}
};

class ScaleLnGamma1Down : public ScaleLnGamma1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLnGamma1Down (int height) : ScaleLnGamma1Up (height) {}
};
class ScaleLnGamma2Up : public StatisticsBase {
public:
	double locations1[1300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10( LnGammaF( x ) );}
	virtual double getValue (double x) {return InvLnGammaF( pow( 10, x - 1.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 1200; ind++) {
			locations1 [ind] = (double) scale_length * (1.0 + log10(LnGammaF(2.2 + (double) ind / 1000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 1200, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations1, 1, 1200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("2.3"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("2.4"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("2.5"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("2.6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("2.7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("2.8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("2.9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("3.0"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T ("3.1"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T ("3.2"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T ("3.3"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1100], _T ("3.4"), x, location, 0, right_index - 1100);
	}
	ScaleLnGamma2Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1200;}
};

class ScaleLnGamma2Down : public ScaleLnGamma2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLnGamma2Down (int height) : ScaleLnGamma2Up (height) {}
};
class ScaleLnGamma3Up : public StatisticsBase {
public:
	double locations1[800];
	double locations2[600];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( LnGammaF( x ) );}
	virtual double getValue (double x) {return InvLnGammaF( pow( 10, x ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 700; ind++) {
			locations1 [ind] = (double) scale_length * (log10(LnGammaF(3.3 + (double) ind / 1000.0)));
		}
		for (int ind = 0; ind <= 500; ind++) {
			locations2 [ind] = (double) scale_length * (log10(LnGammaF(4.0 + (double) ind / 100.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {//3.3-4.0 4.0-8.7
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("3.4"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("3.5"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("3.6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("3.7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("3.8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("3.9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("4"), x, location);
		draw_text_marker (dc, _T ("4.5"), x + locations2 [50]);
		location = draw_markings_for_100 (dc, locations2, _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("8"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T ("9"), x, location, 0, right_index - 400);
	}
	ScaleLnGamma3Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1200;}
};

class ScaleLnGamma3Down : public ScaleLnGamma3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLnGamma3Down (int height) : ScaleLnGamma3Up (height) {}
};
class ScaleLnGamma4Up : public StatisticsBase {
public:
	double locations1[1300];
	double locations2[300];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( LnGammaF( x )) - 1.0 ;}
	virtual double getValue (double x) {return InvLnGammaF( pow( 10, x + 1.0 ) );}
	void init_locations (void) {
		for (int ind = 1; ind <= 1200; ind++) {
			locations1 [ind] = (double) scale_length * (log10(LnGammaF(8.0 + (double) ind / 100.0)) - 1.0);
		}
		for (int ind = 0; ind <= 200; ind++) {
			locations2 [ind] = (double) scale_length * (log10(LnGammaF(20.0 + (double) ind / 10.0)) - 1.0);
		}

		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 1200, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("9"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("10"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("11"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("12"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("13"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("14"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("15"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1100], _T ("20"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T ("30"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("40"), x, location, 0, right_index - 100);
	}
	ScaleLnGamma4Up (int height) : StatisticsBase (height) {left_index = 0; right_index = 1400;}
};

class ScaleLnGamma4Down : public ScaleLnGamma4Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLnGamma4Down (int height) : ScaleLnGamma4Up (height) {}
};

bool check_statistical_scales (SetupFileReader & fr, Sliderule * slide_rule) {
/* GJM -- New F Dist Scales (Based on D scale) */
	if (fr . id ("scale_FDist")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v1 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v2 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleFDistUp (powerof10, v1, v2, fr . int_symbol));}
	if (fr . id ("scale_FDist_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v1 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v2 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleFDistDown (powerof10, v1, v2, fr . int_symbol));}
/* GJM -- New Beta Dist Scales (Based on D scale) */
	if (fr . id ("scale_BetaDist")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v1 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v2 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleBetaDistUp (powerof10, v1, v2, fr . int_symbol));}
	if (fr . id ("scale_BetaDist_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v1 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v2 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleBetaDistDown (powerof10, v1, v2, fr . int_symbol));}
/* GJM -- New Students T Dist Scales (Based on D scale) */
	if (fr . id ("scale_StudentsTDist_1Tail")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDist1TailUp (powerof10, v, fr . int_symbol));}
	if (fr . id ("scale_StudentsTDist_1Tail_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDist1TailDown (powerof10, v, fr . int_symbol));}
	if (fr . id ("scale_StudentsTDist_2Tail")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDist2TailUp (powerof10, v, fr . int_symbol));}
	if (fr . id ("scale_StudentsTDist_2Tail_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double v = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDist2TailDown (powerof10, v, fr . int_symbol));}
	if (fr . id ("scale_StudentsTDegOfFreedom")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double area = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDegOfFreedomUp (powerof10, area, fr . int_symbol));}
	if (fr . id ("scale_StudentsTDegOfFreedom_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double area = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleStudentsTDegOfFreedomDown (powerof10, area, fr . int_symbol));}
//* GJM -- New Gamma Dist Scales (Based on D scale) */
	if (fr . id ("scale_GammaDist")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double a = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double b = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleGammaDistUp (powerof10, a, b, fr . int_symbol));}
	if (fr . id ("scale_GammaDist_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double a = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double b = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleGammaDistDown (powerof10, a, b, fr . int_symbol));}
/* GJM -- New Normal Dist Scales (Based on D scale) */
	if (fr . id ("scale_NormalDist_1Tail")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleNormalDist1TailUp (powerof10, fr . int_symbol));}
	if (fr . id ("scale_NormalDist_1Tail_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleNormalDist1TailDown (powerof10, fr . int_symbol));}
	if (fr . id ("scale_NormalDist_2Tail")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleNormalDist2TailUp (powerof10, fr . int_symbol));}
	if (fr . id ("scale_NormalDist_2Tail_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleNormalDist2TailDown (powerof10, fr . int_symbol));}
/* GJM -- New ChiSquare Scales (Based on D scale) */
	if (fr . id ("scale_ChiSquareDist")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double degoffreedom = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleChiSquareDistUp (powerof10, degoffreedom, fr . int_symbol));}
	if (fr . id ("scale_ChiSquareDist_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double degoffreedom = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleChiSquareDistDown (powerof10, degoffreedom, fr . int_symbol));}
	if (fr . id ("scale_ChiSquareDegOfFreedom")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double area = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleChiSquareDegOfFreedomUp (powerof10, area, fr . int_symbol));}
	if (fr . id ("scale_ChiSquareDegOfFreedom_down")) {
		if (! fr . get_float ()) return false;
		double powerof10 = fr . float_symbol;
		if (! fr . get_float ()) return false;
		double area = fr . float_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleChiSquareDegOfFreedomDown (powerof10, area, fr . int_symbol));}
/* GJM -- New LogGamma / LnGamma Scales (Based on LLD scales) */
	if (fr . id ("scale_LogGamma0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma0Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma0Down (fr . int_symbol));}
	if (fr . id ("scale_LogGamma1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma1Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma1Down (fr . int_symbol));}
	if (fr . id ("scale_LogGamma2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma2Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma2Down (fr . int_symbol));}
	if (fr . id ("scale_LogGamma3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma3Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma3Down (fr . int_symbol));}
	if (fr . id ("scale_LogGamma4")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma4Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma4_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma4Down (fr . int_symbol));}
	if (fr . id ("scale_LogGamma5")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma5Up (fr . int_symbol));}
	if (fr . id ("scale_LogGamma5_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLogGamma5Down (fr . int_symbol));}
	if (fr . id ("scale_LnGamma0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma0Up (fr . int_symbol));}
	if (fr . id ("scale_LnGamma0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma0Down (fr . int_symbol));}
	if (fr . id ("scale_LnGamma1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma1Up (fr . int_symbol));}
	if (fr . id ("scale_LnGamma1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma1Down (fr . int_symbol));}
	if (fr . id ("scale_LnGamma2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma2Up (fr . int_symbol));}
	if (fr . id ("scale_LnGamma2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma2Down (fr . int_symbol));}
	if (fr . id ("scale_LnGamma3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma3Up (fr . int_symbol));}
	if (fr . id ("scale_LnGamma3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma3Down (fr . int_symbol));}
	if (fr . id ("scale_LnGamma4")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma4Up (fr . int_symbol));}
	if (fr . id ("scale_LnGamma4_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLnGamma4Down (fr . int_symbol));}
	return true;
}

