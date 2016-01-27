#include "Matrix4x4SSE.h"

using namespace DirectX;

Matrix4x4SSE::Matrix4x4SSE()
{
	Identity();
}

Matrix4x4SSE::Matrix4x4SSE(const DirectX::XMMATRIX &M)
{
	r[0] = M.r[0];
	r[1] = M.r[1];
	r[2] = M.r[2];
	r[3] = M.r[3];
}

Matrix4x4SSE::Matrix4x4SSE(const Vec4SSE &R0, const Vec4SSE &R1, const Vec4SSE &R2,
	const Vec4SSE &R3)
{
	r[0] = R0.v;
	r[1] = R1.v;
	r[2] = R2.v;
	r[3] = R3.v;
}

bool Matrix4x4SSE::Decompose(Vec4SSE &Scale, Vec4SSE &RotationQuaternion,
	Vec4SSE &Translation)
{
	return XMMatrixDecompose(&Scale.v, &RotationQuaternion.v, &Translation.v, (*this));
}

Vec4SSE Matrix4x4SSE::Determinant()
{
	return{ XMMatrixDeterminant(*this) };
}

Matrix4x4SSE Matrix4x4SSE::Inverse()
{
	return{ XMMatrixInverse(nullptr, (*this)) };
}

Matrix4x4SSE Matrix4x4SSE::MultiplyTranspose(const Matrix4x4SSE &M)
{
	return{ XMMatrixMultiplyTranspose(*this, M) };
}

Matrix4x4SSE Matrix4x4SSE::Reflect(const Vec4SSE &ReflectionPlane)
{
	return{ XMMatrixReflect(ReflectionPlane.v) };
}

Matrix4x4SSE Matrix4x4SSE::Transpose()const
{
	return{ XMMatrixTranspose(*this) };
}

// Matrix utility functions
bool Matrix4x4SSE::IsIdentity()
{
	return XMMatrixIsIdentity(*this);
}

bool Matrix4x4SSE::IsInfinite()
{
	return XMMatrixIsInfinite(*this);
}

bool Matrix4x4SSE::IsNaN()
{
	return XMMatrixIsNaN(*this);
}

// Setters
Matrix4x4SSE &Matrix4x4SSE::Identity()
{
	*this = XMMatrixIdentity();
	return *this;
}

// Builders
Matrix4x4SSE &Matrix4x4SSE::AffineTransformation(const Vec4SSE &Scaling,
	const Vec4SSE &RotationOrigin, const Vec4SSE &RotationQuaternion,
	const Vec4SSE &Translation)
{
	*this = XMMatrixAffineTransformation(Scaling.v, RotationOrigin.v, 
		RotationQuaternion.v, Translation.v);
	return (*this);
}

Matrix4x4SSE &Matrix4x4SSE::AffineTransformation2D(const Vec4SSE &Scaling,
	const Vec4SSE &RotationOrigin, const float Rotation,
	const Vec4SSE &Translation)
{
	*this = XMMatrixAffineTransformation2D(Scaling.v, RotationOrigin.v,
		Rotation, Translation.v);
	return *this;
}

// Builds a view matrix for a left-handed coordinate system.  Angles the camera
// to point at the specified coordinates.
Matrix4x4SSE &Matrix4x4SSE::LookAtLH(const Vec4SSE &EyePosition, const Vec4SSE &FocusPosition,
	const Vec4SSE &UpDirection)
{
	*this = XMMatrixLookAtLH(EyePosition.v, FocusPosition.v, UpDirection.v);
	return *this;
}

// Builds a view matrix for a right-handed coordinate system.  Angles the camera
// to point at the specified coordinates.
Matrix4x4SSE &Matrix4x4SSE::LookAtRH(const Vec4SSE &EyePosition, const Vec4SSE &FocusPosition,
	const Vec4SSE &UpDirection)
{
	*this = XMMatrixLookAtRH(EyePosition.v, FocusPosition.v, UpDirection.v);
	return *this;
}

// Builds a view matrix for a left-handed coordinate system.  Angles the camera
// to point in the specified direction.
Matrix4x4SSE &Matrix4x4SSE::LookToLH(const Vec4SSE &EyePosition, const Vec4SSE &EyeDirection,
	const Vec4SSE &UpDirection)
{
	*this = LookToLH(EyePosition.v, EyeDirection.v, UpDirection.v);
	return *this;
}

// Builds a view matrix for a right-handed coordinate system.  Angles the camera
// to point in the specified direction.
Matrix4x4SSE &Matrix4x4SSE::LookToRH(const Vec4SSE &EyePosition, const Vec4SSE &EyeDirection,
	const Vec4SSE &UpDirection)
{
	*this = LookToRH(EyePosition.v, EyeDirection.v, UpDirection.v);
	return *this;
}

// Builds an orthogonal projection matrix for left-handed coordinate system
Matrix4x4SSE &Matrix4x4SSE::OrthographicsLH(const float ViewWidth, const float ViewHeight,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
	return *this;
}

// Builds an orthogonal projection matrix for right-handed coordinate system
Matrix4x4SSE &Matrix4x4SSE::OrthographicsRH(const float ViewWidth, const float ViewHeight,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixOrthographicRH(ViewWidth, ViewHeight, NearZ, FarZ);
	return *this;
}

// Builds a custom orthogonal projection matrix for a left-handed coordinate system.
Matrix4x4SSE &Matrix4x4SSE::OrthographicOffCenterLH(const float ViewLeft, const float ViewRight,
	const float ViewBottom, const float ViewTop, const float NearZ,
	const float FarZ)
{
	*this = XMMatrixOrthographicOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop,
		NearZ, FarZ);
	return *this;
}

// Builds a custom orthogonal projection matrix for a right-handed coordinate system.
Matrix4x4SSE &Matrix4x4SSE::OrthographicOffCenterRH(const float ViewLeft, const float ViewRight,
	const float ViewBottom, const float ViewTop, const float NearZ,
	const float FarZ)
{
	*this = XMMatrixOrthographicOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop,
		NearZ, FarZ);
	return *this;
}

// Builds a left-handed perspective projection matrix based on a field of view.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveFovLH(const float FovAngleY, const float AspectRatio,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
	return *this;
}

// Builds a right-handed perspective projection matrix based on a field of view.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveFovRH(const float FovAngleY, const float AspectRatio,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixPerspectiveFovRH(FovAngleY, AspectRatio, NearZ, FarZ);
	return *this;
}

// Builds a left-handed perspective projection matrix.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveLH(const float ViewWidth, const float ViewHeight,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixPerspectiveLH(ViewWidth, ViewHeight, NearZ, FarZ);
	return *this;
}

// Builds a right-handed perspective projection matrix.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveRH(const float ViewWidth, const float ViewHeight,
	const float NearZ, const float FarZ)
{
	*this = XMMatrixPerspectiveRH(ViewWidth, ViewHeight, NearZ, FarZ);
	return *this;
}

// Builds a custom version of a left-handed perspective projection matrix.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveOffCenterLH(const float ViewLeft, const float ViewRight,
	const float ViewBottom, const float ViewTop, const float NearZ,
	const float FarZ)
{
	*this = XMMatrixPerspectiveOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop,
		NearZ, FarZ);
	return *this;
}

// Builds a custom version of a right-handed perspective projection matrix.
Matrix4x4SSE &Matrix4x4SSE::PerspectiveOffCenterRH(const float ViewLeft, const float ViewRight,
	const float ViewBottom, const float ViewTop, const float NearZ,
	const float FarZ)
{
	*this = XMMatrixPerspectiveOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop,
		NearZ, FarZ);
	return *this;
}


Matrix4x4SSE &Matrix4x4SSE::RotateAxis(const Vec4SSE &Axis, const float Angle)
{
	*this = XMMatrixRotationAxis(Axis.v, Angle);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::RotateNormalAxis(const Vec4SSE &Axis, const float Angle)
{
	*this = XMMatrixRotationNormal(Axis.v, Angle);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::RotateQuaternion(const Vec4SSE &Quaternion)
{
	*this = XMMatrixRotationQuaternion(Quaternion.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::RotateFromVector(const Vec4SSE &Angles)
{
	*this = XMMatrixRotationRollPitchYawFromVector(Angles.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::Scale(const Vec4SSE &V)
{
	*this = XMMatrixScalingFromVector(V.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::Shadow(const Vec4SSE &ShadowPlane,
	const Vec4SSE &LightPosition)
{
	*this = XMMatrixShadow(ShadowPlane.v, LightPosition.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::Transformation(const Vec4SSE &ScalingOrigin,
	const Vec4SSE &ScalingOrientationQuaternion, const Vec4SSE &Scaling,
	const Vec4SSE &RotationOrigin, const Vec4SSE &RotationQuaternion,
	const Vec4SSE &Translation)
{
	*this = XMMatrixTransformation(ScalingOrigin.v, ScalingOrientationQuaternion.v,
		Scaling.v, RotationOrigin.v, RotationQuaternion.v, Translation.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::Transformation2D(const Vec4SSE &ScalingOrigin,
	const float ScalingOrientation, const Vec4SSE &Scaling,
	const Vec4SSE &RotationOrigin, const float Rotation,
	const Vec4SSE &Translation)
{
	*this = XMMatrixTransformation2D(ScalingOrigin.v, ScalingOrientation, Scaling.v,
		RotationOrigin.v, Rotation, Translation.v);
	return *this;
}

Matrix4x4SSE &Matrix4x4SSE::Translate(const Vec4SSE &V)
{
	*this = XMMatrixTranslationFromVector(V.v);
	return *this;
}
