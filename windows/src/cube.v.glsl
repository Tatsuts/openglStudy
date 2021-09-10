#version 130
in vec3 aPos;
in vec2 aCoord;
in vec3 aNormal;

out vec2 texCoord;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

mat3 transpose(mat3 matrix) {
    vec3 row0 = matrix[0];
    vec3 row1 = matrix[1];
    vec3 row2 = matrix[2];
    mat3 result = mat3(
        vec3(row0.x, row1.x, row2.x),
        vec3(row0.y, row1.y, row2.y),
        vec3(row0.z, row1.z, row2.z)
    );
    return result;
}

float det(mat2 matrix) {
    return matrix[0].x * matrix[1].y - matrix[0].y * matrix[1].x;
}

mat3 inverse(mat3 matrix) {
    vec3 row0 = matrix[0];
    vec3 row1 = matrix[1];
    vec3 row2 = matrix[2];

    vec3 minors0 = vec3(
        det(mat2(row1.y, row1.z, row2.y, row2.z)),
        det(mat2(row1.z, row1.x, row2.z, row2.x)),
        det(mat2(row1.x, row1.y, row2.x, row2.y))
    );
    vec3 minors1 = vec3(
        det(mat2(row2.y, row2.z, row0.y, row0.z)),
        det(mat2(row2.z, row2.x, row0.z, row0.x)),
        det(mat2(row2.x, row2.y, row0.x, row0.y))
    );
    vec3 minors2 = vec3(
        det(mat2(row0.y, row0.z, row1.y, row1.z)),
        det(mat2(row0.z, row0.x, row1.z, row1.x)),
        det(mat2(row0.x, row0.y, row1.x, row1.y))
    );
    mat3 adj = transpose(mat3(minors0, minors1, minors2));

return (1.0 / dot(row0, minors0)) * adj;
}

void main(){
  gl_Position = m_projection * m_view * m_model * vec4(aPos, 1.0);
  //gl_Position = vec4(aPos, 1.0);
  texCoord = aCoord;
  FragPos = vec3(m_model * vec4(aPos, 1.0));
  Normal = transpose(inverse(mat3(m_model))) *aNormal;
};
