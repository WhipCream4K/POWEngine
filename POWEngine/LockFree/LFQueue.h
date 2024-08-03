#pragma once

#include <atomic>
#include <limits>
#include <memory_resource>

namespace powe
{

	template<typename T>
	class LFQueue {

		struct Node {

			Node(T data)
				: data(data)
				, next(nullptr)
				, version(0)
			{
			}

			T data;
			std::atomic<Node*> next;
			int version{};
		};

	public:

		LFQueue(std::pmr::memory_resource* memResource)
			: m_MemResource(memResource)
		{
			static_assert(std::atomic<Node*>::is_always_lock_free, "Atomic Node is not lock free");

			Node* node = new (m_MemResource->allocate(sizeof(Node), alignof(Node))) Node(std::numeric_limits<T>::min());
			m_Head.store(node, std::memory_order_relaxed);
			m_Tail.store(node, std::memory_order_relaxed);
		}

		~LFQueue() {
			while (Node* node = m_Head.exchange(nullptr)) {
				delete node;
			}
		}

		void push(const T& data)
		{
			Node* newNode{ Allocate(data) };
			Node* oldTail;

			do
			{
				oldTail = m_Tail.load(std::memory_order_relaxed);
				newNode->version = oldTail->version + 1;
				oldTail->next.store(newNode, std::memory_order_relaxed);
			} while (!m_Tail.compare_exchange_weak(oldTail, newNode)); // using DCAS
		}

		void push(T&& data) {

			Node* newNode{ Allocate(std::forward<T>(data)) };
			Node* oldTail;

			do
			{
				oldTail = m_Tail.load(std::memory_order_relaxed);
				newNode->version = oldTail->version + 1;
				oldTail->next.store(newNode, std::memory_order_relaxed);
			} while (!m_Tail.compare_exchange_weak(oldTail, newNode)); // using DCAS

		}

		bool pop(T& result) {

			if(m_Head.load(std::memory_order_relaxed) == m_Tail.load(std::memory_order_relaxed))
			{
				return false;
			}

			Node* prevHead{ m_Head.load(std::memory_order_relaxed) };

			Node* newHead;

			while (prevHead != nullptr)
			{
				newHead = prevHead->next.load(std::memory_order_relaxed);

				if (newHead == nullptr)
				{
					return false;
				}

				newHead->version = prevHead->version + 1;

				if (m_Head.compare_exchange_weak(prevHead, newHead, std::memory_order_release, std::memory_order_relaxed))
				{
					break;
				}
			}

			result = std::move(prevHead->data);
			delete prevHead;
			return true;
		}

		bool empty() const
		{
			return m_Head.load(std::memory_order_relaxed) == m_Tail.load(std::memory_order_relaxed);
		}

	private:

		template<typename U>
		Node* Allocate(U&& data) {
			return new (m_MemResource->allocate(sizeof(Node), alignof(Node))) Node(std::forward<U>(data));
		}

	private:

		std::atomic<Node*> m_Head;
		std::atomic<Node*> m_Tail;
		std::pmr::memory_resource* m_MemResource;
	};
}