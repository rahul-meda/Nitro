
#include "BroadPhase.h"
#include "Cube.h"
#include "Physics.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Nitro
{
	AABB::AABB()
	{}

	AABB::AABB(const glm::vec3& pos, const glm::vec3& ext)
		: position(pos), extents(ext)
	{}

	AABB::AABB(const AABB& A, const AABB& B)
	{
		glm::vec3 de = A.extents - B.extents;
		glm::vec3 d = B.position - A.position;

		// check if one box is completely inside the other
		if ((std::abs(de.x) >= std::abs(d.x)) &&
			std::abs(de.y) >= std::abs(d.y) &&
			std::abs(de.z) >= std::abs(d.z))
		{
			if (A.extents.x > B.extents.x)
			{
				position = A.position;
				extents = A.extents;
			}
			else
			{
				position = B.extents;
				extents = B.extents;
			}
		}
		else
		{
			// the boxes are overlapping
			glm::vec3 minA = A.position - A.extents;
			glm::vec3 maxA = A.position + A.extents;
			glm::vec3 minB = B.position - B.extents;
			glm::vec3 maxB = B.position + B.extents;

			glm::vec3 min;
			glm::vec3 max;
			for (int i = 0; i < 3; i++)
			{
				min[i] = std::fminf(minA[i], minB[i]);
				max[i] = std::fmaxf(maxA[i], maxB[i]);
			}

			position = (min + max)*0.5f;
			extents = (max - min)*0.5f;
		}
	}

	bool AABB::Overlap(const AABB& other) const
	{
		glm::vec3 d = other.position - position;

		if (((extents.x + other.extents.x) > std::abs(d.x)) &&
			((extents.y + other.extents.y) > std::abs(d.y)) &&
			((extents.z + other.extents.z) > std::abs(d.z)))
			return true;

		return false;
	}

	float AABB::CalculateGrowth(const AABB& other) const
	{
		AABB grown(*this, other);

		float growth = grown.CalculateVolume() - CalculateVolume();

		return growth;
	}

	float AABB::CalculateVolume() const
	{
		return extents.x*extents.y*extents.z;
	}

	AABBNode::AABBNode(AABBNode* parent, AABB& aabb, Body* body)
		: parent(parent), aabb(aabb), body(body)
	{
		left = nullptr;
		right = nullptr;
	}

	bool AABBNode::IsLeaf() const
	{
		return (left == nullptr);
	}

	bool AABBNode::Overlap(const AABBNode* other) const
	{
		return (aabb.Overlap(other->aabb));
	}

	AABBTree::AABBTree()
		: rootNode(nullptr)
	{}

	void AABBTree::InsertNode(AABB& aabb, Body* body)
	{
		if (rootNode == nullptr)
		{
			rootNode = new AABBNode(nullptr, aabb, body);
			return;
		}

		InsertNode(rootNode, aabb, body);
	}

	AABBNode* AABBTree::InsertNode(AABBNode* root, AABB& aabb, Body* body)
	{
		// If this is a leaf node,
		// place the bodies in the children
		// parent loses the body
		if (root->IsLeaf())
		{
			// left child gets the parent's body
			root->left = new AABBNode(root, root->aabb, root->body);
			
			// right child gets the given body
			root->right = new AABBNode(root, aabb, body);

			// body is removed from the parent
			root->body = nullptr;

			// recalculate the bounding volume
			RecalculateAABB(root);
		}

		// General case: Recurse into the left or right tree
		// based on which tree would grow the least to incorporate the body
		else
		{
			if (root->left->aabb.CalculateGrowth(aabb) < root->right->aabb.CalculateGrowth(aabb))
			{
				root->left = InsertNode(root->left, aabb, body);
			}
			else
			{
				root->right = InsertNode(root->right, aabb, body);
			}
		}

		return root;
	}

	void AABBTree::RecalculateAABB(AABBNode* node)
	{
		if (node->IsLeaf()) return;

		// merge the child aabbs - parent contains child aabbs
		node->aabb = AABB(node->left->aabb, node->right->aabb);

		// recurse up the tree
		if (node->parent)
			RecalculateAABB(node->parent);
	}

	void AABBTree::DrawTree()
	{
		DrawTree(rootNode);
	}

	void AABBTree::DrawTree(AABBNode* root)
	{
		glm::mat4 M(1), VP(1), MVP(1);
		VP = Camera::GetInstance().GetVPMatrix();
		if (root != nullptr)
		{
			static Cube* box = new Cube(glm::vec3(0,0,1));
			AABB aabb = root->aabb;
			M = glm::translate(aabb.position) * glm::scale(aabb.extents);
			MVP = VP * M;
			box->SetMVP(MVP);
			box->FillBox(false);
			box->Render();

			DrawTree(root->left);
			DrawTree(root->right);
		}
	}

	void AABBTree::ClearCrossedFlags(AABBNode* node)
	{
		node->childrenCrossed = false;
		if (!node->IsLeaf())
		{
			ClearCrossedFlags(node->left);
			ClearCrossedFlags(node->right);
		}
	}

	void AABBTree::CrossChildren(AABBNode* node)
	{
		if (!node->childrenCrossed)
		{
			ComputePairs(node->left, node->right);
			node->childrenCrossed = true;
		}
	}

	void AABBTree::ComputePairs(AABBNode* n0, AABBNode* n1)
	{
		if (n0->IsLeaf())
		{
			// 2 leaves
			if (n1->IsLeaf())
			{
				if (n0->aabb.Overlap(n1->aabb))
					pairs.push_back(BodyPair(n0->body, n1->body));
			}
			// 1 leaf / 1 branch
			else
			{
				CrossChildren(n1);
				ComputePairs(n0, n1->left);
				ComputePairs(n0, n1->right);
			}
		}
		else
		{
			if (n1->IsLeaf())
			{
				CrossChildren(n0);
				ComputePairs(n0->left, n1);
				ComputePairs(n0->right, n1);
			}
			else
			{
				// 2 branches
				CrossChildren(n0);
				CrossChildren(n1);
				ComputePairs(n0->left, n1->left);
				ComputePairs(n0->left, n1->right);
				ComputePairs(n0->right, n1->left);
				ComputePairs(n0->right, n1->right);
			}
		}
	}

	void AABBTree::ComputePairs()
	{
		pairs.clear();

		if (rootNode == nullptr)
			return;

		if (rootNode->IsLeaf())
			return;

		ClearCrossedFlags(rootNode);

		ComputePairs(rootNode->left, rootNode->right);
	}
}