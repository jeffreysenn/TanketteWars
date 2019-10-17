#pragma once

#include <map>

namespace Collision
{
	enum class ObjectType
	{
		Static,
		Dynamic,
		None,
		Count,
	};
	static const int objectTypeCount = static_cast<int>(ObjectType::Count);


	enum class ResponseType
	{
		Ignore = 0,
		Overlap = 1,
		Collision = 1 << 1,
	};

	inline ResponseType operator & (ResponseType a, ResponseType b)
	{
		return static_cast<ResponseType>(static_cast<int>(a) & static_cast<int>(b));
	}


	enum class ObjectResponsePreset
	{
		CollideAll,
		OverlapAll,
		NoCollision,
	};

	struct ObjectResponse
	{
	public:
		ObjectResponse()
		{
			initializeResponse();
		}

		ObjectResponse(ObjectResponsePreset preset)
		{
			initializeResponse();

			switch (preset)
			{
			case ObjectResponsePreset::CollideAll:
				for (auto &response : responseMap)
				{
					response.second = ResponseType::Collision;
				}
				break;
			case ObjectResponsePreset::OverlapAll:
				for (auto &response : responseMap)
				{
					response.second = ResponseType::Overlap;
				}
				break;
			case ObjectResponsePreset::NoCollision:
				for (auto &response : responseMap)
				{
					response.second = ResponseType::Ignore;
				}
				break;
			}
		}

		ResponseType &operator[](const ObjectType &type)
		{
			return responseMap[type];
		}

		const ResponseType &operator[](const ObjectType &type) const
		{
			return responseMap.at(type);
		}

	private:
		void initializeResponse()
		{
			for (int i = 0; i < objectTypeCount; ++i)
			{
				responseMap[(ObjectType)i] = ResponseType::Ignore;
			}
		}

	private:
		std::map<ObjectType, ResponseType> responseMap;
	};

}
