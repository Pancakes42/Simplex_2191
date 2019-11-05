#pragma once

#ifndef _MYOCTANTCLASS_H_
#define _MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{

		static uint m_uOctantCount;
		static uint m_uMaxLevel;
		static uint m_uIdealEntityCount;

		uint m_uID = 0;
		uint m_uLevel = 0;
		uint m_uChildren = 0;

		float m_fSize = 0.0f;

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;

		vector3 m_v3Center = vector3(0.0f);
		vector3 m_v3Min = vector3(0.0f);
		vector3 m_v3Max = vector3(0.0f);

		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChild[8];

		std::vector<uint> m_EntityList;

		MyOctant* m_pRoot = nullptr;
		std::vector<MyOctant*> m_lChild;


	public:
		MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount);

		MyOctant(vector3 a_v3Center, float a_fSize);

		MyOctant& operator=(MyOctant const& other);

		~MyOctant(void);

		bool IsColliding(uint a_uRBIndex);

		void Swap(MyOctant& other);

		float GetSize(void);

		vector3 GetCenterGlobal(void);

		vector3 GetMinGlobal(void);

		vector3 GetMaxGlobal(void);

		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);

		void Display(vector3 a_v3color = C_YELLOW);

		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);

		void ClearEntityList(void);

		void Subdivide(void);

		MyOctant* GetChild(uint a_nChild);

		MyOctant* GetParent(void);

		bool IsLeaf(void);

		bool ContainsMoreThan(uint a_Entities);

		void KillBranches(void);

		void ConstructTree(uint a_nMaxLevel = 3);

		void AssignIDtoEntity(void);

		uint GetOctantCount(void);

	private:

		void Release(void);

		void Init(void);

		void ConstructList(void);

	};

}



#endif
