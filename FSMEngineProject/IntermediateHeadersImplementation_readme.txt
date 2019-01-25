Basically the idea is to reduce the number of possible headers available for including within the
rest of the project code, and to provide some structural padding between how 3rd-party library files
are initially included and how/where they are getting included in the rest of the project.

At the top is the header 'BuildSettings.h', which is to be configured by a user and will impact how all of the
third-party components are built.


The design essentially should look something like this:                                                                                              N,M are both positive natural numbers


          [TOP] 
   [Build-Configuation header]                                                            ___  [BuildSettings.h]  ________________________________________________________________________                     
	       (intended for editing)                                                   _____/    /         |        \____                                            \                       \
	    			                                     	   		          _____/       __/          |             \____                                        |                       \
                                                                       ______/            /             |                  \____                                   |                        \   
		                                     		            _____/                   |              |                       \____                              |                         \
	                                                           V                         V              V                           /|\                           /|\                        /|\                              (etc...)
 [independent 'buffer' headers                           glad_includer.h          glm_includer.h    elcc_includer.h      {widely_used_lib_includers}     {AssetLoading_includers}      {audio_library_includers}
             for 3rd party libraries]                         |          \               |                    \                 |       |       |           |       |       |              |       |       |
			                                                  |           \___           |                     \                |       |       |           |       |       |              |       |       |
															  V               \          |                      \               |       |       |           |       |       |              \        \       \
 [dependent 'buffer' headers                             glfw_includer.h       \         |                       \             /       /       /            |       |       |               \        \       \
             for 3rd party libraries]                         |                 \___     |                        \            |       |       |            |       |       |                |       |       |
			                                                  |                     \    |                         \           |       |       |            |       |       |                |       |       |
															  |                      \   |                          \          |       |       |            |       |       |                |       |       |
                                                              V                       V  V                           V         V       V       V            V       V       V                |       |       |
                                                 +---------------------------+     +------------------+          +---------------------------------+     +-----+-----+--------+         +----------------------------+
[Headers for inclusion                           |GraphicsLanguageFramework.h| <-- |GraphicsLanguage.h|          |       UniversalIncludes.h       |     |image|model|resource|         |   Audio Library Headers    |
            with Application code]               |      [glfw, glm, glad]    |     |    [glm, glad]   |          |  [Logging, other useful stuff]  |     |   [Asset loading]  | (x N)   |     [openal, decoders]     | (x M)
			                                     +---------------------------+     +------------------+          +---------------------------------+     +-----+-----+--------+         +----------------------------+
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
                                                              |                             |                                     |                         |     |       |                     |      |      |
	[Bottom]												  V                             V                                     V                         V     V       V                     V      V      V
												+=========================================================================================================================================================================+
												                                                                       Application Code






Basically the idea is to have an internal implicit 'step' where each specific library gets its own header that is in charge of ensuring
any additional requirements for their respective library are met and that any necessary settings obtained from 'BuildSettings.h' are 
observed before the third party library is included. This will be useful for when importing libraries that come in multiple forms, like
for example 'glad', which has both a debug and a release version. Also, by only allowing the headers below the library importer headers 
to be used by application code, there is a greatly reduced chance of library dependencies being linked out of order.

Also, this let's me hide a lot of the third party libraries in different folders from the main 'source' folder, while then shifting all 
of the responsibility for the larger filepaths in the include statements to headers designed for that specific task.