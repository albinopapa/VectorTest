#include "Skeleton.h"

Vec4 zRot(0.0f, 0.0f, 1.0f, 0.0f);
Vec4 vIdent(0.0f, 0.0f, 0.0f, 1.0f);

Skeleton::Skeleton()
	:
	root(new Bone({ 0.0f, 0.0f, 0.0f, 0.0f }, 0)),
	pos({ 0.0f, 0.0f, 0.0f, 0.0f })
{
}

Skeleton::Skeleton(const Vec4 &Pos)
	:
	root(new Bone(Pos, 0)),
	pos(Pos)
{
}

Skeleton &Skeleton::operator=(Skeleton &&Skelly)
{
	root= std::move(Skelly.root);
	pos = Skelly.pos;

	return *this;
}

void Skeleton::AddBone(const Vec4 &Pos, unsigned int Bid, const float InitialAngle)
{
	std::unique_ptr<Bone> child(new Bone(Pos, Bid));
	root->AddChild(child, InitialAngle);
}

void Skeleton::Bend(unsigned int Bid, const float Angle)
{
	Bone *seg = root->GetBone(Bid);
	seg->UpdateAngle(Angle);
}

void Skeleton::Update()
{
	root->Update();
}

void Skeleton::Draw(D3DGraphics &gfx)
{
	root->Draw(gfx);
}

Skeleton::~Skeleton()
{
	
}


// Bone class begin
Skeleton::Bone::Bone(const Vec4 &Pos, unsigned int Bid)
	:
	pos(Pos),
	bid(Bid),
	child(nullptr),
	parent(nullptr)
{}

void Skeleton::Bone::AddChild(std::unique_ptr<Bone> &Child, float angle)
{	
	if (!child)
	{
		Child->q.RotationNormal(zRot, angle);
		Child->parent = this;
		child = std::move(Child);
	}
	else
	{
		child->AddChild(Child, angle);
	}
}

void Skeleton::Bone::SetAngle(const float Angle)
{
	float temp = fmodf(Angle, DirectX::XM_2PI);
	q.RotateAxis(zRot, temp);
}

Skeleton::Bone *Skeleton::Bone::GetBone(unsigned int Bid)
{
	Bone *bone = nullptr;

	if (bid == Bid)
	{
		bone = this;
	}
	else if (child)
	{
		bone = child->GetBone(Bid);
	}

	return bone;
}

Quaternion &Skeleton::Bone::GetQuaternion()
{
	return q;
}

void Skeleton::Bone::UpdateAngle(const float Angle)
{
	float curAngle;
	Vec4 curAxis;
	q.GetAxisAndAngle(curAxis, curAngle);

	curAngle += Angle;
	SetAngle(curAngle);

	if (child)
	{
		child->UpdateAngle(Angle);
	}
}

void Skeleton::Bone::Update()
{
	Vec4 curPos = pos;

	if (parent)
	{
		Mat4 parentTranslate, parentOrientation;
		Mat4 orientation, translate;
		Mat4 m, parTransform, transform;
		//parTransform = parTransform.Translate(parent->pos) * parTransform.RotateQuaternion(parent->q);
		parentTranslate.Translate(parent->pos);
		parentOrientation.RotateQuaternion(parent->q);

		orientation.RotateQuaternion(q);
		translate.Translate(len);

		m = (orientation * translate)*(parentOrientation * parentTranslate);

		pos = vIdent * m;
	}
	if (child)
	{
		child->Update();
	}
}

void Skeleton::Bone::Draw(D3DGraphics &gfx)
{
	if (parent)
	{
		D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
		gfx.DrawLine(parent->pos.X(), parent->pos.Y(), pos.X(), pos.Y(), red);
	}
	if (child)
	{
		child->Draw(gfx);
	}
}

Skeleton::Bone::~Bone()
{
}

// Bone class end