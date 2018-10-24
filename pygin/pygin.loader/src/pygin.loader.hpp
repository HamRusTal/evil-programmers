﻿#pragma once

/*
pygin.loader.hpp

*/
/*
Copyright 2017 Alex Alabuzhev
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "system.hpp"

class adapter
{
public:
	bool ModuleInit();
	void ModuleFree();

	BOOL Initialize(GlobalInfo* Info) const noexcept;
	BOOL IsPlugin(const wchar_t* FileName) const noexcept;
	HANDLE CreateInstance(const wchar_t* FileName) const noexcept;
	FARPROC GetFunctionAddress(HANDLE Instance, const wchar_t* FunctionName) const noexcept;
	BOOL GetError(ErrorInfo* Info) const noexcept;
	BOOL DestroyInstance(HANDLE Instance) const noexcept;
	void Free(const ExitInfo* Info) const noexcept;

private:
	void Cleanup();

	module_ptr m_Adapter;

	BOOL (WINAPI *m_Initialize)(GlobalInfo*){};
	BOOL (WINAPI *m_IsPlugin)(const wchar_t*){};
	HANDLE (WINAPI *m_CreateInstance)(const wchar_t*){};
	FARPROC (WINAPI *m_GetFunctionAddress)(HANDLE, const wchar_t*){};
	BOOL (WINAPI *m_GetError)(ErrorInfo*){};
	BOOL (WINAPI *m_DestroyInstance)(HANDLE){};
	void (WINAPI *m_Free)(const ExitInfo*){};

	std::wstring m_Summary;
	std::wstring m_Description;
	bool m_Activated{};
};