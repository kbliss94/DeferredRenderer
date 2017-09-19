#pragma once

//globals
const int MAX_TRIANGLES = 2000;

#include "Terrain.h"
#include "Frustum.h"
#include "TerrainShader.h"

//has the same definition of VertexType that's used in Terrain bc it'll be taking over the storing/rendering of the terrain vertex info

namespace TerrainRenderer
{
	class QuadTree
	{
	private:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
			D3DXVECTOR3 normal;
		};

		struct VectorType
		{
			float x, y, z;
		};

		//each node will be defined as such:
		struct NodeType
		{
			float positionX, positionZ, width;
			int triangleCount;
			ID3D11Buffer *vertexBuffer, *indexBuffer;
			VectorType* vertexArray;
			NodeType* nodes[4];
		};

	public:
		QuadTree();
		QuadTree(const QuadTree& rhs);
		~QuadTree();

		bool Initialize(std::shared_ptr<Terrain> terrain, ID3D11Device* device);
		void Shutdown();
		void Render(Frustum* frustum, ID3D11DeviceContext* context, TerrainShader* shader);

		int GetTriDrawCount();
		int GetTriTotalCount();
		void UpdateTerrainData();
		bool GetHeightAtPosition(float posX, float posZ, float& height);

	private:
		void CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth);
		void CreateTreeNode(NodeType* node, float positionX, float positionZ, float width, ID3D11Device* device);
		int CountTriangles(float positionX, float positionZ, float width);
		bool IsTriangleContained(int index, float positionX, float positionZ, float width);
		void ReleaseNode(NodeType* node);
		void RenderNode(NodeType* node, Frustum* frustum, ID3D11DeviceContext* deviceContext, TerrainShader* shader);
		//Finds the node that contains the given position, then finds the triangle w/in the node that the position is on top of & calculates its height
		void FindNode(NodeType* node, float x, float z, float& height);
		bool CheckHeightOfTriangle(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);

	private:
		int mTriangleCount, mTriDrawCount;

		//list of the vertices from the Terrain object for building the quad tree
		VertexType* mVertexList;
		std::shared_ptr<Terrain> mTerrain;

		//parent node is the root of the quad tree. this node will be expanded recursively to build the entire tree
		NodeType* mParentNode;

		ID3D11Device* mDevice;
	};
}


