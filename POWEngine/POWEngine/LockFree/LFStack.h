#pragma once
#include <optional>

namespace powe
{
	// Lock-Free stack using LIFO context
	template<typename T>
	class LFStack
	{
		struct Node
		{
			Node(T&& data)
				: nextNode()
				, tag()
				, data(std::move(data))
			{
			}

			Node(const T& param)
				: nextNode()
				, tag()
				, data(param)
			{
			}

			SharedPtr<Node> nextNode;
			size_t tag;
			T data;
		};

	public:

		void Push(T&& data) noexcept;
		void Push(const T& data) noexcept;

		[[nodiscard]] std::optional<T> PopReturn(); // maybe has data or not c++17
		void Pop() noexcept;
		bool Empty() const noexcept;

		LFStack()
			: m_Head(std::make_shared<Node>(T{}))
		{
		}

		LFStack(const LFStack&) = delete;
		LFStack& operator=(const LFStack&) = delete;
		LFStack(LFStack&&) noexcept = default;
		LFStack& operator=(LFStack&&) noexcept = default;
		~LFStack();


	private:

		std::atomic<SharedPtr<Node>> m_Head; // since c++20
	};

	template <typename T>
	void LFStack<T>::Push(T&& data) noexcept
	{
		SharedPtr<Node> newNode{ std::make_shared<Node>(std::forward<T>(data)) };
		SharedPtr<Node> oldHead{ m_Head.load() };

		do
		{
			newNode->nextNode = oldHead;
			++oldHead->tag;
		} while (!m_Head.compare_exchange_weak(oldHead, newNode));

	}

	template <typename T>
	void LFStack<T>::Push(const T& data) noexcept
	{
		SharedPtr<Node> newNode{ std::make_shared<Node>(data) };
		SharedPtr<Node> oldHead{ m_Head.load() };

		do
		{
			newNode->nextNode = oldHead;
			++oldHead->tag;
		}
		while (!m_Head.compare_exchange_weak(oldHead,newNode));
	}

	template <typename T>
	std::optional<T> LFStack<T>::PopReturn()
	{
		SharedPtr<Node> oldHead{ m_Head.load() };

		do
		{
			if (!oldHead || !oldHead->nextNode) // never delete the first node
				return std::nullopt;

			++oldHead->tag;
		}
		while (!m_Head.compare_exchange_weak(oldHead,oldHead->nextNode));

		return oldHead->data;
	}

	template <typename T>
	void LFStack<T>::Pop() noexcept
	{
	}

	template <typename T>
	bool LFStack<T>::Empty() const noexcept
	{
		return m_Head.load();
	}

	template <typename T>
	LFStack<T>::~LFStack()
	{
		SharedPtr<Node> head{ m_Head.load() };
		while (head)
		{
			head = std::move(head->nextNode);
		}
	}
}


