#version 450 core

// Name:                                                           HeatMap.frag
// Type:                                                   Fragment, Secondary* 
// Programmer:                                                   Forrest Miller
// Approx. Date:                                                   May 23, 2019
//                                                          
//                         *(i.e. this file does not define a 'main()' function 
//
// Info: 
//         This is a utility shader composed of the definitions for a family of 
//          functions which are used to compute an isomorphic (1-to-1 and onto)
//               mapping from their specified  domain of GLSL types to a linear
//                             gradient of the various heat values (see below). 
//         
//         [Note that the range of this mapping does not include the entire RGB
//          color space but instead is the set of colors defined below plus the 
//          continuous range of colors produced when interpolating between them]
//                                                                
//
// Reference:     
//          http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
//
//
//                             [HEAT MAP DETAILS]
//      In this heat map, the colors will progress from cold to hot as follows:
//
//
//  Domain is closed interval  [0.0, 1.0]              
//
//    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
//    | 0.0 | 0.1 | 0.2 | 0.3 | 0.4 | 0.5 | 0.6 | 0.7 | 0.8 | 0.9 | 1.0 |
//    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
//      \|/   \|/   \|/   \|/   \|/   \|/   \|/   \|/   \|/   \|/   \|/   
//       /     |     |     |     |     |     |     |     |     |     |     
//     ,/    ,/    ,/    ,/    ,/    ,/    ,/    ,/    ,/    ,/    ,/     
//     |     |     |     |     |     |     |     |     |     |     |
// [COLD]    |     |     |     |     |     |     |     |     |     |   [HOT]  
//     V     V     V     V     V     V     V     V     V     V     V  
//  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------+
//  | COL0  COL1  COL2  COL3  COL4  COL5  COL6  COL7  COL8  COL9  COL10|     
//  +  \   /  \   /  \  /  \  /   \  /  \  /  \  /  \  /  \  /  \  /   +  
//  |  COL05  COL15  COL25 COL35  COL45 COL55 COL65 COL75 COL85 COL95  |
//  +------------------------------------------------------------------+
//    
//  Range is the set of the 20 predefined colorpoints plus the continuous 
//  gradiant of color that is formed when performing the designated
//  interpolation between any consecutive fixed points.
//
//
//                             [Available Colors Pool]

const vec3 BLACK = vec3(0.02, 0.02, 0.02);
const vec3 BLACK_BLUE = vec3(0.02, 0.03, 0.14);
const vec3 DARK_BLUE = vec3(0.02, 0.09, 0.26);
const vec3 DEEP_BLUE = vec3(0.05, 0.25, 0.55);
const vec3 BLUE = vec3(0.07, 0.29, 0.76);
const vec3 DARK_CYAN = vec3(0.1, 0.6, 0.85);
const vec3 CYAN = vec3(0.13, 0.94, 0.98);
const vec3 BLUE_GREEN = vec3(0.14, 0.99, 0.88);
const vec3 LIGHT_GREEN = vec3(0.18, 1.0, 0.7);
const vec3 GREEN = vec3(0.39, 1.0, 0.22);
const vec3 YELLOWISH_GREEN = vec3(0.56, 1.0, 0.19); 
const vec3 GREENISH_YELLOW = vec3(0.7, 0.95, 0.0);      
const vec3 YELLOW = vec3(1.0, 1.0, 0.0);            
const vec3 YELLOW_ORANGE = vec3(1.0, 0.82, 0.0);
const vec3 ORANGE = vec3(1.0, 0.67, 0.0);
const vec3 BLOOD_ORANGE = vec3(1.0, 0.49, 0.0); 
const vec3 ALMOST_FULL_RED = vec3(1.0, 0.24, 0.0);
const vec3 RED = vec3(1.0, 0.0, 0.0);
const vec3 RED_GREY = vec3(1.0, 0.37, 0.37);
const vec3 RED_WHITE = vec3(1.0, 0.75, 0.75);
const vec3 WHITE = vec3(1.0, 1.0, 1.0);

//extra
const vec3 LIGHT_BROWN = vec3(0.3, 0.2, 0.1);

#define NUMBER_OF_FIXED_COLORS 21

vec3 FIXED_COLORS[NUMBER_OF_FIXED_COLORS] = {
                                               BLACK,           /*  COL0   */
                                               BLACK_BLUE,      /*  COL05  */
                                               DARK_BLUE,       /*  COL1   */
                                               DEEP_BLUE,       /*  COL15  */
                                               BLUE,            /*  COL2   */
                                               DARK_CYAN,       /*  COL25  */
                                               CYAN,            /*  COL3   */
                                               BLUE_GREEN,      /*  COL35  */
                                               LIGHT_GREEN,     /*  COL4   */
                                               GREEN,           /*  COL45  */
                                               YELLOWISH_GREEN, /*  COL5   */
                                               GREENISH_YELLOW, /*  COL55  */
                                               YELLOW,          /*  COL6   */
                                               YELLOW_ORANGE,   /*  COL65  */
                                               ORANGE,          /*  COL7   */
                                               BLOOD_ORANGE,    /*  COL75  */
                                               ALMOST_FULL_RED, /*  COL8   */
                                               RED,             /*  COL85  */
                                               RED_GREY,        /*  COL9   */
                                               RED_WHITE,       /*  COL95  */
                                               WHITE,           /*  COL10  */
                                               };

//  [Coldest]
//#define COL0    0
//#define COL05   1
//#define COL1    2
//#define COL15   3
//#define COL2    4
//#define COL25   5
//#define COL3    6
//#define COL35   7
//#define COL4    8
//#define COL45   9
//#define COL5   10
//#define COL55  11
//#define COL6   12
//#define COL65  13
//#define COL7   14
//#define COL75  15
//#define COL8   16
//#define COL85  17
//#define COL9   18
//#define COL95  19
//#define COL10  20
//   [Hotest]

/*                           ///////////////////                                
                             //  PROTOTYPES   //                                
                             ///////////////////                              */


//   AVAILABLE FUNCTIONS

void normalizedIntervalToHeatMapLinear(in /*genType*/float magnitude,
                                        out vec3 heat);

void normalizedIntervalToHeatMapSmoothstep(in /*genType*/float magnitude,
                                            out vec3 heat);


void compactIntervalToHeatMapLinear(in float minMag,
                                    in float maxMag,
                                    in /*genType*/float magnitude,
                                    out vec3 heat);

void compactIntervalToHeatMapSmoothstep(in float minMag,
                                        in float maxMag,
                                        in /*genType*/float magnitude,
                                        out vec3 heat);


//    void convertToHeatMap(in float magnitude, out vec3 tempColor);
//    void convertToHeatMap(in float magnitude, out vec3 tempColor);







/*                           ////////////////////////                          
                            //   IMPLEMENTATION   //                          
                            ////////////////////////                        */


//Most basic version of the 'HeatMap' family of functions.
//Will convert a [genType]/[floating point] value representing a normalized
//magnitude (i.e. 0.0 <= magnitude <= 1.0) to an rgb color representing the 
//corrosponding heat value using a linear interpolation between fixed color
//points. 
// Parameters:
//   [in]   float  magnitude      A float within the closed interval [0.0, 1.0]
//   [out]  vec3   color          
void normalizedIntervalToHeatMapLinear(in /*genType*/float magnitude,
                                        out           vec3  heat) {
    float mag = length(magnitude);
    mag = clamp(mag, 0.0, 1.0);
        
    float colorIndexVal = (mag / 0.05);
        
    int lowerBoundColorIndex = int( floor(colorIndexVal));
    int upperBoundColorIndex = int( ceil(colorIndexVal));

    float interpolationFactor = fract(colorIndexVal);

    heat = mix(FIXED_COLORS[lowerBoundColorIndex],
                FIXED_COLORS[upperBoundColorIndex],
                interpolationFactor);
}


//Most basic version of the 'HeatMap' family of functions.
//Will convert a [genType]/[floating point] value representing a normalized
//magnitude (i.e. 0.0 <= magnitude <= 1.0) to an rgb color representing the 
//corrosponding heat value using a linear interpolation between fixed color
//points. 
// Parameters:
//   [in]   float  magnitude      A float within the closed interval [0.0, 1.0]
//   [out]  vec3   color          The output color value representing the 'heat'
//                                corrosponding to the input magnitude
void normalizedIntervalToHeatMapSmoothstep(in /*genType*/float magnitude,
                                           out vec3 heat) {
    float mag = length(magnitude);
    mag = clamp(mag, 0.0, 1.0);
        
    float colorIndexVal = (mag / 0.05);
        
    int lowerBoundColorIndex = int( floor(colorIndexVal));
    int upperBoundColorIndex = int( ceil(colorIndexVal));

    float interpolationFactor = fract(colorIndexVal);

    //Smoothstep requires 2 'edge' vectors and a third vector parameter
    //located between the 2 edges. So to get this third parameter, we
    //compute the vector between the two edges
    vec3 vectorBetweenFixedColors = 
       FIXED_COLORS[upperBoundColorIndex] - FIXED_COLORS[lowerBoundColorIndex];

    //Then scale it by our interpolationFactor to get a vector 
    //that exists between both edges
    vectorBetweenFixedColors *= interpolationFactor;

    heat = smoothstep(FIXED_COLORS[lowerBoundColorIndex],
                      FIXED_COLORS[upperBoundColorIndex],
                      vectorBetweenFixedColors);  
}





void compactIntervalToHeatMapLinear(in float minMag,
                                    in float maxMag,
                                    in /*genType*/float magnitude,
                                    out vec3 heat) {
    //Implementation is basically the same as above except now 
    //the interval is [minMag, maxMag] instead of [0.0, 1.0] 
    heat = vec3(1.0, 1.0, 1.0);
}




void compactIntervalToHeatMapSmoothstep(in float minMag,
                                        in float maxMag,
                                        in /*genType*/float magnitude,
                                        out vec3 heat) {
                                        
     heat = vec3(1.0, 1.0, 1.0);
}