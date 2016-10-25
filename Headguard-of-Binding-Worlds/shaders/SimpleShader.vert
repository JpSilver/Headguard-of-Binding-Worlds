#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormals;

flat out int hide;

const int MAXLIGHTS = 32;

uniform mat4 mProject;
uniform mat4 mView;
uniform mat4 mModel;

uniform mat3x4 mLocal[128];
uniform mat3x4 mGlobal[128];
//Light///////////////////
uniform vec4 lPos[MAXLIGHTS];
uniform vec3 lRot[MAXLIGHTS];
uniform vec3 color[MAXLIGHTS];
uniform float lRange[MAXLIGHTS];
/////////////////////////////////

out vec2 uv;
out vec3 normal;
out vec3 lightDir;

void bRotate(vec3 in1, mat3x4 in2, inout vec4 d){
	d.x = in1.x*in2[0][0] + in1.y*in2[1][0] + in1.z*in2[2][0];
	d.z = in1.x*in2[0][2] + in1.y*in2[1][2] + in1.z*in2[2][2];
	d.y = in1.x*in2[0][1] + in1.y*in2[1][1] + in1.z*in2[2][1];
	d.w = 1.0;
}

void bTranslate(vec4 data, mat3x4 mat, inout vec4 d){
	vec3 tmp;
	tmp.x = data.x - mat[0][3];
	tmp.y = data.y - mat[1][3];
	tmp.z = data.z - mat[2][3];
	bRotate(tmp, mat, d);
}

float DotProduct(vec3 data, vec4 mat){
	return ((data.x*mat.x) + (data.y * mat.y) + (data.z * mat.z));
}

void boneTransform(vec3 data, mat3x4 mat, inout vec4 d){
	d.x = dot(data, mat[0].xyz) + mat[0][3];
	d.y = dot(data, mat[1].xyz) + mat[1][3];
	d.z = dot(data, mat[2].xyz) + mat[2][3];
	d.w = 1.0;
}

void rotate(vec3 data, mat3x4 mat, inout vec4 norm){
	norm.x = dot(data, mat[0].xyz);
	norm.y = dot(data, mat[1].xyz);
	norm.z = dot(data, mat[2].xyz);
};

void main(){
	mat4 mvp = mProject * mView * mModel;
	int boneID = int(vertexPosition_modelspace.w);
	hide = boneID;
	vec4 vertex = vec4(vertexPosition_modelspace.xyz, 1.0);
	vec4 newPos = vec4(0, 0, 0, 1);
	vec4 pos = vec4(0, 0, 0, 1);
	vec4 tmp = vec4(0, 0, 0, 1);
	vec4 norm = vec4(0, 0, 0, 1);
	
	mat3x4 lmatrix0 = mLocal[boneID];
	mat3x4 boneMatrix = mGlobal[boneID];
	
	bTranslate(vertex, lmatrix0, newPos);
	boneTransform(newPos.xyz, boneMatrix, pos);

	bRotate(vertexNormals, lmatrix0, tmp);
	rotate(tmp.xyz, boneMatrix, norm);

	normal = norm.xyz;

	gl_Position = mvp * (pos);
	vec4 lightPos = lPos[0];
	uv = vertexUV;//vec2(vertexUV.x, -vertexUV.y);
	lightDir = normalize(pos.xyz - lightPos.xyz);
	
}

