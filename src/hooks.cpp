#include "hooks.h"
#include <SKSE/Impl/PCH.h>
#include <REL/REL.h>
#include <REL/Relocation.h>

#include "ui.h"


void InstallHooks()
{
	{   // Present hook
		auto swapChain = RE::BSGraphics::Renderer::GetSingleton()->GetCurrentRenderWindow()->swapChain;

		REL::Relocation<std::uintptr_t> vtable_swapChain{ *(std::uintptr_t*)swapChain };

		IDXGISwapChain_Present::func = vtable_swapChain.write_vfunc(8, &IDXGISwapChain_Present::thunk);
		REX::INFO("Successfully hooked Present()");
	}
}