/*                                                                */
/* Stage:                     VERTEX SHADER                       */
/*                                                                */
#version 460 core   
// [+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+]
// [|---------------------------  Header  ---------------------------|]
// [+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+]
// [|  File:    Basic.vert                                           |]
// [|  Title:   Basic Vertex Shader                                  |]
// [|  Desc:    Designed for use with Basic.frag to provide a        |]
// [|             very basic rendering pipeline for typical          |]
// [|             mesh geometry rendering.                           |]
// [|  Author:  Forrest S Miller                                     |]
// [|  Date:    March 7, 2019                                        |]
// [+----------------------------------------------------------------+]

  //-----------------//
  //    SIDENOTE:    //
  //-----------------//
 //////////////////// Why All the Fancy Formatting? ////////////////////
//     I recently looked at some VHDL code which was written with a    //
//  very specific format which I thought might translate well to       //
//  serve as a useful GLSL shader format style. The basic idea is      //
//  to have each glsl file be broken into 2 main sections:             //
//                                                                     //
//    (i) an upper section clearly outlining every aspect of this      //
//         stages interface, starting with inputs and ending with      //
//         outputs.                                                    //
//                                                                     //
//   (ii) a lower section dedicated to the shader stage's              //
//         implementation. This section will contain every function,   //
//         including the 'main()' for the stage. [In theory this       //
//         section could exist multiple times where each of them have  //
//         implement the same interface but with separate behavior]    //
//                                                                     //
 ///////////////////////////////////////////////////////////////////////

 //The following macros are used to simplify the syntax throughout the rest
 //of this file.
 #define IN(x) layout(location = x) in
 #define OUT(x) layout(location = x) out



/*                                      SECTION 1                                              \  
/----------------------------------------------------------------------------------------------\  
|                                                                                              |  
|       //---------------------------------------------------------------------//              |  
|       //          /+---------------------------------------------+\          //              |  
|       //         /++---------------------------------------------++\         //              |
|       //        |||                STAGE INTERFACE                |||        //              |
|       //         \++---------------------------------------------++/         //              |
|       //          \+---------------------------------------------+/          //              |
|       //---------------------------------------------------------------------//              |
|                                                                                              |
|          ____                                                           ____                 |
|         <    >                                                         <    >                |
|          \__/                                                           \__/                 |
|            \_                     /~~~~~~~~~~~~~~~\                     _/                   |
|              +--------------------+    INPUTS!    +--------------------+                     |
|            _/                     \~~~~~~~~~~~~~~~/                     \_                   |
|          _/_                                                             _\_                 |
|        /'~~~'\                                                         /'~~~'\               |
|                                                                                              |
|    +------------------------>   EXPECTED PER-VERTEX FORMAT   <------------------------+      |
|    |  Description:                                                                    |      |
|    |    The primary input for a vertex shader is a stream of vertices which are to be |      |
|    |      processed. An instance (thread) of the vertex shader is run for each vertex |      |
|    |      in this stream. The following defines the expected per-vertex data          |      |
|    |      components and layout for each vertex which is to be processed by this      |      |
|    |      shader.                                                                     |      |
|    |                                                                                  |      |
|    |    vertex {                                                                      |      |
|    |*/     IN(0)     vec3 WorldPosition; /*{x,y,z}                                    |      |  //layout(location = 0) in vec3 ModelPosition;  // {X,Y,Z}
|    |*/     IN(1)     vec3 VertNormal;    /*{xN,yN,zN}                                 |      |  //layout(location = 1) in vec3 ModelNormal;    // {Xn, Yn, Zn}
|    |*/     IN(2)     vec2 TextureCoord;  /*{s,t}                                      |      |  //layout(location = 2) in vec2 ModelTexCoord;  // {s, t}
|    |    }                                                                             |      |
|    +----------------------------------------------------------------------------------+      |
|                                                                                              |
|                                                                                              |
|    +------------------------>    PER-Draw-Call Parameters    <------------------------+      |
|    | [i.e. UNIFORMS]                                                                  |      |
|    |  Description: For each draw call there typically is a collection of parameters   |      |    
|    |    which remain constant for every vertex being processed from the stream. These |      |
|    |    parameters are values like for example an offset into world-space shared by   |      |  
|    |    all vertices (a position) or perhaps the current time in a simulation for     |      |
|    |    when the draw call gets made. Values such as these are specified here as      |      |    
|    |    a special type of variable known in GLSL as 'Uniforms'.                       |      |
|    |                                                                                  |      |    
|    |*/     uniform  float  zoom;      /*                                              |      |
|    |*/     uniform  float  time;      /*                                              |      |
|    |*/     uniform   mat4  rotation;  /*                                              |      |
|    |*/     uniform   mat4  MVP;       /* Model View Projection Matrix                 |      |
|    |                                                                                  |      |       
|    +----------------------------------------------------------------------------------+      |
|                                                                                              |    
|                               [WORK STILL IN PROGRESS...]                                    |    
|                                                                                              |    
|                                                                                              |   
|          ____                                                            ____                |
|         <    >                                                          <    >               |
|          \__/                                                            \__/                |
|            \_                     /~~~~~~~~~~~~~~~~\                     _/                  |
|              +--------------------+    OUTPUTS!    +--------------------+                    |
|            _/                     \~~~~~~~~~~~~~~~~/                     \_                  |
|           /                                                                \                 |
|                                                                                              | 
|
|         /////////////////////////////////////////////////
|         //    --OUTPUT          
|         
|         // main() should write to the built-in output variable"
|         //   vec4 gl_Position;
|         //
|         // The following should also be written to so they can be used in 
|         // the later shader stages
|    |*/         OUT(0)      vec3 position;  /*                  //layout(location = 0) out vec3 position;
|    |*/         OUT(1)      vec3 normal;    /*                  //layout(location = 1) out vec3 normal;
|    |*/         OUT(2)      vec2 texCoord;  /*                  //layout(location = 2) out vec2 texCoord;                   
|
|
|      +-----------------------------------------------+
|      |  END            Shader Interface Description  |                                       |
|      +-----------------------------------------------+                                       |
\----------------------------------------------------------------------------------------------/
\                                                END OF SECTION 1                              /    */
















//  BEGIN       Shader Logic Implementation Description

void main() {
/*
	//Do the cool effect on each vertice's position
	float cool = (sin(2.14*(time - (0.001*vert))));
	cool *= step(0.25, abs(cool)); //Cut off asymptotic behavior as 'cool' gets close to 0
	position = ModelPosition + vec4(cos(inst + cool), 1.0 / cool, (3.0*inst) / cool, zoom);

	//Texture Coordinates are just passed straight through unmodified
	texCoord = ModelTexCoord;

	//The normals are rotated to align with the model and then passed through
	normal = mat3(rotation) * ModelNormal;


	gl_Position = (MVP * position) + vec4(-2.0, -2.0, 2.0, 0.0); //Perform an optional translation at the end
    */
}

//  END         Shader Logic Implementation Description