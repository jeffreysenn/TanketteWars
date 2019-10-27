#pragma once

namespace mw
{
namespace unit
{
constexpr float PIXEL_PER_TILE = 80;

inline float unit2pix(float uint)
{
	return uint * PIXEL_PER_TILE;
}

inline float pix2unit(float pix)
{
	return pix / PIXEL_PER_TILE;
}
}
}