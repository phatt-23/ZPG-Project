// provides a base for lighting algorithms
//   - it exports needed parameters for these algorithms
#type vertex
#version 400

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_WorldPos;
out vec3 v_WorldNormal;
out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main() {
    vec4 worldPos = u_Model * vec4(a_Pos, 1.f);
    v_WorldPos = vec3(worldPos);
    v_WorldNormal = normalize(transpose(inverse(mat3(u_Model))) * a_Normal);
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProj * worldPos;
}

// Why transform the normal with the transposed inverse of the 3x3 submatrix of the model matrix?
// Normal vectors must rotate with the object. We can't just use the u_Model, because normal vector doesn't
// represent a position, rather it is a direction. If transformed by u_Model, it can be transformed in such a way
// that is no longer perpendicular to the triangle primitive's surface. Thus we need a different matrix.
// To get this "normal model matrix": 
// Get rid of translation by calling mat3. 
// Inverse undoes the rotation and scale. Scale is the culprit behind normal vectors needing to be corrected like this. 
// Transpose to undo the undoing of the rotation, thus rotating it back to the previous rotatoin. Transpose doesn't effect scaling.
// We get a matrix that undoes the scaling but preserves the rotation of the normal vector. 
    