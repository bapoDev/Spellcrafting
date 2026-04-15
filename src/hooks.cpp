#include "hooks.h"
#include <SKSE/Impl/PCH.h>
#include <REL/REL.h>

#include "ui.h"


void InstallHooks()
{
	auto swapChain = RE::BSGraphics::Renderer::GetSingleton()->GetCurrentRenderWindow()->swapChain;
	auto trampoline = SKSE::GetTrampolineInterface();

	REL::Relocation<std::uintptr_t> vtable{ *(std::uintptr_t*)swapChain };

	IDXGISwapChain_Present::func = vtable.write_vfunc(8, &IDXGISwapChain_Present::thunk);
	REX::INFO("Successfully hooked Present()");
}