
#pragma once

#include <glm/glm.hpp>
#include "Body.h"
#include <vector>

namespace Nitro
{
	// Axis-Aligned-Bounding-Box
	// A container for objects
	class AABB
	{
	public:
		// center of the box
		glm::vec3 position;

		// half-extents of the box
		glm::vec3 extents;

	public:
		AABB();

		// creates an AABB with the given data
		AABB(const glm::vec3& pos, const glm::vec3& ext);

		// creates an AABB enclosing the given AABBs
		AABB(const AABB& A, const AABB& B);

		// checks if this box overlaps with given box
		bool Overlap(const AABB& other) const;

		// calculates how much this box has to grow
		// to merge with the given box 
		float CalculateGrowth(const AABB& other) const;

		// calculates the volume of the box
		// determines how the tree is recursed
		float CalculateVolume() const;
	};

	// The Binary tree is built from Nodes
	// Each node represents a bounding volume
	// container for all the nodes below the hierarchy
	class AABBNode
	{
		friend class AABBTree;

	protected:
		// pointer to left chid
		AABBNode* left;

		// pointer to right child
		AABBNode* right;

		// pointer to the parent of this node
		AABBNode* parent;

		// The box containing all the boxes below this node
		AABB aabb;

		// The body in this box
		// Only leaf nodes have body (why?)
		Body* body;

		// flag to check if the children of this node have been cross-checked
		bool childrenCrossed;

	public:
		// Creates a new node with the given data
		AABBNode(AABBNode* parent, AABB& aabb, Body* body = nullptr);

		// Checks if this node is at the bottom of the hierarchy
		bool IsLeaf() const;

		// Checks if this node overlaps with given node
		bool Overlap(const AABBNode* other) const;
	};

	// The Binary Tree Hierarchy
	// Maintains the list of potentially colliding Body Pairs
	// Provides methods to:
	// 1. build the tree (insertion method)
	// 2. Pick objects given mouse point
	// 3. Raycast query
	class AABBTree
	{
		// potentially colliding bodies
		struct BodyPair
		{
			BodyPair(Body* A, Body* B)
				: A(A), B(B)
			{}

			Body* A;
			Body* B;
		};

	public:
		// pointer to root node of the tree
		AABBNode* rootNode;

		// The list of potentially colliding body pairs
		std::vector<BodyPair> pairs;

	public:
		// Creatres an AABB tree
		AABBTree();

		// Insert a new node with given data into the tree
		void InsertNode(AABB& aabb, Body* body);

		// Recursive InsertNode helper
		AABBNode* InsertNode(AABBNode* root, AABB& aabb, Body* body);

		// Update tree by recalculating the AABBs
		// starting from this node and propagating up the tree
		void RecalculateAABB(AABBNode* node);

		// Draw the tree
		void DrawTree();

		// Recursively Draw the Tree
		void DrawTree(AABBNode* root);

		void ClearCrossedFlags(AABBNode* node);

		void CrossChildren(AABBNode* node);

		// Calculates the body pairs that are potentially colliding
		void ComputePairs();

		// Recursive ComputePairs helper
		void ComputePairs(AABBNode* n0, AABBNode* n1);
	};
}