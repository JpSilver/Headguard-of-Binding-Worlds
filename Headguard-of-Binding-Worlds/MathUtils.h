#pragma once

#define	Q_PI	3.14159265358979323846

#include <glm\vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <string>
#include <vector>
#include <glm/gtx/quaternion.hpp>

using std::string;
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::vec2;

namespace Math {

	inline void AngleMatrix(const vec3 angles, m_matrix &matrix)
	{
		float		angle;
		float		sr, sp, sy, cr, cp, cy;

		angle = angles.z;
		sy = sin(angle);
		cy = cos(angle);
		angle = angles.y;
		sp = sin(angle);
		cp = cos(angle);
		angle = angles.x;
		sr = sin(angle);
		cr = cos(angle);

		// matrix = (Z * Y) * X
		matrix[0][0] = cp*cy;
		matrix[1][0] = cp*sy;
		matrix[2][0] = -sp;
		matrix[0][1] = sr*sp*cy + cr*-sy;
		matrix[1][1] = sr*sp*sy + cr*cy;
		matrix[2][1] = sr*cp;
		matrix[0][2] = (cr*sp*cy + -sr*-sy);
		matrix[1][2] = (cr*sp*sy + -sr*cy);
		matrix[2][2] = cr*cp;
		matrix[0][3] = 0.0;
		matrix[1][3] = 0.0;
		matrix[2][3] = 0.0;
	};

	inline void AngleQuaternion(const vec3 angles, vec4 &quaternion)
	{
		float		angle;
		float		sr, sp, sy, cr, cp, cy;

		// FIXME: rescale the inputs to 1/2 angle
		angle = angles.z * 0.5;
		sy = sin(angle);
		cy = cos(angle);
		angle = angles.y * 0.5;
		sp = sin(angle);
		cp = cos(angle);
		angle = angles.x * 0.5;
		sr = sin(angle);
		cr = cos(angle);

		quaternion.x = sr*cp*cy - cr*sp*sy; // X
		quaternion.y = cr*sp*cy + sr*cp*sy; // Y
		quaternion.z = cr*cp*sy - sr*sp*cy; // Z
		quaternion.w = cr*cp*cy + sr*sp*sy; // W
	};

	inline void QuaternionMatrix(const vec4 qt, glm::mat3x4 &matrix)
	{
		float quaternion[4] = { qt.x, qt.y, qt.z, qt.w };
		matrix[0][0] = 1.0 - 2.0 * quaternion[1] * quaternion[1] - 2.0 * quaternion[2] * quaternion[2];
		matrix[1][0] = 2.0 * quaternion[0] * quaternion[1] + 2.0 * quaternion[3] * quaternion[2];
		matrix[2][0] = 2.0 * quaternion[0] * quaternion[2] - 2.0 * quaternion[3] * quaternion[1];

		matrix[0][1] = 2.0 * quaternion[0] * quaternion[1] - 2.0 * quaternion[3] * quaternion[2];
		matrix[1][1] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[2] * quaternion[2];
		matrix[2][1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[3] * quaternion[0];

		matrix[0][2] = 2.0 * quaternion[0] * quaternion[2] + 2.0 * quaternion[3] * quaternion[1];
		matrix[1][2] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[3] * quaternion[0];
		matrix[2][2] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[1] * quaternion[1];

	};

	inline void R_ConcatTransforms(const mat3x4 in1, const mat3x4 in2, mat3x4 &out)
	{
		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
			in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
			in1[0][2] * in2[2][2];
		out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
			in1[0][2] * in2[2][3] + in1[0][3];
		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
			in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
			in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
			in1[1][2] * in2[2][2];
		out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
			in1[1][2] * in2[2][3] + in1[1][3];
		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
			in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
			in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
			in1[2][2] * in2[2][2];
		out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
			in1[2][2] * in2[2][3] + in1[2][3];
	}

	inline glm::mat3x4 convertm_Matrix(const m_matrix mat) {
		glm::mat3x4 retMat = glm::mat3x4();
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 3; y++) {
				retMat[y][x] = mat[y][x];
			}
		}
		return retMat;
	}

	inline void QuaternionSlerp(const vec4 p1, const vec4 q1, float t, vec4 &qto)
	{
		float p[4] = { p1.x, p1.y, p1.z, p1.w };
		float q[4] = { q1.x, q1.y, q1.z, q1.w };
		float qt[4];
		int i;
		float omega, cosom, sinom, sclp, sclq;

		// decide if one of the quaternions is backwards
		float a = 0;
		float b = 0;
		for (i = 0; i < 4; i++) {
			a += (p[i] - q[i])*(p[i] - q[i]);
			b += (p[i] + q[i])*(p[i] + q[i]);
		}
		if (a > b) {
			for (i = 0; i < 4; i++) {
				q[i] = -q[i];
			}
		}

		cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

		if ((1.0 + cosom) > 0.00000001) {
			if ((1.0 - cosom) > 0.00000001) {
				omega = acos(cosom);
				sinom = sin(omega);
				sclp = sin((1.0 - t)*omega) / sinom;
				sclq = sin(t*omega) / sinom;
			}
			else {
				sclp = 1.0 - t;
				sclq = t;
			}
			for (i = 0; i < 4; i++) {
				qt[i] = sclp * p[i] + sclq * q[i];
			}
		}
		else {
			qt[0] = -p[1];
			qt[1] = p[0];
			qt[2] = -p[3];
			qt[3] = p[2];
			sclp = sin((1.0 - t) * 0.5 * Q_PI);
			sclq = sin(t * 0.5 * Q_PI);
			for (i = 0; i < 3; i++) {
				qt[i] = sclp * p[i] + sclq * qt[i];
			}
		}
		qto.x = qt[0];
		qto.y = qt[1];
		qto.z = qt[2];
		qto.w = qt[3];
	};

	inline void interpolateTranslation(const vec3 A, const vec3 B, float delta, vec3 &out) {
		out.x = A.x + (B.x - A.x)*delta;
		out.x = A.y + (B.y - A.y)*delta;
		out.x = A.z + (B.z - A.z)*delta;
	}

	inline void interpolateRotation(const vec3 rotA, const vec3 rotB, float delta, vec4 &out) {
		vec4 q1 = vec4(0.0, 0.0, 0.0, 1.0);
		Math::AngleQuaternion(rotA, q1);
		vec4 q2 = vec4(0.0, 0.0, 0.0, 1.0);
		Math::AngleQuaternion(rotB, q2);
		Math::QuaternionSlerp(q1, q2, delta, out);
	};

};