#pragma once
#include "SSEMath.h"
#include "D3DGraphics.h"
#include <memory>

class Skeleton
{
	class Bone
	{
	public:
		Bone(const Vec4 &Pos, unsigned int Bid);
		void AddChild(std::unique_ptr<Bone> &Child, float angle);
		
		void Update();
		void Draw(D3DGraphics &gfx);
				
		Bone *GetBone(unsigned int Bid);

		// These functions assume you have already querried for a specific 
		// bone using the GetBone function.
		Quaternion &GetQuaternion();
		void SetAngle(const float Angle);
		void UpdateAngle(const float Angle);
		~Bone();

	private:
		Vec4 pos, len;
		Quaternion q;
		unsigned int  bid;
		std::unique_ptr<Bone> child;
		Bone *parent;
	};
public:
	Skeleton();
	Skeleton(const Vec4 &Pos);
	~Skeleton();

	Skeleton &operator=(Skeleton &&Skelly);
	void AddBone(const Vec4 &Pos, unsigned int Bid, const float InitialAngle);

	void Update();
	void Bend(unsigned int Bid, const float Angle);
	void Draw(D3DGraphics &gfx);

private:
	Vec4 pos;
	std::unique_ptr<Bone> root;
};

