#pragma once
namespace mw
{
enum class NetRole
{
	None,              // No role at all.
	SimulatedProxy,    // Locally simulated proxy of this actor.
	AutonomousProxy,   // Locally autonomous proxy of this actor.
	Authority,         // Authoritative control over the actor.
};
}