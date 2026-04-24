#include "hooks.h"
#include <SKSE/Impl/PCH.h>
#include <REL/REL.h>
#include <REL/Relocation.h>

#include "ui.h"


void InstallHooks()
{
	auto renderer = RE::BSGraphics::Renderer::GetSingleton();
	{
		auto swapChain = renderer->GetCurrentRenderWindow()->swapChain;

		REL::Relocation<std::uintptr_t> vtable_swapChain{ *(std::uintptr_t*)swapChain };

		IDXGISwapChain_Present::func = vtable_swapChain.write_vfunc(8, &IDXGISwapChain_Present::thunk);
		REX::INFO("Successfully hooked Present().");
	} 
	{
		static REL::Relocation<uintptr_t> target{ REL::ID(68617), 0x7B };
		InputHook::func = target.write_call<5>(InputHook::thunk);
		REX::INFO("Successfully hooked whatever input bs that was.");
	}
}