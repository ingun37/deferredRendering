adding vertex attribute
-add "layout (location = n) in type var" to every .vert files
-modify "getFixedVertexAttributeInfoArray" function. if type is matrix then add four vector types.
-modify "JVERTEXATTRIBUTE" enum. if type is matrix then add four enum
-add member variables to "JVertex" class