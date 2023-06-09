#pragma once

#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

#include "TreeNode.h"

namespace assignment4
{
	template<typename T>
	class TreeNode;

	template<typename T>
	class BinarySearchTree final
	{
	public:
		BinarySearchTree() = default;
		BinarySearchTree(const BinarySearchTree& other) = default;
		~BinarySearchTree() = default;
		BinarySearchTree& operator=(const BinarySearchTree& other) = default;

		void Insert(std::unique_ptr<T> data);
		bool Search(const T& data);
		bool Delete(const T& data);
		const std::weak_ptr<TreeNode<T>> GetRootNode() const;
		void Clear();

		static std::vector<T> TraverseInOrder(const std::shared_ptr<TreeNode<T>> startNode);

	private:
		void insertRecursive(std::shared_ptr<TreeNode<T>> rootNode, std::unique_ptr<T> data);
		std::shared_ptr<TreeNode<T>> searchOrNullRecursive(std::shared_ptr<TreeNode<T>> rootNode, const T& data);
		std::shared_ptr<TreeNode<T>> getSmallerSwapNodeRecursive(std::shared_ptr<TreeNode<T>> startNode);
		std::shared_ptr<TreeNode<T>> getGreaterSwapNodeRecursive(std::shared_ptr<TreeNode<T>> startNode);
		
		static void traverseRecursive(std::vector<T>& outNodes, const std::shared_ptr<TreeNode<T>> currentNode);

	private:
		std::shared_ptr<TreeNode<T>> mRoot;

	};

	template<typename T>
	void BinarySearchTree<T>::Clear()
	{
		mRoot = nullptr;
	}

	template<typename T>
	void BinarySearchTree<T>::Insert(std::unique_ptr<T> data)
	{
		assert(data != nullptr);
		if (mRoot == nullptr)
		{
			std::shared_ptr<TreeNode<T>> node = std::make_shared<TreeNode<T>>(std::move(data));
			mRoot = node;
			return;
		}

		insertRecursive(mRoot, std::move(data));
	}

	template<typename T>
	void BinarySearchTree<T>::insertRecursive(std::shared_ptr<TreeNode<T>> rootNode, std::unique_ptr<T> data)
	{
		if (*rootNode->Data >= *data)
		{
			if (rootNode->Left == nullptr)
			{
				auto newNode = std::make_shared<TreeNode<T>>(rootNode, std::move(data));
				rootNode->Left = newNode;
				return;
			}

			return insertRecursive(rootNode->Left, std::move(data));
		}

		if (*rootNode->Data < *data)
		{
			if (rootNode->Right == nullptr)
			{
				auto newNode = std::make_shared<TreeNode<T>>(rootNode, std::move(data));
				rootNode->Right = newNode;
				return;
			}

			return insertRecursive(rootNode->Right, std::move(data));
		}
	}

	template<typename T>
	const std::weak_ptr<TreeNode<T>> BinarySearchTree<T>::GetRootNode() const
	{
		return mRoot;
	}

	template<typename T>
	bool BinarySearchTree<T>::Search(const T& data)
	{
		if (searchOrNullRecursive(mRoot, data) == nullptr)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	std::shared_ptr<TreeNode<T>> BinarySearchTree<T>::searchOrNullRecursive(std::shared_ptr<TreeNode<T>> rootNode, const T& data)
	{
		if (rootNode == nullptr)
		{
			return nullptr;
		}
		else if (*rootNode->Data == data)
		{
			return rootNode;
		}
		else if (*rootNode->Data > data)
		{
			return searchOrNullRecursive(rootNode->Left, data);
		}
		else
		{
			return searchOrNullRecursive(rootNode->Right, data);
		}
	}

	template<typename T>
	bool BinarySearchTree<T>::Delete(const T& data)
	{
		std::shared_ptr<TreeNode<T>> deleteNode = searchOrNullRecursive(mRoot, data);

		if (deleteNode == nullptr)
		{
			return false;
		}

		if (deleteNode->Left == nullptr && deleteNode->Right == nullptr)
		{
			if (deleteNode == mRoot)
			{
				assert(deleteNode->Parent.lock() == nullptr);

				mRoot = nullptr;
				return true;
			}

			std::shared_ptr<TreeNode<T>> parentNode = deleteNode->Parent.lock();

			if (*parentNode->Data >= *deleteNode->Data)
			{
				parentNode->Left = nullptr;
			}
			else
			{
				parentNode->Right = nullptr;
			}

			return true;
		}

		std::shared_ptr<TreeNode<T>> swapNode;

		if (deleteNode->Left != nullptr)
		{
			swapNode = getSmallerSwapNodeRecursive(deleteNode->Left);
			assert(swapNode != nullptr);
			assert(*deleteNode->Data >= *swapNode->Data);
		}
		else
		{
			swapNode = getGreaterSwapNodeRecursive(deleteNode->Right);
			assert(swapNode != nullptr);
			assert(*deleteNode->Data < *swapNode->Data);
		}

		swapNode->Left = deleteNode->Left;
		swapNode->Right = deleteNode->Right;

		if (swapNode->Left != nullptr)
		{
			swapNode->Left->Parent = swapNode;
		}

		if (swapNode->Right != nullptr)
		{
			swapNode->Right->Parent = swapNode;
		}

		std::shared_ptr<TreeNode<T>> parentNode = deleteNode->Parent.lock();

		if (parentNode == nullptr)
		{
			mRoot = swapNode;

			std::shared_ptr<TreeNode<T>> tmp = nullptr;
			swapNode->Parent = tmp;

			return true;
		}
		
		swapNode->Parent = parentNode;

		if (*parentNode->Data >= *swapNode->Data)
		{
			parentNode->Left = swapNode;
		}
		else
		{
			parentNode->Right = swapNode;
		}

		return true;
	}

	template<typename T>
	std::shared_ptr<TreeNode<T>> BinarySearchTree<T>::getSmallerSwapNodeRecursive(std::shared_ptr<TreeNode<T>> startNode)
	{
		if (startNode->Right == nullptr)
		{
			std::shared_ptr<TreeNode<T>> parent = startNode->Parent.lock();

			if (*parent->Data >= *startNode->Data)
			{
				parent->Left = startNode->Left;
			}
			else
			{
				parent->Right = startNode->Left;
			}

			if (startNode->Left != nullptr)
			{
				startNode->Left->Parent = parent;
			}

			return startNode;
		}

		return getSmallerSwapNodeRecursive(startNode->Right);
	}

	template<typename T>
	std::shared_ptr<TreeNode<T>> BinarySearchTree<T>::getGreaterSwapNodeRecursive(std::shared_ptr<TreeNode<T>> startNode)
	{
		if (startNode->Left == nullptr)
		{
			std::shared_ptr<TreeNode<T>> parent = startNode->Parent.lock();

			if (*parent->Data >= *startNode->Data)
			{
				parent->Left = startNode->Right;
			}
			else
			{
				parent->Right = startNode->Right;
			}

			if (startNode->Right != nullptr)
			{
				startNode->Right->Parent = parent;
			}

			return startNode;
		}

		return getGreaterSwapNodeRecursive(startNode->Left);
	}

	template<typename T>
	std::vector<T> BinarySearchTree<T>::TraverseInOrder(const std::shared_ptr<TreeNode<T>> startNode)
	{
		std::vector<T> v;
		traverseRecursive(v, startNode);

		return v;
	}

	template<typename T>
	void BinarySearchTree<T>::traverseRecursive(std::vector<T>& outNodes, const std::shared_ptr<TreeNode<T>> currentNode)
	{
		if (currentNode == nullptr)
		{
			return;
		}

#ifdef _DEBUG
		if (currentNode->Left != nullptr)
		{
			assert(currentNode->Left->Parent.lock() == currentNode);
		}

		if (currentNode->Right != nullptr)
		{
			assert(currentNode->Right->Parent.lock() == currentNode);
		}
#endif

		traverseRecursive(outNodes, currentNode->Left);
		outNodes.push_back(*currentNode->Data);
		traverseRecursive(outNodes, currentNode->Right);
	}
}