#pragma once
#include "SSEMath.h"
#include "D3DGraphics.h"

class Skeleton
{
	class Bone
	{
	public:
		Bone(const Vec4 &Pos)
			:
			pos(Pos),
			child(nullptr),
			parent(nullptr)
		{}
		void AddChild(Bone *Child, float angle)
		{
			Child->pos = (pos + (pos.Normalize() * Vec4(150.0f, 0.0f, 0.0f, 0.0f).Length()));
			
			Vec4 axis(0.0f, 0.0f, 1.0f, 0.0f);

			if (!child)
			{
				Child->q.RotateAxis(axis, angle);
				Child->parent = this;
				child = Child;
			}
			else
			{
				angle += 0.17f;
				child->AddChild(Child, angle);
			}
		}
		void Update(const Vec4 &RotateAxis, const float angle)
		{
			Quaternion addMe;
			addMe.RotateAxis(RotateAxis, angle);
			q *= addMe;

			if (parent)
			{
				Vec4 one(0.0f, 0.0f, 0.0f, 1.0f);
				Vec4 delta = pos - parent->pos;

				Mat4 parentTranslate, parentOrientation;
				parentTranslate.Translate(parent->pos);
				parentOrientation.RotateQuaternion(parent->q);
				
				Mat4 orientation, translate;
				orientation.RotateQuaternion(q);
				translate.Translate(delta);

				Mat4 m = (orientation * translate)*(parentOrientation * parentTranslate);

				//Vec4 t = one * m;
				pos = one * m;
			}
			if (child)
			{
				child->Update(RotateAxis, angle);
			}
		}
		void Draw(D3DGraphics &gfx)
		{
			if (parent)
			{
				D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
				gfx.DrawLine(parent->pos.X(), parent->pos.Y(), pos.X(), pos.Y(), red);
			}
			else if (child)
			{
				child->Draw(gfx);
			}
		}
		~Bone()
		{
			if (child)
			{
				delete child;
				child = nullptr;
			}
		}

		Vec4 pos, base;
		Quaternion q;
		Bone *child, *parent;
		int index;
	};
public:
	Skeleton();
	Skeleton(const Vec4 &Pos)
		:
		root(nullptr),
		pos(Pos)
	{
		root = new Bone(Pos);

		AddBone();
		AddBone();
		AddBone();
	}
	~Skeleton();

	void AddBone()
	{
		Bone *child = new Bone(pos);
		float angle = 0.17f;
		root->AddChild(child, angle);
	}
	void Update(const float dt)
	{
		root->Update({ 0.0f, 0.0f, 1.0f, 0.0f }, dt);
	}
	void Draw(D3DGraphics &gfx)
	{
		root->Draw(gfx);
	}
private:
	Bone *root;
	Vec4 pos;
};

