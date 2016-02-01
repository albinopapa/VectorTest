#pragma once
#include "Vec4SSE.h"
#include "QuaternionSSE.h"

class Matrix4x4SSE :public DirectX::XMMATRIX
{
public:
	Matrix4x4SSE();
	Matrix4x4SSE(const Vec4SSE &R0, const Vec4SSE &R1, const Vec4SSE &R2,
		const Vec4SSE &R3);
	Matrix4x4SSE(const DirectX::XMMATRIX &M);
	
	Matrix4x4SSE operator*(const Matrix4x4SSE &M);
	Vec4SSE operator*(const Vec4SSE &V);
	bool _vectorcall Decompose(Vec4SSE &Scale, Vec4SSE &RotationQuaternion, Vec4SSE &Translation);
	Vec4SSE Determinant();
	Matrix4x4SSE Inverse();
	Matrix4x4SSE _vectorcall MultiplyTranspose(const Matrix4x4SSE &M);
	Matrix4x4SSE _vectorcall Reflect(const Vec4SSE &ReflectionPlane);
	Matrix4x4SSE Transpose()const;

	// Matrix utility functions
	bool IsIdentity();
	bool IsInfinite();
	bool IsNaN();

	// Setters
	Matrix4x4SSE &Identity();

	// Builders
	Matrix4x4SSE &_vectorcall AffineTransformation(const Vec4SSE &Scaling,
		const Vec4SSE &RotationOrigin, const QuaternionSSE &RotationQuaternion,
		const Vec4SSE &Translation);
	Matrix4x4SSE &_vectorcall AffineTransformation2D(const Vec4SSE &Scaling,
		const Vec4SSE &RotationOrigin, const float Rotation,
		const Vec4SSE &Translation);
	// Builds a view matrix for a left-handed coordinate system.  Angles the camera
	// to point at the specified coordinates.
	Matrix4x4SSE &_vectorcall LookAtLH(const Vec4SSE &EyePosition, 
		const Vec4SSE &FocusPosition, const Vec4SSE &UpDirection);
	// Builds a view matrix for a right-handed coordinate system.  Angles the camera
	// to point at the specified coordinates.
	Matrix4x4SSE &_vectorcall LookAtRH(const Vec4SSE &EyePosition, 
		const Vec4SSE &FocusPosition, const Vec4SSE &UpDirection);

	// Builds a view matrix for a left-handed coordinate system.  Angles the camera
	// to point in the specified direction.
	Matrix4x4SSE &_vectorcall LookToLH(const Vec4SSE &EyePosition, 
		const Vec4SSE &EyeDirection, const Vec4SSE &UpDirection);
	// Builds a view matrix for a right-handed coordinate system.  Angles the camera
	// to point in the specified direction.
	Matrix4x4SSE &_vectorcall LookToRH(const Vec4SSE &EyePosition, 
		const Vec4SSE &EyeDirection, const Vec4SSE &UpDirection);

	// Builds an orthogonal projection matrix for left-handed coordinate system
	Matrix4x4SSE &_vectorcall OrthographicsLH(const float ViewWidth, 
		const float ViewHeight, const float NearZ, const float FarZ);
	// Builds an orthogonal projection matrix for right-handed coordinate system
	Matrix4x4SSE &_vectorcall OrthographicsRH(const float ViewWidth, 
		const float ViewHeight, const float NearZ, const float FarZ);

	// Builds a custom orthogonal projection matrix for a left-handed coordinate system.
	Matrix4x4SSE &_vectorcall OrthographicOffCenterLH(const float ViewLeft, 
		const float ViewRight, const float ViewBottom, const float ViewTop, 
		const float NearZ, const float FarZ);
	// Builds a custom orthogonal projection matrix for a right-handed coordinate system.
	Matrix4x4SSE &_vectorcall OrthographicOffCenterRH(const float ViewLeft, 
		const float ViewRight, const float ViewBottom, const float ViewTop, 
		const float NearZ, const float FarZ);

	// Builds a left-handed perspective projection matrix based on a field of view.
	Matrix4x4SSE &_vectorcall PerspectiveFovLH(const float FovAngleY, 
		const float AspectRatio, const float NearZ, const float FarZ);
	// Builds a right-handed perspective projection matrix based on a field of view.
	Matrix4x4SSE &_vectorcall PerspectiveFovRH(const float FovAngleY, 
		const float AspectRatio, const float NearZ, const float FarZ);

	// Builds a left-handed perspective projection matrix.
	Matrix4x4SSE &_vectorcall PerspectiveLH(const float ViewWidth, 
		const float ViewHeight, const float NearZ, const float FarZ);
	// Builds a right-handed perspective projection matrix.
	Matrix4x4SSE &_vectorcall PerspectiveRH(const float ViewWidth, 
		const float ViewHeight, const float NearZ, const float FarZ);

	// Builds a custom version of a left-handed perspective projection matrix.
	Matrix4x4SSE &_vectorcall PerspectiveOffCenterLH(const float ViewLeft, 
		const float ViewRight, const float ViewBottom, const float ViewTop, 
		const float NearZ, const float FarZ);
	// Builds a custom version of a right-handed perspective projection matrix.
	Matrix4x4SSE &_vectorcall PerspectiveOffCenterRH(const float ViewLeft, 
		const float ViewRight, const float ViewBottom, const float ViewTop, 
		const float NearZ, const float FarZ);

	Matrix4x4SSE &_vectorcall RotateAxis(const Vec4SSE &Axis, const float Angle);
	Matrix4x4SSE &_vectorcall RotateNormalAxis(const Vec4SSE &Axis, const float Angle);
	Matrix4x4SSE &_vectorcall RotateQuaternion(const QuaternionSSE &Quaternion);
	Matrix4x4SSE &_vectorcall RotateFromVector(const Vec4SSE &Angles);
	Matrix4x4SSE &_vectorcall Scale(const Vec4SSE &V);
	Matrix4x4SSE &_vectorcall Shadow(const Vec4SSE &ShadowPlane, const Vec4SSE &LightPosition);
	Matrix4x4SSE &_vectorcall Transformation(const Vec4SSE &ScalingOrigin,
		const QuaternionSSE &ScalingOrientationQuaternion, const Vec4SSE &Scaling,
		const Vec4SSE &RotationOrigin, const QuaternionSSE &RotationQuaternion,
		const Vec4SSE &Translation);
	Matrix4x4SSE &_vectorcall Transformation2D(const Vec4SSE &ScalingOrigin,
		const float ScalingOrientation, const Vec4SSE &Scaling,
		const Vec4SSE &RotationOrigin, const float Rotation,
		const Vec4SSE &Translation);
	Matrix4x4SSE &_vectorcall Translate(const Vec4SSE &V);
};
