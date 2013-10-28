#ifndef DV2520_STDAFX_H
#define DV2520_STDAFX_H

// Std:
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <windows.h>
#include <exception>

// Dx:
#include <d3d11.h>
#include <D3D11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

// Win:
#include <windows.h>

// Application:
#include <Util.h> // Utility methods included for brevity.

// Conditionals:
#ifdef DV2520_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // DV2520_DEBUG

#endif // DV2520_STDAFX_H