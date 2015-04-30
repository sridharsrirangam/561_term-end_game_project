#include "TFT_LCD.h"

const COLOR_T omnom[20][20]=
{
{
{0,0,0}, 
{5,7,4}, 
{0,2,1}, 
{0,1,0}, 
{7,5,6}, 
{5,1,0}, 
{3,0,0}, 
{3,4,0}, 
{0,1,6}, 
{0,5,0}, 
{38,51,25}, 
{15,26,0}, 
{2,4,0}, 
{4,3,1}, 
{0,0,0}, 
{4,4,4}, 
{1,2,0}, 
{4,5,0}, 
{1,0,5}, 
{4,1,12}, 
},

{
{5,5,7}, 
{0,1,3}, 
{0,1,0}, 
{3,5,4}, 
{1,0,2}, 
{4,0,1}, 
{10,6,3}, 
{0,1,0}, 
{0,4,0}, 
{0,9,0}, 
{162,175,149}, 
{42,52,25}, 
{0,2,0}, 
{4,3,1}, 
{2,2,4}, 
{0,0,2}, 
{0,0,0}, 
{0,0,0}, 
{1,0,5}, 
{9,6,17}, 
},

{
{1,0,10}, 
{0,0,5}, 
{4,6,5}, 
{0,1,0}, 
{2,0,5}, 
{5,0,7}, 
{3,0,0}, 
{0,1,0}, 
{1,9,0}, 
{28,41,13}, 
{38,48,23}, 
{0,5,0}, 
{6,7,0}, 
{1,0,0}, 
{0,0,4}, 
{3,2,10}, 
{6,6,16}, 
{0,0,10}, 
{1,0,8}, 
{2,1,9}, 
},

{
{0,0,9}, 
{3,2,10}, 
{0,1,3}, 
{0,1,0}, 
{1,0,5}, 
{4,0,6}, 
{7,2,6}, 
{6,6,0}, 
{22,31,4}, 
{155,169,134}, 
{72,83,53}, 
{1,9,0}, 
{0,1,0}, 
{3,1,4}, 
{0,0,5}, 
{2,1,9}, 
{0,0,7}, 
{0,0,8}, 
{1,0,8}, 
{0,0,5}, 
},

{
{4,2,7}, 
{0,0,4}, 
{0,0,2}, 
{1,2,4}, 
{7,6,11}, 
{30,28,31}, 
{255,253,255}, 
{252,251,246}, 
{246,254,233}, 
{140,151,121}, 
{163,173,139}, 
{190,196,170}, 
{255,254,250}, 
{248,246,251}, 
{10,9,14}, 
{0,0,2}, 
{0,1,0}, 
{5,7,4}, 
{0,0,2}, 
{0,0,4}, 
},

{
{2,0,3}, 
{1,1,1}, 
{4,7,0}, 
{0,5,0}, 
{37,38,33}, 
{255,255,251}, 
{255,254,255}, 
{255,254,250}, 
{252,255,244}, 
{242,252,228}, 
{250,255,230}, 
{254,255,237}, 
{252,251,247}, 
{255,254,255}, 
{250,250,250}, 
{1,4,0}, 
{0,6,0}, 
{0,6,0}, 
{0,1,0}, 
{0,0,2}, 
},

{
{3,0,7}, 
{3,4,0}, 
{0,6,0}, 
{138,147,118}, 
{253,255,235}, 
{255,255,244}, 
{254,253,249}, 
{253,253,251}, 
{1,7,0}, 
{240,248,233}, 
{246,252,240}, 
{0,3,0}, 
{255,255,250}, 
{255,255,250}, 
{251,252,244}, 
{242,246,231}, 
{38,47,18}, 
{0,9,0}, 
{0,3,0}, 
{0,2,1}, 
},

{
{1,0,4}, 
{5,7,0}, 
{22,34,0}, 
{162,177,118}, 
{251,255,222}, 
{254,255,238}, 
{255,255,250}, 
{254,255,250}, 
{0,5,0}, 
{248,255,241}, 
{250,255,246}, 
{0,2,0}, 
{254,254,246}, 
{255,255,242}, 
{250,255,233}, 
{249,255,225}, 
{163,177,128}, 
{42,55,12}, 
{0,5,0}, 
{0,2,0}, 
},

{
{3,4,0}, 
{2,9,0}, 
{165,185,98}, 
{164,185,92}, 
{245,255,205}, 
{254,255,236}, 
{252,255,246}, 
{254,255,248}, 
{250,255,238}, 
{247,255,232}, 
{252,255,241}, 
{251,255,241}, 
{255,255,246}, 
{253,255,237}, 
{248,255,217}, 
{150,165,106}, 
{164,182,108}, 
{166,185,121}, 
{0,9,0}, 
{0,7,0}, 
},

{
{0,1,0}, 
{11,19,0}, 
{157,178,85}, 
{165,191,84}, 
{152,170,94}, 
{245,255,205}, 
{249,255,222}, 
{246,255,219}, 
{243,255,205}, 
{45,63,5}, 
{36,50,1}, 
{246,255,217}, 
{248,255,221}, 
{253,255,219}, 
{238,255,187}, 
{166,188,106}, 
{162,184,102}, 
{45,64,0}, 
{0,7,0}, 
{0,5,0}, 
},

{
{0,3,0}, 
{0,7,0}, 
{88,108,36}, 
{161,186,85}, 
{163,189,79}, 
{170,193,85}, 
{234,255,164}, 
{231,252,159}, 
{149,177,64}, 
{160,189,73}, 
{155,179,81}, 
{155,175,88}, 
{153,170,90}, 
{150,170,85}, 
{158,185,82}, 
{160,187,90}, 
{235,255,190}, 
{219,235,196}, 
{2,12,1}, 
{0,2,4}, 
},

{
{1,3,0}, 
{0,5,0}, 
{240,255,196}, 
{240,255,169}, 
{152,183,56}, 
{166,198,65}, 
{154,181,68}, 
{163,190,77}, 
{162,195,65}, 
{155,188,58}, 
{159,187,74}, 
{165,190,86}, 
{165,187,87}, 
{155,181,74}, 
{164,196,71}, 
{33,66,0}, 
{30,56,0}, 
{32,50,12}, 
{0,8,0}, 
{0,6,9}, 
},

{
{0,2,3}, 
{4,13,0}, 
{35,55,0}, 
{157,184,87}, 
{154,186,61}, 
{41,73,0}, 
{160,187,84}, 
{158,185,88}, 
{155,186,82}, 
{160,191,88}, 
{165,192,99}, 
{156,180,92}, 
{155,180,88}, 
{68,97,0}, 
{113,151,14}, 
{158,198,66}, 
{155,184,91}, 
{41,62,5}, 
{0,7,0}, 
{0,5,0}, 
},

{
{0,2,3}, 
{2,11,0}, 
{37,60,0}, 
{161,192,88}, 
{154,190,58}, 
{156,193,54}, 
{234,255,146}, 
{235,255,155}, 
{36,70,0}, 
{31,65,0}, 
{33,65,0}, 
{45,74,0}, 
{236,255,154}, 
{228,255,137}, 
{158,200,52}, 
{152,195,53}, 
{156,189,82}, 
{2,27,0}, 
{2,16,0}, 
{0,11,3}, 
},

{
{0,5,0}, 
{0,10,0}, 
{59,83,9}, 
{157,192,76}, 
{156,196,45}, 
{156,199,32}, 
{162,202,43}, 
{163,203,44}, 
{157,199,39}, 
{156,198,36}, 
{165,207,47}, 
{154,194,35}, 
{156,196,37}, 
{155,197,37}, 
{153,199,38}, 
{152,196,51}, 
{114,148,36}, 
{113,139,66}, 
{14,28,5}, 
{0,4,3}, 
},

{
{0,5,1}, 
{24,37,11}, 
{118,141,73}, 
{111,145,35}, 
{157,197,49}, 
{159,202,35}, 
{155,196,30}, 
{160,203,36}, 
{158,201,36}, 
{151,196,31}, 
{157,200,35}, 
{154,197,30}, 
{161,204,29}, 
{157,200,32}, 
{150,195,44}, 
{114,153,28}, 
{117,146,54}, 
{116,135,79}, 
{32,43,26}, 
{0,2,4}, 
},

{
{0,3,5}, 
{0,8,0}, 
{119,138,92}, 
{111,138,57}, 
{109,142,27}, 
{161,197,63}, 
{153,191,52}, 
{157,195,58}, 
{157,193,67}, 
{158,193,73}, 
{153,188,72}, 
{107,144,15}, 
{158,198,40}, 
{159,198,43}, 
{153,188,70}, 
{33,61,0}, 
{38,51,5}, 
{24,32,9}, 
{8,11,2}, 
{0,1,0}, 
},

{
{0,2,8}, 
{0,5,0}, 
{0,11,0}, 
{0,17,0}, 
{35,58,0}, 
{161,187,96}, 
{159,185,94}, 
{28,56,0}, 
{36,63,0}, 
{46,72,0}, 
{28,53,0}, 
{36,64,0}, 
{159,189,75}, 
{156,186,72}, 
{163,187,111}, 
{6,22,0}, 
{3,7,0}, 
{2,3,0}, 
{0,1,0}, 
{0,1,0}, 
},

{
{1,1,9}, 
{4,8,7}, 
{0,4,0}, 
{0,8,0}, 
{0,9,0}, 
{1,15,0}, 
{0,6,0}, 
{2,14,0}, 
{0,8,0}, 
{0,12,0}, 
{0,5,0}, 
{0,8,0}, 
{5,18,0}, 
{2,15,0}, 
{0,10,0}, 
{0,4,0}, 
{0,2,0}, 
{0,1,0}, 
{1,2,0}, 
{2,3,0}, 
},

{
{0,0,7}, 
{0,0,2}, 
{5,8,0}, 
{0,4,0}, 
{0,6,0}, 
{0,2,0}, 
{2,6,18}, 
{0,0,12}, 
{0,3,7}, 
{2,8,8}, 
{1,6,12}, 
{0,1,4}, 
{3,9,0}, 
{0,3,0}, 
{0,1,0}, 
{0,1,2}, 
{2,2,0}, 
{1,2,0}, 
{0,1,0}, 
{0,1,0}, 
}
};