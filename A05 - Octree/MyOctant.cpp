#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;
void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (m_uLevel != 0)return;

	m_uMaxLevel = a_nMaxLevel;

	m_EntityList.clear();

	KillBranches();
	m_lChild.clear();

	if (ContainsMoreThan(m_uIdealEntityCount)) {
		Subdivide();
	}

	AssignIDtoEntity();

	ConstructList();
	


}
void MyOctant::AssignIDtoEntity(void)
{
	for (uint n = 0; n < m_uChildren; n++) {
		m_pChild[n]->AssignIDtoEntity();
	}
	if (m_uChildren == 0) {
		uint entities = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < entities; i++) {
			if (IsColliding(i)) {
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }

void MyOctant::Release(void)
{
	if (m_uLevel == 0) {
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
	
}

void MyOctant::Init(void) {
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr; 
	for (uint n = 0; n < 8; n++) {
		m_pChild[n] = nullptr;
	}
}

void MyOctant::ConstructList(void)
{
	for (uint c = 0; c < m_uChildren; c++) {
		m_pChild[c]->ConstructList();
	}

	if (m_EntityList.size() > 0) {
		m_pRoot->m_lChild.push_back(this);
	}
}

Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

bool MyOctant::IsColliding(uint a_uRBIndex)
{
	uint objectsInScene = m_pEntityMngr->GetEntityCount();

	if (a_uRBIndex >= objectsInScene) {
		return false;
	}

	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* pRigidBody = pEntity->GetRigidBody();

	vector3 v3MinD = pRigidBody->GetMinGlobal();
	vector3 v3MaxD = pRigidBody->GetMaxGlobal();

	if (v3MinD.x < m_v3Min.x)return false;
	if (v3MaxD.x > m_v3Max.x) return false;

	if (v3MinD.y < m_v3Min.y) return false;
	if (v3MaxD.y > m_v3Max.y) return false;

	if (v3MinD.z < m_v3Min.z) return false;
	if (v3MaxD.z > m_v3Max.z) return false;

	return true;
}

void MyOctant::Swap(MyOctant& other) {
	
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max,other.m_v3Max);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_pParent, other.m_pParent);

	for (uint n = 0; n < 8; n++) {
		std::swap(m_pChild[n],other.m_pChild[n]);
	}
}

float MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (m_uLevel == a_nIndex) {
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)
		* glm::scale(vector3(m_fSize)), a_v3Color,RENDER_WIRE);
		return;
	}

	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_nIndex, a_v3Color);
	}

}

void MyOctant::Display(vector3 a_v3color) {
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_v3color);
	}

	

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)
		* glm::scale(vector3(m_fSize)), a_v3color, RENDER_WIRE);
	
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	for (uint i = 0; i < m_uChildren; i++) {
		if (!m_pChild[i]->IsLeaf()) {
			m_pChild[i]->DisplayLeafs(a_v3Color);
		}
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)
		* glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);

}

MyOctant* MyOctant::GetParent(void) {
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_Entities)
{
	return a_Entities > m_EntityList.size();
}

void MyOctant::KillBranches(void)
{
	for (uint i = 0; i < m_uChildren; i++) {
		m_lChild[i]->KillBranches();
	}

	for (uint i = 0; i < m_uChildren; i++) {
		delete m_lChild[i];
	}
}

MyOctant::MyOctant(uint a__nMaxLevel, uint a_nIdealEntityCount) {
	Init();

	m_uOctantCount = 0;
	m_uMaxLevel = a__nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> lMinMax;

	uint Objects = m_pEntityMngr->GetEntityCount();
	for (uint f = 0; f < Objects; f++) {
		MyEntity* pEntity = m_pEntityMngr->GetEntity(f);
		MyRigidBody* pRigidBody = pEntity->GetRigidBody();
		lMinMax.push_back(pRigidBody->GetMinGlobal());
		lMinMax.push_back(pRigidBody->GetMaxGlobal());
	}

	MyRigidBody* pRigidBody = new MyRigidBody(lMinMax);

	vector3 vHalfWidth = pRigidBody->GetHalfWidth();

	float Max = vHalfWidth.x;
	for (int i = 1; i < 3; i++) {
		if (Max < vHalfWidth[i])Max = vHalfWidth[i];
	}

	vector3 v3Center = pRigidBody->GetCenterLocal();

	lMinMax.clear();

	SafeDelete(pRigidBody);

	m_fSize = Max * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(Max));
	m_v3Max = m_v3Center + (vector3(Max));

	m_uOctantCount++;

	ConstructTree(m_uMaxLevel);

}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) {
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;


	m_v3Min = m_v3Center - (vector3(a_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(a_fSize) / 2.0f);

	m_uOctantCount++;

}

MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other) {
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}


void Simplex::MyOctant::ClearEntityList(void)
{
	for (uint c = 0; c < m_uChildren; c++) {
		m_pChild[c]->ClearEntityList();
	}
	m_EntityList.clear();
}

void MyOctant::Subdivide(void) {
	if (m_uLevel >= m_uMaxLevel) {
		return;
	}

	if (m_uChildren != 0) {
		return;
	}

	


	m_uChildren = 8;

	float fSize = m_fSize / 4.0f;
	float fSizeD = fSize * 2.0f;
	vector3 v3Center;

	//First Child (Bottom Left Back)
	v3Center = vector3(m_v3Center.x,m_v3Center.y,m_v3Center.z);
	v3Center.x -= fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;

	//Bottom Left Back
	m_pChild[0] = new MyOctant(v3Center, fSizeD);
	

	//Bottom Right Back
	v3Center.x += fSizeD;
	m_pChild[1] = new MyOctant(v3Center, fSizeD);

	//Bottom Right Front
	v3Center.z += fSizeD;
	m_pChild[2] = new MyOctant(v3Center, fSizeD);

	//Bottom Left Front
	v3Center.x -= fSizeD;
	m_pChild[3] = new MyOctant(v3Center, fSizeD);

	//Top Left Front
	v3Center.y += fSizeD;
	m_pChild[4] = new MyOctant(v3Center, fSizeD);

	//Top Right Front
	v3Center.x += fSizeD;
	m_pChild[5] = new MyOctant(v3Center, fSizeD);

	//Top Right Back
	v3Center.z -= fSizeD;
	m_pChild[6] = new MyOctant(v3Center, fSizeD);

	//Top Left Back
	v3Center.x -= fSizeD;
	m_pChild[7] = new MyOctant(v3Center, fSizeD);

	for (int i = 0; i < 8; i++) {
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pRoot = m_pRoot;

		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount)) {
			m_pChild[i]->Subdivide();
		}
	}

}

MyOctant* MyOctant::GetChild(uint a_nChild) {
	if (a_nChild > 7) return nullptr;
	else return m_pChild[a_nChild];
}



