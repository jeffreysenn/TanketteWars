#pragma once

#include "Command.h"

#include <queue>
namespace mw
{

	class CommandQueue
	{
	public:
		void push(const Command& command) { mQueue.push(command); }

		Command pop()
		{
			Command removedCommand = mQueue.front();
			mQueue.pop();
			return removedCommand;
		}

		bool isEmpty() const { return mQueue.empty(); }

	private:
		::std::queue<Command> mQueue;
	};


}