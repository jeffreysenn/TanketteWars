#pragma once
#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <memory>
#include <functional>
#include <map>

namespace mw
{

	template<typename BaseState>
	class StateStack
	{
	public:
		typedef ::std::unique_ptr<BaseState> StatePtr;

		enum class Action
		{
			Push,
			Pop,
			Clear
		};

	public:
		StateStack() {}

		template <typename StateType, typename StateIDType>
		void registerState(StateIDType stateID)
		{
			mFactories[(int)stateID] = []()
			{
				return StatePtr(::std::make_unique<StateType>());
			};
		}

		template <typename StateIDTyoe>
		void pushState(StateIDTyoe stateID)
		{
			mPendingList.push_back(PendingChange(Action::Push, (int)stateID));
		}

		void popState()
		{
			mPendingList.push_back(PendingChange(Action::Pop));
		}

		void clearStates()
		{
			mPendingList.push_back(PendingChange(Action::Clear));
		}

		bool isEmpty() const
		{
			return mStack.empty();
		}

	protected:
		void applyPendingChanges();

	private:
		StatePtr createState(int stateID)
		{
			return mFactories[stateID]();
		}

	private:
		struct PendingChange
		{
			explicit PendingChange(const Action& action,
								   const int& stateID = -1)
				: action(action)
				, stateID(stateID)
			{
			}

			Action action;
			int stateID;
		};

	protected:
		::std::vector<StatePtr> mStack;
		::std::vector<PendingChange> mPendingList;

		::std::map<int, ::std::function<StatePtr()>> mFactories;
	};


	template<typename BaseState>
	void StateStack<BaseState>::applyPendingChanges()
	{
		for (auto const& change : mPendingList)
		{
			switch (change.action)
			{
			case Action::Push:
				mStack.push_back(createState(change.stateID));
				break;

			case Action::Pop:
				mStack.pop_back();
				break;

			case Action::Clear:
				mStack.clear();
				break;
			}
		}
		mPendingList.clear();
	}
}